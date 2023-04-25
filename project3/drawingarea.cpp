#include "drawingarea.h"
#include "line.h"
#include "circle.h"
#include "polygon.h"

#include <QPainter>
#include <QMouseEvent>
#include <QtCore/qfile.h>
#include <QXmlStreamWriter>

DrawingArea::DrawingArea(QWidget *parent) : QWidget(parent), penWidth(1), penColor(Qt::black), antyaliasing(false),
                                            tool(line) {
    setAttribute(Qt::WA_StaticContents);
    resize(1000, 1000);
    QImage initImage(size(), QImage::Format_ARGB32);
    initImage.fill(qRgb(255, 255, 255));
    image = initImage;
}

bool DrawingArea::open(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    clear();
    QXmlStreamReader xmlReader(&file);
    while (!xmlReader.atEnd()) {
        xmlReader.readNext();
        if (xmlReader.isStartElement()) {
            auto shapeName = xmlReader.name().toString();
            if (shapeName == "line") {
                shapes.append(Line::deserialise(xmlReader));
            } else if (shapeName == "circle") {
                shapes.append(Circle::deserialise(xmlReader));
            } else if (shapeName == "polygon") {
                shapes.append(Polygon::deserialise(xmlReader));
            }
        }
    }
    file.close();
    update();
    return true;
}

bool DrawingArea::save(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("shapes");
    for (auto &shape: shapes)
        shape->serialise(xmlWriter);
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    file.close();
    return true;
}

void DrawingArea::clear() {
    shapes.clear();
    image.fill(qRgb(255, 255, 255));
    update();
}

void DrawingArea::setTool() {
    auto *action = qobject_cast<QAction *>(sender());
    if (action) {
        tool = action->data().value<Tools>();
    }
}

void DrawingArea::toggleAntialysing() {
    antyaliasing = !antyaliasing;
    image.fill(qRgb(255, 255, 255));
    update();
}

void DrawingArea::mousePressEvent(QMouseEvent *event) {
    if (event->button() != Qt::LeftButton)
        return;

    backupImage = image.copy();
    switch (tool) {
        case line:
            shapes.append(new Line(event->pos(), penColor, penWidth));
            currentShape = shapes.last();
            break;
        case circle:
            shapes.append(new Circle(event->pos(), penColor, penWidth));
            currentShape = shapes.last();
            break;
        case polygon: {
            auto closestShape = snapClosestShape(event->pos());
            Polygon *polygon = dynamic_cast<Polygon *>(closestShape);
            if (polygon != nullptr && !polygon->isClosed() && polygon->isEnd()) {
                polygon->addPoint(event->pos());
                currentShape = closestShape;
                image.fill(qRgb(255, 255, 255));
                update();
                backupImage = image.copy();
                break;
            }

            shapes.append(new Polygon(event->pos(), penColor, penWidth));
            currentShape = shapes.last();
            break;
        }
        case mov:
        case res:
        case rem:
        case format:
            auto closestShape = snapClosestShape(event->pos());
            if (!closestShape)
                return;
            currentShape = closestShape;
            image.fill(qRgb(255, 255, 255));
            update();
            backupImage = image.copy();

            if (tool == format)
                currentShape->format(penColor, penWidth);
            break;
    }

    if (tool == rem) {
        shapes.removeOne(currentShape);
    }

    update();
}

void DrawingArea::mouseMoveEvent(QMouseEvent *event) {
    if (!(event->buttons() & Qt::LeftButton))
        return;

    switch (tool) {
        case line:
        case circle:
            currentShape->resize(event->pos());
            break;

        case polygon: {
            auto polygon = dynamic_cast<Polygon *>(currentShape);
            if (polygon != nullptr && !polygon->isClosed())
                polygon->resize(event->pos());
            break;
        }
        case mov:
            if (!currentShape)
                return;
            currentShape->move(event->pos());
            break;
        case res:
            if (!currentShape)
                return;
            currentShape->resize(event->pos());
            break;
        case rem:
        case format:
            return;
    }

    image = backupImage.copy();
    update();
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() != Qt::LeftButton)
        return;

    if (tool == polygon) {
        auto polygon = dynamic_cast<Polygon *>(currentShape);
        if (polygon != nullptr)
            polygon->close();
    }

    currentShape = nullptr;
    update();
}

void DrawingArea::paintEvent(QPaintEvent *event) {
    QPainter painter(&image);
    painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    for (auto &shape: shapes) {
        shape->draw(painter, antyaliasing);
    }

    QPainter areaPainter(this);
    QRect dirtyRect = event->rect();
    areaPainter.drawImage(dirtyRect, image, dirtyRect);
}

Shape *DrawingArea::snapClosestShape(const QPoint &point) const {
    int minDistance = 1000000;
    Shape *closestShape = nullptr;
    for (int i = 0; i < shapes.size(); ++i) {
        auto distance = shapes[i]->snap(point);
        if (distance < minDistance) {
            minDistance = distance;
            closestShape = shapes[i];
        }
    }

    if (minDistance > 25)
        return nullptr;

    return closestShape;
}
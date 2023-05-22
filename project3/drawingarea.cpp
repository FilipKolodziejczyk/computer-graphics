#include "drawingarea.h"
#include "line.h"
#include "circle.h"
#include "polygon.h"
#include "rectangle.h"

#include <QPainter>
#include <QMouseEvent>
#include <QtCore/qfile.h>
#include <QXmlStreamWriter>

DrawingArea::DrawingArea(QWidget *parent) : QWidget(parent), _penWidth(1), _penColor(Qt::black), _antyaliasing(false),
                                            _tool(line), _fillingWithImage(false) {
    setAttribute(Qt::WA_StaticContents);
    resize(1000, 1000);
    QImage initImage(size(), QImage::Format_ARGB32);
    initImage.fill(qRgb(255, 255, 255));
    _image = initImage;
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
                _shapes.append(Line::deserialise(xmlReader));
            } else if (shapeName == "circle") {
                _shapes.append(Circle::deserialise(xmlReader));
            } else if (shapeName == "rectangle") {
                _shapes.append(Rectangle::deserialise(xmlReader));
            } else if (shapeName == "polygon") {
                _shapes.append(Polygon::deserialise(xmlReader));
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
    xmlWriter.writeStartElement("_shapes");
    for (auto &shape: _shapes)
        shape->serialise(xmlWriter);
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    file.close();
    return true;
}

void DrawingArea::clear() {
    _shapes.clear();
    _image.fill(qRgb(255, 255, 255));
    update();
}

void DrawingArea::setTool() {
    auto *action = qobject_cast<QAction *>(sender());
    if (action) {
        _tool = action->data().value<Tools>();
    }
}

void DrawingArea::toggleAntialysing() {
    _antyaliasing = !_antyaliasing;
    _image.fill(qRgb(255, 255, 255));
    update();
}

void DrawingArea::mousePressEvent(QMouseEvent *event) {
    if (event->button() != Qt::LeftButton)
        return;

    _editing = true;

    _backupImage = _image.copy();
    switch (_tool) {
        case line:
            _shapes.append(new Line(event->pos(), _penColor, _penWidth));
            _currentShape = _shapes.last();
            break;
        case circle:
            _shapes.append(new Circle(event->pos(), _penColor, _penWidth));
            _currentShape = _shapes.last();
            break;
        case rectangle:
            _shapes.append(new Rectangle(event->pos(), _penColor, _penWidth));
            _currentShape = _shapes.last();
            break;
        case polygon: {
            auto closestShape = snapClosestShape(event->pos());
            auto *polygon = dynamic_cast<Polygon *>(closestShape);
            if (polygon != nullptr && !polygon->isClosed() && polygon->isEnd()) {
                polygon->addPoint(event->pos());
                _currentShape = closestShape;
                _image.fill(qRgb(255, 255, 255));
                update();
                _backupImage = _image.copy();
                break;
            }

            _shapes.append(new Polygon(event->pos(), _penColor, _penWidth));
            _currentShape = _shapes.last();
            break;
        }
        case mov:
        case res:
        case rem:
        case fill:
        case format: {
            auto closestShape = snapClosestShape(event->pos());
            if (!closestShape)
                return;

            _currentShape = closestShape;
            _image.fill(qRgb(255, 255, 255));
            update();
            _backupImage = _image.copy();

            break;
        }
        case clip: {
            auto closestShape = snapClosestShape(event->pos());
            if (!closestShape)
                return;

            if (_clippedShape == nullptr) {
                _clippedShape = closestShape;
            } else {
                auto *rectangle = dynamic_cast<Rectangle *>(closestShape);
                if (rectangle != nullptr && _clippedShape != rectangle) {
                    auto newShapes = _clippedShape->LiangBarskyClip(rectangle);
                    for (auto &shape: newShapes)
                        _shapes.append(shape);
                }

                _clippedShape = nullptr;
            }
        }

    }

    if (_tool == rem)
        _shapes.removeOne(_currentShape);

    if (_tool != clip)
        _clippedShape = nullptr;

    if (_tool == format)
        _currentShape->format(_penColor, _penWidth);

    if (_tool == fill) {
        auto *filledShape = dynamic_cast<FilledShape *>(_currentShape);
        if (filledShape != nullptr && (typeid(*filledShape) != typeid(Polygon) || dynamic_cast<Polygon *>(filledShape)->isClosed())) {
            if (_fillingWithImage) {
                filledShape->setFillingImage(_fillingImage);
            } else {
                filledShape->setFillingColor(_fillingColor);
            }
        }
    }

    update();
}

void DrawingArea::mouseMoveEvent(QMouseEvent *event) {
    if (!(event->buttons() & Qt::LeftButton))
        return;

    switch (_tool) {
        case line:
        case circle:
        case rectangle:
            _currentShape->resize(event->pos());
            break;

        case polygon: {
            auto polygon = dynamic_cast<Polygon *>(_currentShape);
            if (polygon != nullptr && !polygon->isClosed())
                polygon->resize(event->pos());
            break;
        }
        case mov:
            if (!_currentShape)
                return;
            _currentShape->move(event->pos());
            break;
        case res:
            if (!_currentShape)
                return;
            _currentShape->resize(event->pos());
            break;
        case rem:
        case format:
        case clip:
        case fill:
            return;
    }

    _image = _backupImage.copy();
    update();
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() != Qt::LeftButton)
        return;

    _editing = false;

    if (_tool == polygon) {
        auto polygon = dynamic_cast<Polygon *>(_currentShape);
        if (polygon != nullptr)
            polygon->close();
    }

    _currentShape = nullptr;
    update();
}

void DrawingArea::paintEvent(QPaintEvent *event) {
    QPainter painter(&_image);
    painter.setPen(QPen(_penColor, _penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    for (auto &shape: _shapes) {
        FilledShape *filledShape = dynamic_cast<FilledShape *>(shape);
        if (!_editing && filledShape != nullptr)
            filledShape->fill(painter);
    }

    for (auto &shape: _shapes)
        shape->draw(painter, _antyaliasing);

    QPainter areaPainter(this);
    QRect dirtyRect = event->rect();
    areaPainter.drawImage(dirtyRect, _image, dirtyRect);
}

Shape *DrawingArea::snapClosestShape(const QPoint &point) const {
    int minDistance = 1000000;
    Shape *closestShape = nullptr;
    for (auto shape: _shapes) {
        auto distance = shape->snap(point);
        if (distance < minDistance) {
            minDistance = distance;
            closestShape = shape;
        }
    }

    if (minDistance > 25)
        return nullptr;

    return closestShape;
}
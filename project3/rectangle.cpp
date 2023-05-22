#include <QBuffer>
#include "rectangle.h"

Rectangle::Rectangle(QPoint start, QColor color, int width) : FilledShape(color, width),
                                                              _corners{start, start, start, start},
                                                              _snapPointIndex(2) {}

int Rectangle::snap(const QPoint &point) {
    int minDistance = 100000;
    for (int i = 0; i < 4; i++) {
        int distance = qRound(getDistance(_corners[i], point));
        if (distance < minDistance) {
            minDistance = distance;
            _snapPointIndex = i;
        }
    }

    return minDistance;
}

void Rectangle::draw(QPainter &painter, bool antyaliasing) {
    painter.setPen(QPen(_color, 1));

    for (int i = 0; i < 4; i++) {
        Line line(_corners[i], _color, _width);
        line.resize(_corners[(i + 1) % 4]);
        line.draw(painter, antyaliasing);
    }
}

void Rectangle::fill(QPainter &painter) const {
    QImage image = _fillingImage.scaled(right() - left(), bottom() - top());
    painter.setPen(QPen(_fillingColor, 1));

    for (int x = left() + _width; x <= right() - _width; x++) {
        for (int y = top() + _width; y <= bottom() - _width; y++) {
            if (_fillingWithImage)
                painter.setPen(QPen(image.pixelColor(x - left(), y - top()), 1));

            painter.drawPoint(x, y);
        }
    }
}

void Rectangle::move(QPoint newEnd) {
    int dx = newEnd.x() - _corners[_snapPointIndex].x();
    int dy = newEnd.y() - _corners[_snapPointIndex].y();

    for (auto &corner: _corners) {
        corner.setX(corner.x() + dx);
        corner.setY(corner.y() + dy);
    }
}

void Rectangle::resize(QPoint newEnd) {
    _corners[_snapPointIndex] = newEnd;

    if (_snapPointIndex % 2 == 0) {
        _corners[(_snapPointIndex + 1) % 4].setX(newEnd.x());
        _corners[(_snapPointIndex + 3) % 4].setY(newEnd.y());
    } else {
        _corners[(_snapPointIndex + 1) % 4].setY(newEnd.y());
        _corners[(_snapPointIndex + 3) % 4].setX(newEnd.x());
    }
}

QList<Shape *> Rectangle::LiangBarskyClip(const Rectangle *clipper) const {
    QList<Shape *> clippedShapes;
    for (int i = 0; i < 4; i++) {
        Line line(_corners[i], _color, _width);
        line.resize(_corners[(i + 1) % 4]);
        QList<Shape *> clippedLine = line.LiangBarskyClip(clipper);
        for (auto &shape: clippedLine)
            clippedShapes.append(shape);
    }

    return clippedShapes;
}

void Rectangle::serialise(QXmlStreamWriter &writer) {
    writer.writeStartElement("rectangle");
    QString cornersString;
    for (auto &corner: this->_corners)
        cornersString += QString::number(corner.x()) + "," + QString::number(corner.y()) + ";";
    writer.writeAttribute("_corners", cornersString);
    writer.writeAttribute("_color", QString::number(_color.red()) + "," + QString::number(_color.green()) + "," +
                                    QString::number(_color.blue()));
    writer.writeAttribute("_width", QString::number(_width));
    writer.writeAttribute("_fillingColor",
                          QString::number(_fillingColor.red()) + "," + QString::number(_fillingColor.green()) + "," +
                          QString::number(_fillingColor.blue()));
    writer.writeAttribute("_fillingWithImage", QString::number(_fillingWithImage));

    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    _fillingImage.save(&buffer, "PNG");
    QString fillingImageString = QString::fromLatin1(byteArray.toBase64().data());
    writer.writeAttribute("_fillingImage", fillingImageString);

    writer.writeEndElement();
}

Rectangle *Rectangle::deserialise(QXmlStreamReader &reader) {
    QPoint corners[4];
    QColor color = Qt::black;
    int width = 0;
    QColor fillingColor = Qt::white;
    bool fillingWithImage = false;
    QImage fillingImage = QImage();

    QXmlStreamAttributes attributes = reader.attributes();
    if (attributes.hasAttribute("_corners")) {
        for (int i = 0; i < 4; i++) {
            QString corner = attributes.value("_corners").toString().split(";")[i];
            corners[i].setX(corner.split(",")[0].toInt());
            corners[i].setY(corner.split(",")[1].toInt());
        }
    }
    if (attributes.hasAttribute("_color")) {
        QStringList colorList = attributes.value("_color").toString().split(",");
        color.setRgb(colorList[0].toInt(), colorList[1].toInt(), colorList[2].toInt());
    }
    if (attributes.hasAttribute("_width"))
        width = attributes.value("_width").toInt();
    if (attributes.hasAttribute("_fillingColor")) {
        QStringList colorList = attributes.value("_fillingColor").toString().split(",");
        fillingColor.setRgb(colorList[0].toInt(), colorList[1].toInt(), colorList[2].toInt());
    }
    if (attributes.hasAttribute("_fillingWithImage"))
        fillingWithImage = attributes.value("_fillingWithImage").toInt();
    if (attributes.hasAttribute("_fillingImage")) {
        QByteArray byteArray = QByteArray::fromBase64(attributes.value("_fillingImage").toString().toLatin1());
        fillingImage.loadFromData(byteArray, "PNG");
    }

    auto *rectangle = new Rectangle(corners[0], color, width);
    for (int i = 1; i < 4; i++)
        rectangle->_corners[i] = corners[i];
    rectangle->_fillingColor = fillingColor;
    rectangle->_fillingWithImage = fillingWithImage;
    rectangle->_fillingImage = fillingImage;

    return rectangle;
}

int Rectangle::left() const {
    return std::min(_corners[0].x(), _corners[2].x());
}

int Rectangle::right() const {
    return std::max(_corners[0].x(), _corners[2].x());
}

int Rectangle::top() const {
    return std::min(_corners[0].y(), _corners[2].y());
}

int Rectangle::bottom() const {
    return std::max(_corners[0].y(), _corners[2].y());
}
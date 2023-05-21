#include "rectangle.h"

Rectangle::Rectangle(QPoint start, QColor color, int width) : Shape(color, width), _corners{start, start, start, start},
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
    writer.writeAttribute("cornersString", cornersString);
    writer.writeAttribute("_color", _color.name());
    writer.writeAttribute("_width", QString::number(_width));
    writer.writeEndElement();
}

Rectangle *Rectangle::deserialise(QXmlStreamReader &reader) {
    QPoint corners[4];
    QColor color;
    int width = 0;

    QXmlStreamAttributes attributes = reader.attributes();
    if (attributes.hasAttribute("_corners")) {
        for (int i = 0; i < 4; i++) {
            QString corner = attributes.value("_corners").toString().split(";")[i];
            corners[i].setX(corner.split(",")[0].toInt());
            corners[i].setY(corner.split(",")[1].toInt());
        }
    }
    if (attributes.hasAttribute("_color"))
        color.setNamedColor(attributes.value("_color").toString());
    if (attributes.hasAttribute("_width"))
        width = attributes.value("_width").toInt();

    auto *rectangle = new Rectangle(corners[0], color, width);
    for (int i = 1; i < 4; i++)
        rectangle->_corners[i] = corners[i];

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
#include "rectangle.h"

Rectangle::Rectangle(QPoint start, QColor color, int width) : Shape(color, width), corners{start, start, start, start},
                                                              snapPointIndex(2) {}

int Rectangle::snap(const QPoint &point) {
    int minDistance = 100000;
    for (int i = 0; i < 4; i++) {
        int distance = getDistance(corners[i], point);
        if (distance < minDistance) {
            minDistance = distance;
            snapPointIndex = i;
        }
    }

    return minDistance;
}

void Rectangle::draw(QPainter &painter, bool antyaliasing) {
    Shape::draw(painter, antyaliasing);
    painter.setPen(QPen(color, 1));

    for (int i = 0; i < 4; i++) {
        Line line(corners[i], color, width);
        line.resize(corners[(i + 1) % 4]);
        line.draw(painter, antyaliasing);
    }
}

void Rectangle::move(QPoint newEnd) {
    Shape::move(newEnd);
    int dx = newEnd.x() - corners[snapPointIndex].x();
    int dy = newEnd.y() - corners[snapPointIndex].y();

    for (auto &corner: corners) {
        corner.setX(corner.x() + dx);
        corner.setY(corner.y() + dy);
    }
}

void Rectangle::resize(QPoint newEnd) {
    Shape::resize(newEnd);
    corners[snapPointIndex] = newEnd;

    if (snapPointIndex % 2 == 0) {
        corners[(snapPointIndex + 1) % 4].setX(newEnd.x());
        corners[(snapPointIndex + 3) % 4].setY(newEnd.y());
    } else {
        corners[(snapPointIndex + 1) % 4].setY(newEnd.y());
        corners[(snapPointIndex + 3) % 4].setX(newEnd.x());
    }
}

void Rectangle::serialise(QXmlStreamWriter &writer) {
    writer.writeStartElement("rectangle");
    QString cornersString;
    for (auto &corner: this->corners)
        cornersString += QString::number(corner.x()) + "," + QString::number(corner.y()) + ";";
    writer.writeAttribute("cornersString", cornersString);
    writer.writeAttribute("color", color.name());
    writer.writeAttribute("width", QString::number(width));
    writer.writeEndElement();
}

Rectangle *Rectangle::deserialise(QXmlStreamReader &reader) {
    QPoint corners[4];
    QColor color;
    int width = 0;

    QXmlStreamAttributes attributes = reader.attributes();
    if (attributes.hasAttribute("corners")) {
        for (int i = 0; i < 4; i++) {
            QString corner = attributes.value("corners").toString().split(";")[i];
            corners[i].setX(corner.split(",")[0].toInt());
            corners[i].setY(corner.split(",")[1].toInt());
        }
    }
    if (attributes.hasAttribute("color"))
        color.setNamedColor(attributes.value("color").toString());
    if (attributes.hasAttribute("width"))
        width = attributes.value("width").toInt();

    auto *rectangle = new Rectangle(corners[0], color, width);
    for (int i = 1; i < 4; i++)
        rectangle->corners[i] = corners[i];

    return rectangle;
}
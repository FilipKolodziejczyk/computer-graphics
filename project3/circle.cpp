#include "circle.h"

Circle::Circle(QPoint center, QColor color, int width) : Shape(color, width), center(center), snapped(center), radius(0) {}

int Circle::snap(const QPoint &point) {
    snapped = point;
    return abs(radius - getDistance(center, point));
}

void Circle::draw(QPainter &painter, bool antyaliasing) {
    Shape::draw(painter, antyaliasing);
    painter.setPen(QPen(color, 1));

    int dE = 3;
    int dSE = 5 - 2 * radius;
    int d = 1 - radius;
    int x = 0;
    int y = radius;
    painter.drawPoint(center.x() + x, center.y() + y);

    while (y > x) {
        if (d < 0) {
            d += dE;
            dE += 2;
            dSE += 2;
        } else {
            d += dSE;
            dE += 2;
            dSE += 4;
            y--;
        }
        x++;
        brush(painter, center.x() + x, center.y() + y);
        brush(painter, center.x() + x, center.y() - y);
        brush(painter, center.x() - x, center.y() + y);
        brush(painter, center.x() - x, center.y() - y);
        brush(painter, center.x() + y, center.y() + x);
        brush(painter, center.x() + y, center.y() - x);
        brush(painter, center.x() - y, center.y() + x);
        brush(painter, center.x() - y, center.y() - x);
    }
}

void Circle::move(QPoint newPosition) {
    Shape::move(newPosition);
    int dx = newPosition.x() - snapped.x();
    int dy = newPosition.y() - snapped.y();
    center.setX(center.x() + dx);
    center.setY(center.y() + dy);
    snapped.setX(snapped.x() + dx);
    snapped.setY(snapped.y() + dy);
}

void Circle::resize(QPoint newPosition) {
    Shape::resize(newPosition);
    radius = getDistance(center, newPosition);
}

QList<Shape *> Circle::LiangBarskyClip(const Rectangle *clipper) const {
    qDebug() << "Circle cannot be clipped";
}

void Circle::serialise(QXmlStreamWriter &writer) {
    writer.writeStartElement("circle");
    writer.writeAttribute("center", QString::number(center.x()) + "," + QString::number(center.y()));
    writer.writeAttribute("radius", QString::number(radius));
    writer.writeAttribute("color", color.name());
    writer.writeAttribute("width", QString::number(width));
    writer.writeEndElement();
}

Circle *Circle::deserialise(QXmlStreamReader &reader) {
    QPoint center;
    int radius = 0;
    QColor color;
    int width = 0;

    QXmlStreamAttributes attributes = reader.attributes();
    if (attributes.hasAttribute("center")) {
        QString centerString = attributes.value("center").toString();
        QStringList centerList = centerString.split(",");
        center.setX(centerList[0].toInt());
        center.setY(centerList[1].toInt());
    }
    if (attributes.hasAttribute("radius")) {
        radius = attributes.value("radius").toInt();
    }
    if (attributes.hasAttribute("color"))
        color = QColor(attributes.value("color").toString());
    if (attributes.hasAttribute("width"))
        width = attributes.value("width").toInt();

    auto *circle = new Circle(center, color, width);
    circle->radius = radius;

    return circle;
}
#include "circle.h"

Circle::Circle(QPoint center, QColor color, int width) : Shape(color, width), _center(center), _snapped(center),
                                                         _radius(0) {}

int Circle::snap(const QPoint &point) {
    _snapped = point;
    return qAbs(_radius - qRound(getDistance(_center, point)));
}

void Circle::draw(QPainter &painter, bool antyaliasing) {
    painter.setPen(QPen(_color, 1));

    int dE = 3;
    int dSE = 5 - 2 * _radius;
    int d = 1 - _radius;
    int x = 0;
    int y = _radius;
    painter.drawPoint(_center.x() + x, _center.y() + y);

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
        brush(painter, _center.x() + x, _center.y() + y);
        brush(painter, _center.x() + x, _center.y() - y);
        brush(painter, _center.x() - x, _center.y() + y);
        brush(painter, _center.x() - x, _center.y() - y);
        brush(painter, _center.x() + y, _center.y() + x);
        brush(painter, _center.x() + y, _center.y() - x);
        brush(painter, _center.x() - y, _center.y() + x);
        brush(painter, _center.x() - y, _center.y() - x);
    }
}

void Circle::move(QPoint newPosition) {
    int dx = newPosition.x() - _snapped.x();
    int dy = newPosition.y() - _snapped.y();
    _center.setX(_center.x() + dx);
    _center.setY(_center.y() + dy);
    _snapped.setX(_snapped.x() + dx);
    _snapped.setY(_snapped.y() + dy);
}

void Circle::resize(QPoint newPosition) {
    _radius = qRound(getDistance(_center, newPosition));
}

QList<Shape *> Circle::LiangBarskyClip(const Rectangle *clipper) const {
    qDebug() << "Circle cannot be clipped";
    return {};
}

void Circle::serialise(QXmlStreamWriter &writer) {
    writer.writeStartElement("circle");
    writer.writeAttribute("_center", QString::number(_center.x()) + "," + QString::number(_center.y()));
    writer.writeAttribute("_radius", QString::number(_radius));
    writer.writeAttribute("_color", _color.name());
    writer.writeAttribute("_width", QString::number(_width));
    writer.writeEndElement();
}

Circle *Circle::deserialise(QXmlStreamReader &reader) {
    QPoint center;
    int radius = 0;
    QColor color;
    int width = 0;

    QXmlStreamAttributes attributes = reader.attributes();
    if (attributes.hasAttribute("_center")) {
        QString centerString = attributes.value("_center").toString();
        QStringList centerList = centerString.split(",");
        center.setX(centerList[0].toInt());
        center.setY(centerList[1].toInt());
    }
    if (attributes.hasAttribute("_radius")) {
        radius = attributes.value("_radius").toInt();
    }
    if (attributes.hasAttribute("_color"))
        color = QColor(attributes.value("_color").toString());
    if (attributes.hasAttribute("_width"))
        width = attributes.value("_width").toInt();

    auto *circle = new Circle(center, color, width);
    circle->_radius = radius;

    return circle;
}
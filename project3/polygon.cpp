#include "shape.h"
#include "polygon.h"
#include "line.h"

Polygon::Polygon(QPoint start, QColor color, int width) : Shape(color, width) {
    points.append(start);
    points.append(start);
}

int Polygon::snap(const QPoint &point) {
    int minIndex = 0;
    int minDistance = getDistance(points[0], point);
    for (int i = 1; i < points.size(); i++) {
        int distance = getDistance(points[i], point);
        if (distance < minDistance) {
            minDistance = distance;
            minIndex = i;
        }
    }
    snapped = minIndex;
    if (minIndex == 0) {
        std::reverse(points.begin(), points.end());
        snapped = points.size() - 1;
    }
    return minDistance;
}

void Polygon::draw(QPainter &painter, bool antyaliasing) {
    Shape::draw(painter, antyaliasing);
    painter.setPen(QPen(color, width));
    for (int i = 0; i < points.size() - 1; i++) {
        Line line(points[i], color, width);
        line.resize(points[i + 1]);
        line.draw(painter, antyaliasing);
    }
}

void Polygon::move(QPoint newEnd) {
    Shape::move(newEnd);
    int dx = newEnd.x() - points[snapped].x();
    int dy = newEnd.y() - points[snapped].y();
    for (int i = 0; i < points.size(); i++) {
        points[i].setX(points[i].x() + dx);
        points[i].setY(points[i].y() + dy);
    }
}

void Polygon::resize(QPoint newEnd) {
    Shape::resize(newEnd);
    points[snapped] = newEnd;
    if (closed && (snapped == 0 || snapped == points.size() - 1))
        points[0] = points[points.size() - 1] = newEnd;
    else if (points.length() > 2 && getDistance(points[0], points[points.size() - 1]) < 25)
        points.last() = QPoint(points[0]);
}

bool Polygon::isEnd() {
    return snapped == points.size() - 1 || snapped == 0;
}

void Polygon::addPoint(QPoint point) {
    points.append(point);
    snapped = points.size() - 1;
}

void Polygon::close() {
    if (points.length() > 2 && points.first() == points.last())
        closed = true;
}

void Polygon::serialise(QXmlStreamWriter &writer) {
    writer.writeStartElement("polygon");
    QString pointsString;
    for (auto point : points)
        pointsString += QString::number(point.x()) + "," + QString::number(point.y()) + ";";
    writer.writeAttribute("points", pointsString);
    writer.writeAttribute("color", QString::number(color.red()) + "," + QString::number(color.green()) + "," +
                                   QString::number(color.blue()));
    writer.writeAttribute("width", QString::number(width));
    writer.writeEndElement();
}

Polygon *Polygon::deserialise(QXmlStreamReader &reader) {
    QList<QPoint> points;
    QColor color;
    int width = 0;
    bool closed = false;

    QXmlStreamAttributes attributes = reader.attributes();
    if (attributes.hasAttribute("points")) {
        QString pointsString = attributes.value("points").toString();
        QStringList pointsList = pointsString.split(";");
        for (int i = 0; i < pointsList.size() - 1; i++) {
            QStringList pointString = pointsList[i].split(",");
            points.append(QPoint(pointString[0].toInt(), pointString[1].toInt()));
        }
    }
    if (attributes.hasAttribute("color")) {
        QStringList colorString = attributes.value("color").toString().split(",");
        color = QColor(colorString[0].toInt(), colorString[1].toInt(), colorString[2].toInt());
    }
    if (attributes.hasAttribute("width"))
        width = attributes.value("width").toInt();

    auto *polygon = new Polygon(points[0], color, width);
    polygon->points = points;
    return polygon;
}
#include <QBuffer>
#include "shape.h"
#include "polygon.h"
#include "line.h"

Polygon::Polygon(QPoint start, QColor color, int width) : FilledShape(color, width), _closed(false) {
    _points.append(start);
    _points.append(start);
}

int Polygon::snap(const QPoint &point) {
    int minIndex = 0;
    int minDistance = qRound(getDistance(_points[0], point));
    for (int i = 1; i < _points.size(); i++) {
        int distance = qRound(getDistance(_points[i], point));
        if (distance < minDistance) {
            minDistance = distance;
            minIndex = i;
        }
    }
    _snapped = minIndex;
    if (minIndex == 0) {
        std::reverse(_points.begin(), _points.end());
        _snapped = static_cast<int>(_points.size()) - 1;
    }
    return minDistance;
}

void Polygon::draw(QPainter &painter, bool antyaliasing) {
    painter.setPen(QPen(_color, _width));
    for (int i = 0; i < _points.size() - 1; i++) {
        Line line(_points[i], _color, _width);
        line.resize(_points[i + 1]);
        line.draw(painter, antyaliasing);
    }
}

void Polygon::fill(QPainter &painter) const {
    if (!isClosed())
        return;

    if (_fillingWithImage) {

    } else {
        painter.setPen(QPen(_color, 1));

    }
}

void Polygon::move(QPoint newEnd) {
    int dx = newEnd.x() - _points[_snapped].x();
    int dy = newEnd.y() - _points[_snapped].y();
    for (auto &_point: _points) {
        _point.setX(_point.x() + dx);
        _point.setY(_point.y() + dy);
    }
}

void Polygon::resize(QPoint newEnd) {
    _points[_snapped] = newEnd;
    if (_closed && (_snapped == 0 || _snapped == _points.size() - 1))
        _points[0] = _points[_points.size() - 1] = newEnd;
    else if (_points.length() > 2 && getDistance(_points[0], _points[_points.size() - 1]) < 25)
        _points.last() = QPoint(_points[0]);
}

QList<Shape *> Polygon::LiangBarskyClip(const Rectangle *clipper) const {
    QList<Shape *> clippedShapes;

    for (int i = 0; i < _points.size() - 1; i++) {
        Line line(_points[i], _color, _width);
        line.resize(_points[i + 1]);
        QList<Shape *> clippedLine = line.LiangBarskyClip(clipper);
        for (auto &shape: clippedLine)
            clippedShapes.append(shape);
    }

    return clippedShapes;
}

bool Polygon::isEnd() {
    return _snapped == _points.size() - 1 || _snapped == 0;
}

void Polygon::addPoint(QPoint point) {
    _points.append(point);
    _snapped = static_cast<int>(_points.size()) - 1;
}

void Polygon::close() {
    if (_points.length() > 2 && _points.first() == _points.last())
        _closed = true;
}

void Polygon::serialise(QXmlStreamWriter &writer) {
    writer.writeStartElement("polygon");
    QString pointsString;
    for (auto point: _points)
        pointsString += QString::number(point.x()) + "," + QString::number(point.y()) + ";";
    writer.writeAttribute("_points", pointsString);
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

Polygon *Polygon::deserialise(QXmlStreamReader &reader) {
    QList<QPoint> points;
    QColor color = Qt::black;
    int width = 0;
    bool closed = false;
    QColor fillingColor = Qt::white;
    bool fillingWithImage = false;
    QImage fillingImage = QImage();

    QXmlStreamAttributes attributes = reader.attributes();
    if (attributes.hasAttribute("_points")) {
        QString pointsString = attributes.value("_points").toString();
        QStringList pointsList = pointsString.split(";");
        for (int i = 0; i < pointsList.size() - 1; i++) {
            QStringList pointString = pointsList[i].split(",");
            points.append(QPoint(pointString[0].toInt(), pointString[1].toInt()));
        }
    }
    if (attributes.hasAttribute("_color")) {
        QStringList colorList = attributes.value("_color").toString().split(",");
        color.setRgb(colorList[0].toInt(), colorList[1].toInt(), colorList[2].toInt());
    }
    if (attributes.hasAttribute("_width"))
        width = attributes.value("_width").toInt();
    if (points[0] == points[points.size() - 1])
        closed = true;
    if (attributes.hasAttribute("_fillingColor")) {
        QStringList colorList = attributes.value("_fillingColor").toString().split(",");
        fillingColor.setRgb(colorList[0].toInt(), colorList[1].toInt(), colorList[2].toInt());
    }
    if (attributes.hasAttribute("_fillingWithImage"))
        fillingWithImage = attributes.value("_fillingWithImage").toInt();
    if (attributes.hasAttribute("_fillingImage")) {
        QImage fillingImage;
        QByteArray byteArray = QByteArray::fromBase64(attributes.value("_fillingImage").toString().toLatin1());
        fillingImage.loadFromData(byteArray, "PNG");
    }

    auto *polygon = new Polygon(points[0], color, width);
    polygon->_points = points;
    polygon->_closed = closed;
    polygon->_fillingColor = fillingColor;
    polygon->_fillingWithImage = fillingWithImage;
    polygon->_fillingImage = fillingImage;

    return polygon;
}
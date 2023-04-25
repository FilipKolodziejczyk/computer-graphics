#ifndef POLYGON_H
#define POLYGON_H

#include "shape.h"
#include "line.h"

class Polygon: public Shape
{
public:
    Polygon(QPoint start, QColor color, int width);
    int snap(const QPoint &point) override;
    void draw(QPainter &painter, bool antyaliasing) override;
    void move(QPoint newEnd) override;
    void resize(QPoint newEnd) override;
    bool isEnd();
    void addPoint(QPoint point);
    void close();
    bool isClosed() const { return closed; }
    void serialise(QXmlStreamWriter &writer) override;
    static Polygon *deserialise(QXmlStreamReader &reader);


private:
    QList<QPoint> points;
    int snapped = 1;
    bool closed = false;
};

#endif // POLYGON_H
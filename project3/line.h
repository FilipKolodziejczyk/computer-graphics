#ifndef LINE_H
#define LINE_H

#include "shape.h"

class Rectangle;

class Line : public Shape {
public:
    Line(QPoint start, QColor color, int width);

    int snap(const QPoint &point) override;

    void draw(QPainter &painter, bool antyaliasing) override;

    void move(QPoint newEnd) override;

    void resize(QPoint newEnd) override;

    QList<Shape *> LiangBarskyClip(const Rectangle *clipper) const override;

    void serialise(QXmlStreamWriter &writer) override;

    static Line *deserialise(QXmlStreamReader &reader);

private:
    void drawStandard(QPainter &painter);

    void drawAntyaliased(QPainter &painter);

    static bool Clip(double denom, double numer, double &tE, double &tL);

    QPoint _start;
    QPoint _end;
};

#endif // LINE_H
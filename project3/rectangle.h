#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"
#include "line.h"

class Rectangle : public Shape {
public:
    Rectangle(QPoint start, QColor color, int width);

    int snap(const QPoint &point) override;

    void draw(QPainter &painter, bool antyaliasing) override;

    void move(QPoint newEnd) override;

    void resize(QPoint newEnd) override;

    QList<Shape *> LiangBarskyClip(const Rectangle *clipper) const override;

    void serialise(QXmlStreamWriter &writer) override;

    static Rectangle *deserialise(QXmlStreamReader &reader);

    int left() const;

    int right() const;

    int top() const;

    int bottom() const;

private:
    QPoint corners[4];
    int snapPointIndex;
};

#endif // RECTANGLE_H
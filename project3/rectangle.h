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

    [[nodiscard]] int left() const;

    [[nodiscard]] int right() const;

    [[nodiscard]] int top() const;

    [[nodiscard]] int bottom() const;

private:
    QPoint _corners[4];
    int _snapPointIndex;
};

#endif // RECTANGLE_H
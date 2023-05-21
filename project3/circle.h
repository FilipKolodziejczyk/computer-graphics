#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"

class Circle : public Shape {
public:
    Circle(QPoint center, QColor color, int width);

    int snap(const QPoint &point) override;

    void draw(QPainter &painter, bool antyaliasing) override;

    void move(QPoint newCenter) override;

    void resize(QPoint newPerimeterPoint) override;

    QList<Shape *> LiangBarskyClip(const Rectangle *clipper) const override;

    void serialise(QXmlStreamWriter &writer) override;

    static Circle *deserialise(QXmlStreamReader &reader);


private:
    QPoint _center;
    QPoint _snapped;
    int _radius;

};

#endif // CIRCLE_H
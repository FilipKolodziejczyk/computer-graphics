#ifndef SHAPE_H
#define SHAPE_H

#include <QColor>
#include <QPainter>
#include <QPen>
#include <QXmlStreamWriter>
#include <QDebug>

class Rectangle;

class Shape {

public:
    Shape(QColor color, int width) : _color(color), _width(width) {}

    virtual int snap(const QPoint &point) = 0;

    virtual void draw(QPainter &painter, bool antyaliasing) = 0;

    virtual void move(QPoint) = 0;

    virtual void resize(QPoint) = 0;

    virtual QList<Shape *> LiangBarskyClip(const Rectangle *clipper) const = 0;

    [[maybe_unused]] void remove() { _removed = true; }

    void format(QColor color, int width) {
        this->_color = color;
        this->_width = width;
    }

    virtual void serialise(QXmlStreamWriter &) = 0;

protected:
    static double getDistance(QPoint a, QPoint b) {
        return sqrt((a.x() - b.x()) * (a.x() - b.x()) + (a.y() - b.y()) * (a.y() - b.y()));
    }

    void brush(QPainter &painter, int x, int y) {
        painter.setPen(QPen(_color, 1));
        for (int i = -_width + 1; i < _width; i++)
            for (int j = -_width + 1; j < _width; j++)
                if (i * i + j * j <= _width * _width)
                    painter.drawPoint(x + i, y + j);
    }

    QColor _color;
    [[maybe_unused]] int _width;
    [[maybe_unused]] bool _removed = false;

};

#endif // SHAPE_H
#ifndef SHAPE_H
#define SHAPE_H

#include <QColor>
#include <QPainter>
#include <QPen>
#include <QXmlStreamWriter>

class Shape {

public:
    Shape(QColor color, int width) : color(color), width(width) {}

    virtual int snap(const QPoint &point) { return 1000000; }

    virtual void draw(QPainter &painter, bool antyaliasing) { }

    virtual void move(QPoint) { }

    virtual void resize(QPoint) { }

    void remove() { removed = true; }

    void format (QColor color, int width) {
        this->color = color;
        this->width = width;
    }

    virtual void serialise(QXmlStreamWriter &) {}

protected:
    static int getDistance(QPoint a, QPoint b) {
        return sqrt((a.x() - b.x()) * (a.x() - b.x()) + (a.y() - b.y()) * (a.y() - b.y()));
    }

    void brush(QPainter &painter, int x, int y) {
        painter.setPen(QPen(color, 1));
        for (int i = -width + 1; i < width; i++)
            for (int j = -width + 1; j < width; j++)
                if (i * i + j * j <= width * width)
                    painter.drawPoint(x + i, y + j);
    }

    QColor color;
    int width;
    bool removed = false;

};

#endif // SHAPE_H
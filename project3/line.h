#ifndef LINE_H
#define LINE_H

#include "shape.h"

class Line : public Shape {
public:
    Line(QPoint start, QColor color, int width);

    int snap(const QPoint &point) override;

    void draw(QPainter &painter, bool antyaliasing) override;

    void move(QPoint newEnd) override;

    void resize(QPoint newEnd) override;

    void serialise(QXmlStreamWriter &writer) override;

    static Line *deserialise(QXmlStreamReader &reader);

private:
    void drawStandard(QPainter &painter);

    void drawAntyaliased(QPainter &painter);

    QPoint start;
    QPoint end;
};

#endif // LINE_H
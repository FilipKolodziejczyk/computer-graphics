#ifndef FILLEDSHAPE_H
#define FILLEDSHAPE_H

#include "shape.h"

class FilledShape : public Shape {
public:
    FilledShape(QColor color, int width) : Shape(color, width), _fillingWithImage(false) {}

    virtual void fill(QPainter &painter) const =  0;

    void setFillingColor(QColor color) { _fillingColor = color; _fillingWithImage = false; }

    void setFillingImage(QImage image) { _fillingImage = image; _fillingWithImage = true; }

    [[nodiscard]] bool getIfFillingWithImage() const { return _fillingWithImage; }

protected:
    QColor _fillingColor = Qt::white;
    QImage _fillingImage;
    bool _fillingWithImage;
};

#endif //FILLEDSHAPE_H
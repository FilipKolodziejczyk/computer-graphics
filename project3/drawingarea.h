#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include "shape.h"

#include <QWidget>
#include <QImage>

enum Tools {
    line, circle, rectangle, polygon, mov, res, rem, format, clip, fill
};

constexpr std::initializer_list<std::pair<Tools, const char *>> tools = {{line,      "Line"},
                                                                         {circle,    "Circle"},
                                                                         {rectangle, "Rectangle"},
                                                                         {polygon,   "Polygon"},
                                                                         {mov,       "Move"},
                                                                         {res,       "Resize"},
                                                                         {rem,       "Remove"},
                                                                         {format,    "Format"},
                                                                         {clip,      "Clip"},
                                                                         {fill,      "Fill"}};

class DrawingArea : public QWidget {
Q_OBJECT

public:
    explicit DrawingArea(QWidget *parent = nullptr);

    bool open(const QString &fileName);

    bool save(const QString &fileName);

    void setPenColor(const QColor &newColor) { _penColor = newColor; }

    void setPenWidth(int newWidth) { _penWidth = newWidth; }

    void setFillingColor(const QColor &newColor) { _fillingColor = newColor; }

    void setFillingImage(const QImage &newImage) { _fillingImage = newImage; }

    [[nodiscard]] QColor getPenColor() const { return _penColor; }

    [[nodiscard]] int getPenWidth() const { return _penWidth; }

    [[nodiscard]] QColor getFillingColor() const { return _fillingColor; }

    [[nodiscard]] QImage getFillingImage() const { return _fillingImage; }

    [[nodiscard]] bool getIfFillingWithImage() const { return _fillingWithImage; }

    [[nodiscard]] Tools getTool() const { return _tool; }

    [[nodiscard]] bool getAntyaliasing() const { return _antyaliasing; }

public slots:

    void clear();

    void setTool();

    void toggleAntialysing();

    void toggleFillingType() { _fillingWithImage = !_fillingWithImage; }

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void paintEvent(QPaintEvent *event) override;

    [[nodiscard]] Shape *snapClosestShape(const QPoint &point) const;

    QImage _image;
    QImage _backupImage;
    QList<Shape *> _shapes;
    Shape *_currentShape{};
    QColor _penColor;
    int _penWidth;
    QColor _fillingColor;
    QImage _fillingImage;
    bool _fillingWithImage{};
    Tools _tool;
    bool _antyaliasing;
    Shape *_clippedShape{};
    bool _editing = false;
};

#endif // DRAWINGAREA_H
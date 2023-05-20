#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include "shape.h"

#include <QWidget>
#include <QImage>

enum Tools {
    line, circle, polygon, mov, res, rem, format
};

constexpr std::initializer_list<std::pair<Tools, const char *>> tools = {{line,    "Line"},
                                                                         {circle,  "Circle"},
                                                                         {polygon, "Polygon"},
                                                                         {mov,     "Move"},
                                                                         {res,     "Resize"},
                                                                         {rem,     "Remove"},
                                                                         {format,  "Format"}};

class DrawingArea : public QWidget {
Q_OBJECT

public:
    explicit DrawingArea(QWidget *parent = nullptr);

    bool open(const QString &fileName);

    bool save(const QString &fileName);

    void setPenColor(const QColor &newColor) { penColor = newColor; }

    void setPenWidth(int newWidth) { penWidth = newWidth; }

    [[nodiscard]] QColor getPenColor() const { return penColor; }

    [[nodiscard]] int getPenWidth() const { return penWidth; }

    [[nodiscard]] Tools getTool() const { return tool; }

    [[nodiscard]] bool getAntyaliasing() const { return antyaliasing; }

public slots:

    void clear();

    void setTool();

    void toggleAntialysing();

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void paintEvent(QPaintEvent *event) override;

    Shape *snapClosestShape(const QPoint &point) const;

    QImage image;
    QImage backupImage;
    QList<Shape *> shapes;
    Shape *currentShape;
    QColor penColor;
    int penWidth;
    Tools tool;
    bool antyaliasing;
};

#endif // DRAWINGAREA_H
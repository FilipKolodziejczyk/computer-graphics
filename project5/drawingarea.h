#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <QWidget>
#include <QImage>

#include "cylinder.h"

class DrawingArea : public QWidget {
Q_OBJECT

public:
    explicit DrawingArea(QWidget *parent = nullptr);

    void loadTexture(const QString &fileName);

public slots:
    void toggleTexture() { _textureOn = !_textureOn; }

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseScrollEvent(QWheelEvent *event);

private:
    void paintEvent(QPaintEvent *event) override;

    QImage _image;
    QImage _backupImage;
    QImage _texture;

    bool _textureOn;
};

#endif // DRAWINGAREA_H
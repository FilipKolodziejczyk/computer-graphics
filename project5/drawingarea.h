#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <QWidget>
#include <QImage>
#include <QGraphicsSceneWheelEvent>
#include "Models3D/Camera.h"
#include "Models3D/Cylinder.h"

class DrawingArea : public QWidget {
Q_OBJECT

public:
    explicit DrawingArea(QWidget *parent = nullptr);

    void loadTexture(const QString &fileName);

public slots:

    void toggleTexture() {
        _textureOn = !_textureOn;
        update();
    }

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

private:
    void paintEvent(QPaintEvent *event) override;

    QImage _image;
    QImage _backupImage;
    QImage _texture;
    Camera _camera = Camera();
    Cylinder _cylinder = Cylinder(100, 200, 40, {100, -100, 0});

    bool _editing = false;
    QPoint _lastPos;
    bool _textureOn;
    Transformation cameraPosition = Transformation();
    Transformation targetPosition = Transformation();
    double fov = 90;
};

#endif // DRAWINGAREA_H
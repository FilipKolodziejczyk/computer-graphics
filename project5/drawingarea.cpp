#include "drawingarea.h"

#include <QPainter>
#include <QMouseEvent>

DrawingArea::DrawingArea(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StaticContents);
    resize(1000, 1000);
    QImage initImage(size(), QImage::Format_ARGB32);
    initImage.fill(qRgb(255, 255, 255));
    _image = initImage;
    _textureOn = false;
}

void DrawingArea::paintEvent(QPaintEvent *event) {
    QPainter painter(&_image);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    _cylinder.draw(painter);

    QPainter areaPainter(this);
    QRect dirtyRect = event->rect();
    areaPainter.drawImage(dirtyRect, _image, dirtyRect);
}

void DrawingArea::mousePressEvent(QMouseEvent *event) {
    if (event->button() != Qt::LeftButton)
        return;
}

void DrawingArea::mouseMoveEvent(QMouseEvent *event) {
    if ((event->buttons() & Qt::LeftButton) == 0)
        return;
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() != Qt::LeftButton)
        return;
}

void DrawingArea::mouseScrollEvent(QWheelEvent *event) {
    if (event->angleDelta().y() > 0) {}
        // _cylinder.zoomIn();
    else {}
        // _cylinder.zoomOut();
    update();
}

void DrawingArea::loadTexture(const QString &fileName) {
    _texture.load(fileName);
    _textureOn = true;
}
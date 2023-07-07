#include "drawingarea.h"

#include <QPainter>
#include <QMouseEvent>

Vector3 up = {0, 1, 0};

DrawingArea::DrawingArea(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StaticContents);
    resize(1000, 1000);
    QImage initImage(size(), QImage::Format_ARGB32);
    initImage.fill(qRgb(255, 255, 255));
    _image = initImage;
    _textureOn = false;
    cameraPosition._position = {-150, 50, -400};
    targetPosition._position = {-150, 50, 0};
    _camera = Camera(cameraPosition, targetPosition, up, Vector2(width(), height()), fov);
}

void DrawingArea::paintEvent(QPaintEvent *event) {
    _image.fill(qRgb(255, 255, 255));
    QPainter painter(&_image);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    auto projection = _camera.project(_cylinder);
    auto projectedVertices = projection.screenPoints;

    for (auto &triangle: _cylinder.triangles) {
        auto a = projectedVertices[triangle.a] - projectedVertices[triangle.b];
        auto b = projectedVertices[triangle.b] - projectedVertices[triangle.c];

        auto cross = Vector3(a.x, a.y, a.z).cross(Vector3(b.x, b.y, b.z));
        if (cross.z <= 0) {
            continue;
        }

        auto p1 = projectedVertices[triangle.a];
        auto p2 = projectedVertices[triangle.b];
        auto p3 = projectedVertices[triangle.c];

        if (_textureOn && !_editing) {
            auto triangleTextureCoordinates = {
                    _cylinder.textureCoordinates[triangle.a],
                    _cylinder.textureCoordinates[triangle.b],
                    _cylinder.textureCoordinates[triangle.c]
            };
            auto trimesh = MeshTriangle(p1, p2, p3, _texture, triangleTextureCoordinates);
            trimesh.draw(painter);
        } else {
            auto trimesh = MeshTriangle(p1, p2, p3);
            trimesh.draw(painter);
        }
    }

    QPainter areaPainter(this);
    QRect dirtyRect = event->rect();
    areaPainter.drawImage(dirtyRect, _image, dirtyRect);
}

void DrawingArea::mousePressEvent(QMouseEvent *event) {
    _editing = true;
    _lastPos = event->pos();
}

void DrawingArea::mouseMoveEvent(QMouseEvent *event) {
    if (!_editing)
        return;

    auto dx = event->position().x() - _lastPos.x();
    auto dy = event->position().y() - _lastPos.y();
    _lastPos = event->pos();

    if (event->buttons() & Qt::LeftButton) {
        auto angleX = dy * 0.01;
        auto angleY = dx * 0.01;

        _cylinder.rotate(angleX, angleY, 0);
        update();
    } else if (event->buttons() & Qt::RightButton) {
        cameraPosition._position.x += dx;
        cameraPosition._position.y += dy;

        _camera = Camera(cameraPosition, targetPosition, up, Vector2(width(), height()), fov);
        update();
    }
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *event) {
    _editing = false;
    update();
}

void DrawingArea::wheelEvent(QWheelEvent *event) {
    auto numPixels = event->pixelDelta();
    auto numDegrees = event->angleDelta() / 8;
    QPoint numSteps;

    if (!numPixels.isNull()) {
        numSteps = numPixels / 15;
    } else if (!numDegrees.isNull()) {
        numSteps = numDegrees / 15;
    }

    cameraPosition._position.z += numSteps.y();
    cameraPosition._position.z = std::max(cameraPosition._position.z, -5000.0);
    cameraPosition._position.z = std::min(cameraPosition._position.z, - _cylinder.radius * 2);
    _camera = Camera(cameraPosition, targetPosition, up, Vector2(width(), height()), fov);
    update();
}


void DrawingArea::loadTexture(const QString &fileName) {
    _texture.load(fileName);
    _textureOn = true;
    _cylinder.texture = _texture;
}
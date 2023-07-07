#include "Camera.h"

Projection::Projection(QList<Vector4> worldPoints, QList<Vector4> screenPoints) : worldPoints(worldPoints), screenPoints(screenPoints) {}

Camera::Camera(Transformation transformation, Transformation targetTransformation, Vector3 up, Vector2 viewportSize, double fov) : transformation(transformation),
    targetTransformation(targetTransformation),
    up(up),
    viewportSize(viewportSize),
    fov(fov),
    cx(viewportSize.x / 2),
    cy(viewportSize.y / 2) {}

Matrix Camera::getViewMatrix() {
    auto z = (transformation._position - targetTransformation._position).normalized();
    auto x = up.cross(z).normalized();
    auto y = z.cross(x).normalized();

    return Matrix({
                          {x.x, x.y, x.z, x.dot(transformation._position)},
                          {y.x, y.y, y.z, y.dot(transformation._position)},
                          {z.x, z.y, z.z, z.dot(transformation._position)},
                          {0, 0, 0, 1}
                  });
}

Matrix Camera::getProjectionMatrix() const {
    auto cot = 1 / tan((fov / 2) * M_PI / 180);
    return Matrix({
                          {-(viewportSize.x / 2) * cot, 0, viewportSize.x / 2, 0},
                          {0, (viewportSize.x / 2) * cot, viewportSize.y / 2, 0},
                          {0, 0, 0, 1},
                          {0, 0, 1, 0}
                  });
}

Vector4 Camera::projectPoint(const Vector4 &worldPoint) const {
    return Vector4(
            cx + distanceToViewport * (worldPoint.x) / (distanceToViewport - worldPoint.z),
            cy + distanceToViewport * (worldPoint.y) / (distanceToViewport - worldPoint.z),
            0,
            0
    );
}

Projection Camera::project(Mesh mesh) {
    QList<Vector4> worldPoints;
    QList<Vector4> viewPoints;
    QList<Vector4> screenPoints;

    for (auto &vertex : mesh.vertices) {
        worldPoints.append(mesh.transformation.transform(vertex));
    }

    for (auto &worldPoint : worldPoints) {
        viewPoints.append(getViewMatrix().dot(Matrix(worldPoint)).toVector4());
    }

    for (auto &viewPoint : viewPoints) {
        auto point = getProjectionMatrix().dot(Matrix(viewPoint)).toVector4();
        screenPoints.append(point / point.w);
    }

    return Projection(worldPoints, screenPoints);
}
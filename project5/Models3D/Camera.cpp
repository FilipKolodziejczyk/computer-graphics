#include "Camera.h"

#include <qdebug.h>

Projection::Projection(QList<Vector> worldPoints, QList<Vector> screenPoints) : worldPoints(worldPoints),
                                                                                screenPoints(screenPoints) {}

Camera::Camera(Transformation transformation, Transformation targetTransformation, Vector viewportSize, Vector up,
               double fov, double minZ) : transformation(transformation), targetTransformation(targetTransformation),
                                          up(up), viewportSize(viewportSize), fov(fov), cx(viewportSize[0] / 2),
                                          cy(viewportSize[1] / 2), minZ(minZ) {}

Matrix Camera::getViewMatrix() {
    auto z = (transformation.dst - targetTransformation.dst).normalized();
    auto x = up.cross(z).normalized();
    auto y = z.cross(x).normalized();

    return Matrix({{x[0], x[1], x[2], x * transformation.dst},
                   {y[0], y[1], y[2], y * transformation.dst},
                   {z[0], z[1], z[2], z * transformation.dst},
                   {0,    0,    0,    1}});
}

Matrix Camera::getProjectionMatrix() const {
    auto cot = 1 / tan((fov / 2) * M_PI / 180);
    return Matrix({{-cx * cot, 0,        viewportSize[0] / 2, 0},
                   {0,         cx * cot, viewportSize[1] / 2, 0},
                   {0,         0,        0,                   1},
                   {0,         0,        1,                   0}});
}

void Camera::shift(double dx, double dy, double dz) {
    auto shiftMatrix = TranslationMatrix(dx, dy, dz);
    auto affinePos = Vector(transformation.dst[0], transformation.dst[1], transformation.dst[2], 1);
    affinePos = (shiftMatrix * Matrix(affinePos)).toVector();
    transformation.dst = Vector(affinePos[0], affinePos[1], std::min(affinePos[2], minZ));
}

Projection Camera::project(Mesh mesh) {
    QList<Vector> worldPoints;
    QList<Vector> viewPoints;
    QList<Vector> screenPoints;

    for (auto &vertex: mesh.vertices) {
        worldPoints.append(mesh.transformation.transform(vertex));
    }

    for (auto &worldPoint: worldPoints) {
        viewPoints.append((getViewMatrix() * Matrix(worldPoint)).toVector());
    }

    for (auto &viewPoint: viewPoints) {
        auto point = (getProjectionMatrix() * Matrix(viewPoint)).toVector();
        screenPoints.append(point / point[3]);
    }

    return Projection(worldPoints, screenPoints);
}
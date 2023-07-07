#ifndef PROJECT5_CAMERA_H
#define PROJECT5_CAMERA_H

#include <QList>
#include "../Math/Vector4.h"
#include "../Transformations/Transformation.h"
#include "Mesh.h"

struct Projection {
    QList<Vector4> worldPoints;
    QList<Vector4> screenPoints;

    Projection(QList<Vector4> worldPoints, QList<Vector4> screenPoints);
};


class Camera {
public:
    Camera() = default;

    Camera(Transformation transformation, Transformation targetTransformation, Vector3 up, Vector2 viewportSize, double fov);

    Matrix getViewMatrix();

    Matrix getProjectionMatrix() const;

    Vector4 projectPoint(const Vector4 &worldPoint) const;

    Projection project(Mesh mesh);

private:
    Transformation transformation;
    Transformation targetTransformation;
    Vector3 up;
    Vector2 viewportSize;
    double fov;
    double distanceToViewport = 1000;
    double cx = 0;
    double cy = 0;
};


#endif //PROJECT5_CAMERA_H
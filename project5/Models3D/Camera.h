#ifndef PROJECT5_CAMERA_H
#define PROJECT5_CAMERA_H

#include <QList>
#include "../Math/Vector.h"
#include "../Transformations/Transformation.h"
#include "Mesh.h"

struct Projection {
    QList<Vector> worldPoints;
    QList<Vector> screenPoints;

    Projection(QList<Vector> worldPoints, QList<Vector> screenPoints);
};


class Camera {
public:
    Camera() = default;

    Camera(Transformation transformation, Transformation targetTransformation, Vector up, Vector viewportSize,
           double fov, double minZ);

    Matrix getViewMatrix();

    Matrix getProjectionMatrix() const;

    void shift(double dx, double dy, double dz);

    Projection project(Mesh mesh);

private:
    Transformation transformation;
    Transformation targetTransformation;
    Vector up;
    Vector viewportSize;
    double fov;
    double minZ;
    double cx = 0;
    double cy = 0;
};


#endif //PROJECT5_CAMERA_H
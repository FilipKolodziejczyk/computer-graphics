#ifndef MESH_H
#define MESH_H


#include <QList>
#include <QImage>
#include <QPainter>
#include "../Math/Vector.h"
#include "../Transformations/Transformation.h"

Vector vecToOffset(const Vector &vector);

struct MeshTriangle {
    Vector &v1, &v2, &v3;
    QColor color = Qt::black;
    QImage texture = QImage();
    QList<Vector> textureCoordinates = QList<Vector>();

    MeshTriangle(Vector &v1, Vector &v2, Vector &v3);

    MeshTriangle(Vector &v1, Vector &v2, Vector &v3, QImage texture, QList<Vector> textureCoordinates);

    void draw(QPainter &painter);
};

struct TriangleVertices {
    int a = 0;
    int b = 0;
    int c = 0;
};

class Mesh {
public:
    QImage texture;

    void rotate(double angleX, double angleY, double angleZ);

    QList<Vector> vertices;
    QList<TriangleVertices> triangles;
    QList<Vector> textureCoordinates;
    Transformation transformation = Transformation();
};


#endif //MESH_H
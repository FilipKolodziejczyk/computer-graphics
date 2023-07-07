#ifndef MESH_H
#define MESH_H


#include <QList>
#include <QImage>
#include <QPainter>
#include "../Math/Vector4.h"
#include "../Transformations/Transformation.h"

Vector2 vec4ToOffset(const Vector4 &vector);

struct MeshTriangle {
    Vector4 &v1, &v2, &v3;
    QColor color = Qt::black;
    QImage texture = QImage();
    QList<Vector2> textureCoordinates = QList<Vector2>();

    MeshTriangle(Vector4 &v1, Vector4 &v2, Vector4 &v3);

    MeshTriangle(Vector4 &v1, Vector4 &v2, Vector4 &v3, QImage texture, QList<Vector2> textureCoordinates);

    void draw(QPainter &painter);
};

Vector3 toBarycentric(Vector4 p, Vector4 a, Vector4 b, Vector4 c);

struct TriangleVertices {
    int a = 0;
    int b = 0;
    int c = 0;
};

class Mesh {
public:
    QImage texture;

    void rotate(double angleX, double angleY, double angleZ);
    QList<Vector4> vertices;
    QList<TriangleVertices> triangles;
    QList<Vector2> textureCoordinates;
    Transformation transformation = Transformation();
};


#endif //MESH_H
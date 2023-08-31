#include "Mesh.h"

#include <utility>

Vector vecToOffset(const Vector &vector) {
    return Vector(vector[0], vector[1]);
}

MeshTriangle::MeshTriangle(Vector &v1, Vector &v2, Vector &v3) : v1(v1), v2(v2), v3(v3) {}

MeshTriangle::MeshTriangle(Vector &v1, Vector &v2, Vector &v3, QImage texture, QList<Vector> textureCoordinates) : v1(
        v1), v2(v2), v3(v3), texture(std::move(texture)), textureCoordinates(std::move(textureCoordinates)) {}

Vector toBarycentric(Vector p, Vector a, Vector b, Vector c) {
    auto v0 = b - a;
    v0 = {v0[0], v0[1], v0[2], 0};
    auto v1 = c - a;
    v1 = {v1[0], v1[1], v1[2], 0};
    auto v2 = p - a;
    v2 = {v2[0], v2[1], v2[2], 0};
    auto d00 = v0 * v0;
    auto d01 = v0 * v1;
    auto d11 = v1 * v1;
    auto d20 = v2 * v0;
    auto d21 = v2 * v1;
    auto denom = d00 * d11 - d01 * d01;
    auto v = (d11 * d20 - d01 * d21) / denom;
    auto w = (d00 * d21 - d01 * d20) / denom;
    auto u = 1.0 - v - w;
    return {u, v, w};
}

void MeshTriangle::draw(QPainter &painter) {
    painter.setPen(color);
    auto p1 = vecToOffset(v1);
    auto p2 = vecToOffset(v2);
    auto p3 = vecToOffset(v3);
    painter.drawLine(p1[0], p1[1], p2[0], p2[1]);
    painter.drawLine(p2[0], p2[1], p3[0], p3[1]);
    painter.drawLine(p3[0], p3[1], p1[0], p1[1]);

    if (texture.isNull())
        return;

    auto minX = std::min({p1[0], p2[0], p3[0]});
    auto minY = std::min({p1[1], p2[1], p3[1]});
    auto maxX = std::max({p1[0], p2[0], p3[0]});
    auto maxY = std::max({p1[1], p2[1], p3[1]});

    for (auto x = qRound(minX); x <= maxX; x++) {
        for (auto y = qRound(minY); y <= maxY; y++) {
            auto p = Vector(x, y, 0, 0);
            auto barycentric = toBarycentric(p, v1, v2, v3);
            if (barycentric[0] < 0 || barycentric[1] < 0 || barycentric[2] < 0)
                continue;
            auto xTexture = barycentric[0] * textureCoordinates[0][0] + barycentric[1] * textureCoordinates[1][0] +
                            barycentric[2] * textureCoordinates[2][0];
            auto yTexture = barycentric[0] * textureCoordinates[0][1] + barycentric[1] * textureCoordinates[1][1] +
                            barycentric[2] * textureCoordinates[2][1];
            auto color = texture.pixelColor(qRound(xTexture * (texture.width() - 1)),
                                            qRound(yTexture * (texture.height() - 1)));
            painter.setPen(color);
            painter.drawPoint(x, y);
        }
    }
}

void Mesh::rotate(double angleX, double angleY, double angleZ) {
    auto rotationMatrix = RotationMatrix(angleX, angleY, angleZ);
    for (auto &vertex: vertices) {
        vertex = (rotationMatrix * Matrix(vertex)).toVector();
    }
}
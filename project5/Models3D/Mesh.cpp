#include "Mesh.h"

Vector2 vec4ToOffset(const Vector4 &vector) {
    return Vector2(vector.x, vector.y);
}

MeshTriangle::MeshTriangle(Vector4 &v1, Vector4 &v2, Vector4 &v3) : v1(v1), v2(v2), v3(v3) {}

MeshTriangle::MeshTriangle(Vector4 &v1, Vector4 &v2, Vector4 &v3, QImage texture, QList<Vector2> textureCoordinates) : v1(v1), v2(v2),
                                                                                                        v3(v3),
                                                                                                        texture(texture),
                                                                                                        textureCoordinates(
                                                                                                                std::move(textureCoordinates)) {}

Vector3 toBarycentric(Vector4 p, Vector4 a, Vector4 b, Vector4 c) {
    auto v0 = b - a;
    auto v1 = c - a;
    auto v2 = p - a;
    auto d00 = v0.dot(v0);
    auto d01 = v0.dot(v1);
    auto d11 = v1.dot(v1);
    auto d20 = v2.dot(v0);
    auto d21 = v2.dot(v1);
    auto denom = d00 * d11 - d01 * d01;
    auto v = (d11 * d20 - d01 * d21) / denom;
    auto w = (d00 * d21 - d01 * d20) / denom;
    auto u = 1.0f - v - w;
    return {u, v, w};
}

void MeshTriangle::draw(QPainter &painter) {
    painter.setPen(color);
    auto p1 = vec4ToOffset(v1);
    auto p2 = vec4ToOffset(v2);
    auto p3 = vec4ToOffset(v3);
    painter.drawLine(p1.x, p1.y, p2.x, p2.y);
    painter.drawLine(p2.x, p2.y, p3.x, p3.y);
    painter.drawLine(p3.x, p3.y, p1.x, p1.y);

    if (texture.isNull())
        return;

    auto minX = std::min({p1.x, p2.x, p3.x});
    auto minY = std::min({p1.y, p2.y, p3.y});
    auto maxX = std::max({p1.x, p2.x, p3.x});
    auto maxY = std::max({p1.y, p2.y, p3.y});

    for (auto x = qRound(minX); x <= maxX; x++) {
        for (auto y = qRound(minY); y <= maxY; y++) {
            auto p = Vector4(x, y, 0, 0);
            auto barycentric = toBarycentric(p, v1, v2, v3);
            if (barycentric.x < 0 || barycentric.y < 0 || barycentric.z < 0)
                continue;
            auto xTexture = barycentric.x * textureCoordinates[0].x + barycentric.y * textureCoordinates[1].x + barycentric.z * textureCoordinates[2].x;
            auto yTexture = barycentric.x * textureCoordinates[0].y + barycentric.y * textureCoordinates[1].y + barycentric.z * textureCoordinates[2].y;
            auto color = texture.pixelColor(qRound(xTexture * (texture.width() - 1)), qRound(yTexture * (texture.height() - 1)));
            painter.setPen(color);
            painter.drawPoint(x, y);
        }
    }
}

void Mesh::rotate(double angleX, double angleY, double angleZ) {
    auto rotationMatrix = RotationMatrix(angleX, angleY, angleZ);
    for (auto &vertex : vertices) {
        vertex = rotationMatrix.dot(Matrix(vertex)).toVector4();
    }
}
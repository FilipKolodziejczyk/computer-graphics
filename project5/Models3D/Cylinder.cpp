#include "Cylinder.h"

Cylinder::Cylinder(double radius, double height, int sides, Vector3 positionOnScene) : radius(radius), height(height) {
    this->vertices = QList<Vector4>(4 * sides + 2);
    this->triangles = QList<TriangleVertices>(4 * sides);
    this->triangles.reserve(4 * sides);
    this->textureCoordinates = QList<Vector2>(4 * sides + 2);

    // Top base
    this->vertices[0] = Vector4(0, height, 0, 1); // Base center vertex
    for (int i = 0; i < sides; i++) {
        double angle = 2 * M_PI * i / sides;
        this->vertices[i + 1] = Vector4(radius * cos(angle), height, radius * sin(angle), 1); // Side vase vertices
    }
    auto &centerTop = this->vertices[0];
    for (int i = 0; i < sides - 1; i++) {

        auto &v2 = this->vertices[i + 2];
        auto &v3 = this->vertices[i + 1];
        // this->triangles.append(MeshTriangle(centerTop, v2, v3));
    }
    // this->triangles.append(MeshTriangle(centerTop, this->vertices[1], this->vertices[sides]));

    // Side
    for (int i = sides; i < 2 * sides; i++) {
        double angle = 2 * M_PI * (i - sides) / sides;
        this->vertices[i + 1] = Vector4(radius * cos(angle), height, radius * sin(angle), 1);
    }
    for (int i = sides; i < 2 * sides - 1; i++) {
        auto &v1 = this->vertices[i + 1];
        auto &v2 = this->vertices[i + 2];
        auto &v3 = this->vertices[i + 1 + sides];
        // this->triangles.append(MeshTriangle(v1, v2, v3));
    }
    // this->triangles.append(MeshTriangle(this->vertices[2 * sides], this->vertices[sides + 1], this->vertices[3 * sides]));

    // Side cont.
    for (int i = 2 * sides; i < 3 * sides; i++) {
        double angle = 2 * M_PI * (i - 2 * sides) / sides;
        this->vertices[i + 1] = Vector4(radius * cos(angle), 0, radius * sin(angle), 1);
    }
    for (int i = 2 * sides; i < 3 * sides - 1; i++) {
        auto &v1 = this->vertices[i + 2];
        auto &v2 = this->vertices[i + 1];
        auto &v3 = this->vertices[i + 2 - sides];
        // this->triangles.append(MeshTriangle(v1, v2, v3));
    }
    // this->triangles.append(MeshTriangle(this->vertices[3 * sides], this->vertices[sides + 1], this->vertices[2 * sides + 1]));


    // Bottom base
    for (int i = 3 * sides; i < 4 * sides; i++) {
        double angle = 2 * M_PI * (i - 3 * sides) / sides;
        this->vertices[i + 1] = Vector4(radius * cos(angle), 0, radius * sin(angle), 1); // Side base vertices
    }
    this->vertices[4 * sides + 1] = Vector4(0, 0, 0, 1); // Base center vertex
    auto &centerBottom = this->vertices[4 * sides + 1];
    for (int i = 3 * sides; i < 4 * sides - 1; i++) {
        auto &v2 = this->vertices[i + 1];
        auto &v3 = this->vertices[i + 2];
        // this->triangles.append(MeshTriangle(centerBottom, v2, v3));
    }
    // this->triangles.append(MeshTriangle(centerBottom, this->vertices[4 * sides], this->vertices[3 * sides + 1]));

    // Top base triangles
    for (int i = 0; i < sides - 1; i++) {
        triangles[i] = {0, i + 2, i + 1};
    }
    triangles[sides - 1] = {0, 1, sides};

    // Bottom base triangles
    for (int i = 0; i < sides - 1; i++) {
        triangles[sides + i] = {4 * sides + 1, 3 * sides + i + 1, 3 * sides + i + 2};
    }
    triangles[2 * sides - 1] = {4 * sides + 1, 4 * sides, 3 * sides + 1};

    // Side triangles
    for (int i = 0; i < sides - 1; i++) {
        triangles[2 * sides + i] = {sides + i + 1, sides + i + 2, 2 * sides + i + 1};
        triangles[3 * sides + i] = {2 * sides + i + 2, 2 * sides + i + 1, sides + i + 2};
    }

    triangles[3 * sides - 1] = {2 * sides, sides + 1, 3 * sides};
    triangles[4 * sides - 1] = {3 * sides, sides + 1, 2 * sides + 1};

    textureCoordinates[0] = Vector2(0.25, 0.25);
    for (int i = 0; i < sides; i++) {
        textureCoordinates[i + 1] = Vector2(0.25 * (1 + cos(2 * M_PI * i / sides)), 0.25 * (1 + sin(2 * M_PI * i / sides)));
    }

    textureCoordinates[4 * sides + 1] = Vector2(0.75, 0.25);
    for (int i = 0; i < sides; i++) {
        textureCoordinates[3 * sides + i + 1] = Vector2(0.25 * (3 + cos(2 * M_PI * i / sides)), 0.25 * (1 + sin(2 * M_PI * i / sides)));
    }

    for (int i = 0; i < sides; i++) {
        textureCoordinates[sides + i + 1] = Vector2(i / (sides - 1), 1);
        textureCoordinates[2 * sides + i + 1] = Vector2(i / (sides - 1), 0.5);
    }

    this->transformation._position = positionOnScene;
}
#ifndef CYLINDER_H
#define CYLINDER_H

#include <cmath>
#include <QList>

struct Vertex {
    Vertex(double px, double py, double pz, double nx, double ny, double nz) : _px(px), _py(py), _pz(pz), _nx(nx),
                                                                               _ny(ny), _nz(nz) {}

    Vertex() = default;

    // [px, py, pz, 1]^T
    double _px, _py, _pz;

    // [nx, ny, nz, 0]^T
    double _nx, _ny, _nz;
};

struct Trimesh {
    Trimesh(Vertex V1, Vertex V2, Vertex V3) : V1(V1), V2(V2), V3(V3) {}
    Trimesh() = default;

    Vertex V1, V2, V3;
};


class Cylinder {
public:
    Cylinder(double radius, double height, int sides): _radius(radius), _height(height), _sides(sides) {
        _vertices.reserve(4 * _sides + 2);

        _vertices.append(topBaseCenterVertex());            // V_0
        for (int i = 1; i <= _sides; ++i)
            _vertices.append(topBaseSideVertex(i));         // V_i; i = 1, 2, ..., n

        _vertices.append(sideVertex(_sides + 1));       // V_(n + 1)
        for (int i = _sides + 2; i <= 3 * _sides; ++i)
            _vertices.append(sideVertex(i));                // V_i; i = n + 2, n + 3, ..., 2n + 1

        for (int i = 1; i <= _sides - 1; ++i)
            _vertices.append(bottomBaseSideVertex(i));      // V_i; i = 3n + 1, 3n + 2, ..., 4n

        _vertices.append(bottomBaseCenterVertex());         // V_(4n + 1)

    }

private:
    static Vertex topBaseVertex(double px, double py, double pz) { return {px, py, pz, 0, 1, 0}; }                      // V_i; i = 0, 1, ..., n
    [[nodiscard]] Vertex topBaseCenterVertex() const { return topBaseVertex(0, _height, 0); }                                         // V_0
    [[nodiscard]] Vertex topBaseSideVertex(int i) const {                                                                                          // V_i; i = 1, 2, ..., n
        return topBaseVertex(_radius * cos(2 * M_PI * i / _sides), _height, _radius * sin(2 * M_PI * i / _sides));
    }
    [[nodiscard]] Trimesh topBaseTrimesh(int i) const {                                                                                            // T_i; i = 1, 2, ..., n - 1
        if (i == _sides - 1)
            return {topBaseCenterVertex(), topBaseSideVertex(1), topBaseSideVertex(_sides)};
        else
            return {topBaseCenterVertex(), topBaseSideVertex(i + 2), topBaseSideVertex(i + 1)};
    }

    [[nodiscard]] Vertex sideVertex(int i) const {                                                                                                 // V_i; i = n + 1, n + 2, ..., 3n
        double px, py, pz;
        if (i <= 2 * _sides)                                                                                                         // p_i; i = n + 1, n + 2, ..., 2n
            px = _radius * cos(2 * M_PI * (i - _sides) / _sides), py = _height, pz = _radius * sin(2 * M_PI * (i - _sides) / _sides);
        else                                                                                                                         // p_i; i = 2n + 1, 2n + 2, ..., 3n
            px = _radius * cos(2 * M_PI * (i + _sides) / _sides), py = 0, pz = _radius * sin(2 * M_PI * (i - _sides) / _sides);

        double nx = px / _radius, ny = 0, nz = pz / _radius;
        return {px, py, pz, nx, ny, nz};
    }
    [[nodiscard]] Trimesh sideTrimesh(int i) const {                                                                                               // T_i; i = n + 1, n + 2, ..., 3n - 1
        if (i < 2 * _sides - 1)
            return {sideVertex(i + 1), sideVertex(i + 2), sideVertex(i + 1 + _sides)};
        else if (i == 2 * _sides - 1)
            return {sideVertex(2 * _sides), sideVertex(_sides + 1), sideVertex(3 * _sides)};
        else if (i < 3 * _sides - 1)
            return {sideVertex(i + 1), sideVertex(i + 2 - _sides), sideVertex(i + 2)};
        else
            return {sideVertex(3 * _sides), sideVertex(_sides  + 1), sideVertex(2 * _sides + 1)};
    }

    static Vertex bottomBaseVertex(double px, double py, double pz) { return {px, py, pz, 0, -1, 0}; }                  // V_i; i = 3n + 1, 3n + 2, ..., 4n + 1
    static Vertex bottomBaseCenterVertex() { return bottomBaseVertex(0, 0, 0); }                                        // V_(4n + 1)
    [[nodiscard]] Vertex bottomBaseSideVertex(int i) const {                                                                                       // V_i; i = 3n + 1, 3n + 3, ..., 4n
        return bottomBaseVertex(_radius * cos(2 * M_PI * i / _sides), 0, _radius * sin(2 * M_PI * i / _sides));
    }
    [[nodiscard]] Trimesh bottomBaseTrimesh(int i) const {                                                                                         // T_i; i = 3n, 3n + 3, ..., 4n - 2
        if (i == _sides - 1)
            return {bottomBaseCenterVertex(), bottomBaseSideVertex(1), bottomBaseSideVertex(_sides)};
        else
            return {bottomBaseCenterVertex(), bottomBaseSideVertex(i + 2), bottomBaseSideVertex(i + 1)};
    }

    double _radius;
    double _height;
    int _sides;
    QList<Vertex> _vertices;
};


#endif //CYLINDER_H
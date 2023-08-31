#ifndef VECTOR_H
#define VECTOR_H


#include <QList>

class Vector {
public:
    Vector() = default;

    explicit Vector(qsizetype size) : values(QList<double>(size, 0)) {}

    Vector(double x, double y) : values(QList<double>({x, y})) {}

    Vector(double x, double y, double z) : values(QList<double>({x, y, z})) {}

    Vector(double x, double y, double z, double w) : values(QList<double>({x, y, z, w})) {}

    Vector operator+(const Vector &other) const;

    Vector operator-(const Vector &other) const;

    Vector operator*(double scalar) const;

    Vector operator/(double scalar) const;

    double operator*(const Vector &other) const;

    Vector &operator+=(const Vector &other);

    Vector &operator-=(const Vector &other);

    Vector &operator*=(double scalar);

    Vector &operator/=(double scalar);

    double operator[](int index) const;

    double &operator[](int index);

    [[nodiscard]] double length() const;

    [[nodiscard]] Vector normalized() const;

    [[nodiscard]] Vector cross(const Vector &other) const;

    [[nodiscard]] qsizetype size() const { return values.size(); }

private:
    QList<double> values;
};


#endif //VECTOR_H
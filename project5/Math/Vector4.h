#ifndef VECTOR4_H
#define VECTOR4_H


class Vector4 {
public:
    double x = 0;
    double y = 0;
    double z = 0;
    double w = 0;

    Vector4() = default;

    Vector4(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

    Vector4 operator+(const Vector4 &other) const;

    Vector4 operator-(const Vector4 &other) const;

    Vector4 operator*(double scalar) const;

    Vector4 operator/(double scalar) const;

    Vector4 &operator+=(const Vector4 &other);

    Vector4 &operator-=(const Vector4 &other);

    Vector4 &operator*=(double scalar);

    Vector4 &operator/=(double scalar);

    double operator[](int index) const;

    double &operator[](int index);

    [[nodiscard]] double length() const;

    [[nodiscard]] Vector4 normalized() const;

    [[nodiscard]] double dot(const Vector4 &other) const;

    [[nodiscard]] Vector4 cross(const Vector4 &other) const;
};


#endif //VECTOR4_H
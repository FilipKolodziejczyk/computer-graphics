#ifndef VECTOR3_H
#define VECTOR3_H


class Vector3 {
public:
    double x = 0;
    double y = 0;
    double z = 0;

    Vector3() = default;

    Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3 &other) const;

    Vector3 operator-(const Vector3 &other) const;

    Vector3 operator*(double scalar) const;

    Vector3 operator/(double scalar) const;

    Vector3 &operator+=(const Vector3 &other);

    Vector3 &operator-=(const Vector3 &other);

    Vector3 &operator*=(double scalar);

    Vector3 &operator/=(double scalar);

    double operator[](int index) const;

    double &operator[](int index);

    [[nodiscard]] double length() const;

    [[nodiscard]] Vector3 normalized() const;

    [[nodiscard]] double dot(const Vector3 &other) const;

    [[nodiscard]] Vector3 cross(const Vector3 &other) const;
};


#endif //VECTOR3_H
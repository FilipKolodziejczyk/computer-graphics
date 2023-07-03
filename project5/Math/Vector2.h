#ifndef VECTOR2_H
#define VECTOR2_H

class Vector2 {
public:
    double x = 0;
    double y = 0;

    Vector2() = default;

    Vector2(double x, double y) : x(x), y(y) {}

    Vector2 operator+(const Vector2 &other) const;

    Vector2 operator-(const Vector2 &other) const;

    Vector2 operator*(double scalar) const;

    Vector2 operator/(double scalar) const;

    Vector2 &operator+=(const Vector2 &other);

    Vector2 &operator-=(const Vector2 &other);

    Vector2 &operator*=(double scalar);

    Vector2 &operator/=(double scalar);

    double operator[](int index) const;

    double &operator[](int index);

    [[nodiscard]] double length() const;

    [[nodiscard]] Vector2 normalized() const;

    [[nodiscard]] double dot(const Vector2 &other) const;

    [[nodiscard]] Vector2 cross(const Vector2 &other) const;

};


#endif // VECTOR2_H
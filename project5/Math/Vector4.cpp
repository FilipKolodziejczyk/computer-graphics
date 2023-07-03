#include <cmath>
#include "Vector4.h"

Vector4 Vector4::operator+(const Vector4 &other) const {
    return {x + other.x, y + other.y, z + other.z, w + other.w};
}

Vector4 Vector4::operator-(const Vector4 &other) const {
    return {x - other.x, y - other.y, z - other.z, w - other.w};
}

Vector4 Vector4::operator*(double scalar) const {
    return {x * scalar, y * scalar, z * scalar, w * scalar};
}

Vector4 Vector4::operator/(double scalar) const {
    return {x / scalar, y / scalar, z / scalar, w / scalar};
}

Vector4 &Vector4::operator+=(const Vector4 &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
}

Vector4 &Vector4::operator-=(const Vector4 &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
}

Vector4 &Vector4::operator*=(double scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
}

Vector4 &Vector4::operator/=(double scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
    return *this;
}

double Vector4::operator[](int index) const {
    switch (index) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        default:
            return w;
    }
}

double &Vector4::operator[](int index) {
    switch (index) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        default:
            return w;
    }
}

double Vector4::length() const {
    return sqrt(x * x + y * y + z * z + w * w);
}

Vector4 Vector4::normalized() const {
    return *this / length();
}

double Vector4::dot(const Vector4 &other) const {
    return x * other.x + y * other.y + z * other.z + w * other.w;
}

Vector4 Vector4::cross(const Vector4 &other) const {
    return {y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x, 0};
}
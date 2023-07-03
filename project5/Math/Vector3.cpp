#include <cmath>
#include "Vector3.h"

Vector3 Vector3::operator+(const Vector3 &other) const {
    return {x + other.x, y + other.y, z + other.z};
}

Vector3 Vector3::operator-(const Vector3 &other) const {
    return {x - other.x, y - other.y, z - other.z};
}

Vector3 Vector3::operator*(double scalar) const {
    return {x * scalar, y * scalar, z * scalar};
}

Vector3 Vector3::operator/(double scalar) const {
    return {x / scalar, y / scalar, z / scalar};
}

Vector3 &Vector3::operator+=(const Vector3 &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector3 &Vector3::operator-=(const Vector3 &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vector3 &Vector3::operator*=(double scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3 &Vector3::operator/=(double scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

double Vector3::operator[](int index) const {
    switch (index) {
        case 0:
            return x;
        case 1:
            return y;
        default:
            return z;
    }
}

double &Vector3::operator[](int index) {
    switch (index) {
        case 0:
            return x;
        case 1:
            return y;
        default:
            return z;
    }
}

double Vector3::length() const {
    return sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::normalized() const {
    return *this / length();
}

double Vector3::dot(const Vector3 &other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vector3 Vector3::cross(const Vector3 &other) const {
    return {y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x};
}
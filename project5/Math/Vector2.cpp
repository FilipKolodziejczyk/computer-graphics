#include <cmath>
#include "Vector2.h"

Vector2 Vector2::operator+(const Vector2 &other) const {
    return {x + other.x, y + other.y};
}

Vector2 Vector2::operator-(const Vector2 &other) const {
    return {x - other.x, y - other.y};
}

Vector2 Vector2::operator*(double scalar) const {
    return {x * scalar, y * scalar};
}

Vector2 Vector2::operator/(double scalar) const {
    return {x / scalar, y / scalar};
}

Vector2 &Vector2::operator+=(const Vector2 &other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vector2 &Vector2::operator-=(const Vector2 &other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector2 &Vector2::operator*=(double scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2 &Vector2::operator/=(double scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

double Vector2::operator[](int index) const {
    return index == 0 ? x : y;
}

double &Vector2::operator[](int index) {
    return index == 0 ? x : y;
}

double Vector2::length() const {
    return sqrt(x * x + y * y);
}

Vector2 Vector2::normalized() const {
    return *this / length();
}

double Vector2::dot(const Vector2 &other) const {
    return x * other.x + y * other.y;
}

Vector2 Vector2::cross(const Vector2 &other) const {
    return {x * other.y - y * other.x, 0};
}
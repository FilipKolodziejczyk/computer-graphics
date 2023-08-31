#include "Vector.h"
#include <cmath>

Vector Vector::operator+(const Vector &other) const {
    if (size() != other.size()) {
        throw std::invalid_argument("Vectors must have the same size");
    }

    Vector result(size());
    for (int i = 0; i < size(); i++) {
        result[i] = (*this)[i] + other[i];
    }
    return result;
}

Vector Vector::operator-(const Vector &other) const {
    if (size() != other.size()) {
        throw std::invalid_argument("Vectors must have the same size");
    }

    Vector result(size());
    for (int i = 0; i < size(); i++) {
        result[i] = (*this)[i] - other[i];
    }
    return result;
}

Vector Vector::operator*(double scalar) const {
    Vector result(size());
    for (int i = 0; i < size(); i++) {
        result[i] = (*this)[i] * scalar;
    }
    return result;
}

Vector Vector::operator/(double scalar) const {
    if (scalar == 0) {
        throw std::invalid_argument("Division by zero");
    }

    Vector result(size());
    for (int i = 0; i < size(); i++) {
        result[i] = (*this)[i] / scalar;
    }
    return result;
}

double Vector::operator*(const Vector &other) const {
    if (size() != other.size()) {
        throw std::invalid_argument("Vectors must have the same size");
    }

    double result = 0;
    for (int i = 0; i < size(); i++) {
        result += (*this)[i] * other[i];
    }
    return result;
}

Vector &Vector::operator+=(const Vector &other) {
    if (size() != other.size()) {
        throw std::invalid_argument("Vectors must have the same size");
    }

    for (int i = 0; i < size(); i++) {
        (*this)[i] += other[i];
    }
    return *this;
}

Vector &Vector::operator-=(const Vector &other) {
    if (size() != other.size()) {
        throw std::invalid_argument("Vectors must have the same size");
    }

    for (int i = 0; i < size(); i++) {
        (*this)[i] -= other[i];
    }
    return *this;
}

Vector &Vector::operator*=(double scalar) {
    for (int i = 0; i < size(); i++) {
        (*this)[i] *= scalar;
    }
    return *this;
}

Vector &Vector::operator/=(double scalar) {
    if (scalar == 0) {
        throw std::invalid_argument("Division by zero");
    }

    for (int i = 0; i < size(); i++) {
        (*this)[i] /= scalar;
    }
    return *this;
}

double Vector::operator[](int index) const {
    return values[index];
}

double &Vector::operator[](int index) {
    return values[index];
}

double Vector::length() const {
    double result = 0;
    for (double value: values) {
        result += value * value;
    }
    return sqrt(result);
}

Vector Vector::normalized() const {
    Vector result(size());
    double length = this->length();
    for (int i = 0; i < size(); i++) {
        result[i] = (*this)[i] / length;
    }
    return result;
}

Vector Vector::cross(const Vector &other) const {
    if (size() != 3 || other.size() != 3) {
        throw std::invalid_argument("Vectors must have size 3");
    }

    Vector result(3);
    result[0] = (*this)[1] * other[2] - (*this)[2] * other[1];
    result[1] = (*this)[2] * other[0] - (*this)[0] * other[2];
    result[2] = (*this)[0] * other[1] - (*this)[1] * other[0];
    return result;
}
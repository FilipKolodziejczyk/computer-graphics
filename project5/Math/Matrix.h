#ifndef MATRIX_H
#define MATRIX_H


#include <QList>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

class Matrix {
public:
    QList<QList<double>> matrix;

    Matrix() = default;

    Matrix(qsizetype rows, qsizetype columns);

    explicit Matrix(const QList<QList<double>> &matrix);

    explicit Matrix(const Vector2 &vector) : matrix({{vector.x, vector.y}}) {}

    explicit Matrix(const Vector3 &vector) : matrix({{vector.x, vector.y, vector.z}}) {}

    explicit Matrix(const Vector4 &vector) : matrix({{vector.x, vector.y, vector.z, vector.w}}) {}

    Matrix operator+(const Matrix &other) const;

    Matrix operator-(const Matrix &other) const;

    Matrix operator*(const Matrix &other) const;

    Matrix operator*(double scalar) const;

    Matrix operator/(double scalar) const;

    Matrix &operator+=(const Matrix &other);

    Matrix &operator-=(const Matrix &other);

    Matrix &operator*=(const Matrix &other);

    Matrix &operator*=(double scalar);

    Matrix &operator/=(double scalar);

    Matrix dot(const Matrix &other) const;

    QList<double> operator[](int index) const;

    QList<double> &operator[](int index);

    [[nodiscard]] Vector2 toVector2() const;

    [[nodiscard]] Vector3 toVector3() const;

    [[nodiscard]] Vector4 toVector4() const;
};

#endif //MATRIX_H
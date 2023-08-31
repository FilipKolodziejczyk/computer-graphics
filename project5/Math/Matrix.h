#ifndef MATRIX_H
#define MATRIX_H


#include <QList>
#include "Vector.h"

class Matrix {
public:
    QList<QList<double>> matrix;

    Matrix() = default;

    Matrix(qsizetype rows, qsizetype columns);

    explicit Matrix(const QList<QList<double>> &matrix);

    explicit Matrix(const Vector &vector);

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

    QList<double> operator[](int index) const;

    QList<double> &operator[](int index);

    [[nodiscard]] Vector toVector() const;

    [[nodiscard]] qsizetype rows() const { return matrix.size(); }

    [[nodiscard]] qsizetype columns() const { return matrix[0].size(); }
};

#endif //MATRIX_H
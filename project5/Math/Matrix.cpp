#include "Matrix.h"

Matrix::Matrix(qsizetype rows, qsizetype columns) {
    if (rows <= 0 || columns <= 0)
        throw std::invalid_argument("Matrix must have at least one row and one column");

    matrix = QList<QList<double>>(rows, QList<double>(columns));
}

Matrix::Matrix(const QList<QList<double>> &matrix) {
    if (matrix.empty() || matrix[0].empty())
        throw std::invalid_argument("Matrix must have at least one row and one column");

    for (int i = 1; i < matrix.size(); i++)
        if (matrix[i].size() != matrix[0].size())
            throw std::invalid_argument("Matrix rows must have the same size");

    this->matrix = matrix;
}

Matrix::Matrix(const Vector &vector) {
    matrix = QList<QList<double>>(vector.size(), QList<double>(1));
    for (int i = 0; i < vector.size(); i++)
        matrix[i][0] = vector[i];
}

Matrix Matrix::operator+(const Matrix &other) const {
    if (rows() != other.rows() || columns() != other.columns())
        throw std::invalid_argument("Matrix dimensions must agree");

    Matrix result(matrix);
    for (int i = 0; i < result.rows(); i++)
        for (int j = 0; j < result.columns(); j++)
            result[i][j] += other[i][j];

    return result;
}

Matrix Matrix::operator-(const Matrix &other) const {
    if (rows() != other.rows() || columns() != other.columns())
        throw std::invalid_argument("Matrix dimensions must agree");

    Matrix result(matrix);
    for (int i = 0; i < result.rows(); i++)
        for (int j = 0; j < result.columns(); j++)
            result[i][j] -= other[i][j];

    return result;
}

Matrix Matrix::operator*(const Matrix &other) const {
    if (columns() != other.rows())
        throw std::invalid_argument("Matrix dimensions must agree");

    Matrix result(rows(), other.columns());
    for (int i = 0; i < result.rows(); i++)
        for (int j = 0; j < result.columns(); j++)
            for (int k = 0; k < columns(); k++)
                result[i][j] += (*this)[i][k] * other[k][j];

    return result;
}

Matrix Matrix::operator*(double scalar) const {
    Matrix result(matrix);
    for (int i = 0; i < result.rows(); i++)
        for (int j = 0; j < result.columns(); j++)
            result[i][j] *= scalar;

    return result;
}

Matrix Matrix::operator/(double scalar) const {
    if (scalar == 0)
        throw std::invalid_argument("Division by zero");

    Matrix result(matrix);
    for (int i = 0; i < result.rows(); i++)
        for (int j = 0; j < result.columns(); j++)
            result[i][j] /= scalar;

    return result;
}

Matrix &Matrix::operator+=(const Matrix &other) {
    if (rows() != other.rows() || columns() != other.columns())
        throw std::invalid_argument("Matrix dimensions must agree");

    for (int i = 0; i < rows(); i++)
        for (int j = 0; j < columns(); j++)
            (*this)[i][j] += other[i][j];

    return *this;
}

Matrix &Matrix::operator-=(const Matrix &other) {
    if (rows() != other.rows() || columns() != other.columns())
        throw std::invalid_argument("Matrix dimensions must agree");

    for (int i = 0; i < rows(); i++)
        for (int j = 0; j < columns(); j++)
            (*this)[i][j] -= other[i][j];

    return *this;
}

Matrix &Matrix::operator*=(const Matrix &other) {
    if (columns() != other.rows())
        throw std::invalid_argument("Matrix dimensions must agree");

    Matrix result(rows(), other.columns());
    for (int i = 0; i < result.rows(); i++)
        for (int j = 0; j < result.columns(); j++)
            for (int k = 0; k < columns(); k++)
                result[i][j] += (*this)[i][k] * other[k][j];

    matrix = result.matrix;
    return *this;
}

Matrix &Matrix::operator*=(double scalar) {
    for (auto &row: matrix)
        for (auto &element: row)
            element *= scalar;

    return *this;
}

Matrix &Matrix::operator/=(double scalar) {
    for (auto &row: matrix)
        for (auto &element: row)
            element /= scalar;

    return *this;
}

QList<double> Matrix::operator[](int row) const {
    if (row < 0 || row >= matrix.size())
        throw std::invalid_argument("Row index out of bounds");

    return matrix[row];
}

QList<double> &Matrix::operator[](int row) {
    if (row < 0 || row >= rows())
        throw std::invalid_argument("Row index out of bounds");

    return matrix[row];
}

Vector Matrix::toVector() const {
    if (columns() != 1)
        throw std::invalid_argument("Matrix must have only one column");

    Vector result(rows());
    for (int i = 0; i < rows(); i++)
        result[i] = matrix[i][0];

    return result;
}
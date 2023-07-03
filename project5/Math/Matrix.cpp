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

Matrix Matrix::operator+(const Matrix &other) const {
    if (matrix.size() != other.matrix.size() || matrix[0].size() != other.matrix[0].size())
        throw std::invalid_argument("Matrix dimensions must agree");

    Matrix result(matrix);
    for (int i = 0; i < matrix.size(); i++)
        for (int j = 0; j < matrix[0].size(); j++)
            result.matrix[i][j] += other.matrix[i][j];

    return result;
}

Matrix Matrix::operator-(const Matrix &other) const {
    if (matrix.size() != other.matrix.size() || matrix[0].size() != other.matrix[0].size())
        throw std::invalid_argument("Matrix dimensions must agree");

    Matrix result(matrix);
    for (int i = 0; i < matrix.size(); i++)
        for (int j = 0; j < matrix[0].size(); j++)
            result.matrix[i][j] -= other.matrix[i][j];

    return result;
}

Matrix Matrix::operator*(const Matrix &other) const {
    if (matrix[0].size() != other.matrix.size())
        throw std::invalid_argument("Matrix dimensions must agree");

    Matrix result(matrix.size(), other.matrix[0].size());
    for (int i = 0; i < result.matrix.size(); i++)
        for (int j = 0; j < result.matrix[0].size(); j++)
            for (int k = 0; k < matrix[0].size(); k++)
                result.matrix[i][j] += matrix[i][k] * other.matrix[k][j];

    return result;
}

Matrix Matrix::operator*(double scalar) const {
    Matrix result(matrix);
    for (int i = 0; i < matrix.size(); i++)
        for (int j = 0; j < matrix[0].size(); j++)
            result.matrix[i][j] *= scalar;

    return result;
}

Matrix Matrix::operator/(double scalar) const {
    Matrix result(matrix);
    for (int i = 0; i < matrix.size(); i++)
        for (int j = 0; j < matrix[0].size(); j++)
            result.matrix[i][j] /= scalar;

    return result;
}

Matrix &Matrix::operator+=(const Matrix &other) {
    if (matrix.size() != other.matrix.size() || matrix[0].size() != other.matrix[0].size())
        throw std::invalid_argument("Matrix dimensions must agree");

    for (int i = 0; i < matrix.size(); i++)
        for (int j = 0; j < matrix[0].size(); j++)
            matrix[i][j] += other.matrix[i][j];

    return *this;
}

Matrix &Matrix::operator-=(const Matrix &other) {
    if (matrix.size() != other.matrix.size() || matrix[0].size() != other.matrix[0].size())
        throw std::invalid_argument("Matrix dimensions must agree");

    for (int i = 0; i < matrix.size(); i++)
        for (int j = 0; j < matrix[0].size(); j++)
            matrix[i][j] -= other.matrix[i][j];

    return *this;
}

Matrix &Matrix::operator*=(const Matrix &other) {
    if (matrix[0].size() != other.matrix.size())
        throw std::invalid_argument("Matrix dimensions must agree");

    Matrix result(matrix.size(), other.matrix[0].size());
    for (int i = 0; i < result.matrix.size(); i++)
        for (int j = 0; j < result.matrix[0].size(); j++)
            for (int k = 0; k < matrix[0].size(); k++)
                result.matrix[i][j] += matrix[i][k] * other.matrix[k][j];

    matrix = result.matrix;
    return *this;
}

Matrix &Matrix::operator*=(double scalar) {
    for (auto & row : matrix)
        for (auto & element : row)
            element *= scalar;

    return *this;
}

Matrix &Matrix::operator/=(double scalar) {
    for (auto & row : matrix)
        for (auto & element : row)
            element /= scalar;

    return *this;
}

QList<double> Matrix::operator[](int row) const {
    if (row < 0 || row >= matrix.size())
        throw std::invalid_argument("Row index out of bounds");

    return matrix[row];
}

QList<double> &Matrix::operator[](int row) {
    if (row < 0 || row >= matrix.size())
        throw std::invalid_argument("Row index out of bounds");

    return matrix[row];
}

Vector2 Matrix::toVector2() const {
    if (matrix.size() != 2 || matrix[0].size() != 1)
        throw std::invalid_argument("Matrix must be 2x1");

    return {matrix[0][0], matrix[1][0]};
}

Vector3 Matrix::toVector3() const {
    if (matrix.size() != 3 || matrix[0].size() != 1)
        throw std::invalid_argument("Matrix must be 3x1");

    return {matrix[0][0], matrix[1][0], matrix[2][0]};
}

Vector4 Matrix::toVector4() const {
    if (matrix.size() != 4 || matrix[0].size() != 1)
        throw std::invalid_argument("Matrix must be 4x1");

    return {matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0]};
}
#pragma once

#include "vector.h"

#include <cassert>

template <std::size_t NRow, std::size_t NCol>
class Matrix {
public:
    Vector<NCol> rows[NRow] = {{}};

    Matrix();
    Matrix(const Matrix&) = default;
    Matrix(Matrix&&) = default;
    Matrix& operator=(const Matrix&) = default;
    Matrix& operator=(Matrix&) = default;
    ~Matrix() = default;

    Vector<NCol>& operator[](const std::size_t index) {
        assert(index >= 0 && index < NRow);
        return rows[index];
    }

    const Vector<NCol>& operator[](const std::size_t index) const {
        assert(index >= 0 && index < NRow);
        return rows[index];
    }

    Matrix<NCol, NRow> transpose() const {
        Matrix<NCol, NRow> result;
        for (auto i = 0; i < NRow; ++i) {
            for (auto j = 0; j < NCol; ++j) {
                result[j][i] = rows[i][j];
            }
        }

        return result;
    }

    double cofactor(const std::size_t row, const std::size_t col) const {
        auto minor_matrix = minorMatrix(row, col);
        return ((row + col) % 2 == 0 ? 1 : -1) * minor_matrix.determinant();
    }

    template <std::size_t Row = NRow - 1, std::size_t Col = NCol - 1>
    Matrix<Row, Col> minorMatrix(const std::size_t row, const std::size_t col) requires(NRow > 1 && NCol > 1) {
        Matrix<Row, Col> result;

        auto r = 0;
        for (auto i = 0; i < Row; ++i) {
            if (i == row) {
                continue;
            }
            auto c = 0;
            for (int j = 0; j < Col; ++j) {
                if (j == col) {
                    continue;
                }

                result[r][c] = rows[i][j];
                ++c;
            }
            ++r;
        }

        return result;
    }

    // I don't care about optimization at the moment.
    // It might be useful to optimize the following operations though

    double determinant() requires(NRow > 1 && NRow == NCol) {
        double determinant{};
        for (auto col = 0; col < NRow; ++col) {
            double sign = ((col % 2) == 0) ? 1 : -1;
            determinant += sign * rows[0][col] * minorMatrix(0, col).determinant();
        }

        return determinant;
    }

    // Base case
    double determinant() requires(NRow == 1 && NCol == 1) {
        return rows[0][0];
    }

    Matrix<NRow, NCol> adjucate() const {
        Matrix<NRow, NCol> adjucate_matrix;
        for (auto i = 0; i < NRow; ++i) {
            for (auto j = 0; j < NCol; ++j) {
                adjucate_matrix[j][i] = cofactor(i, j);
            }
        }
        return adjucate_matrix;
    }

    Matrix<NRow, NCol> inverse() const {
        double det = determinant();
        assert(det != 0);
        return adjucate() / det;
    }

    // Overload division operator for scalar division
    Matrix<NRow, NCol> operator/(double scalar) const {
        Matrix<NRow, NCol> result;
        for (std::size_t i = 0; i < NRow; ++i) {
            for (std::size_t j = 0; j < NCol; ++j) {
                result[i][j] = rows[i][j] / scalar;
            }
        }
        return result;
    }
};

template <int R1, int C1, int C2>
Matrix<R1, C2> operator*(const Matrix<R1, C1>& lhs, const Matrix<C1, C2>& rhs) {
    Matrix<R1, C2> result;
    for (int i = R1; i--;)
        for (int j = C2; j--;)
            for (int k = C1; k--; result[i][j] += lhs[i][k] * rhs[k][j])
                ;
    return result;
}

template <int NRow, int NCol>
Matrix<NRow, NCol> operator*(const Matrix<NRow, NCol>& lhs, const double& val) {
    Matrix<NRow, NCol> result;
    for (int i = NRow; i--; result[i] = lhs[i] * val)
        ;
    return result;
}

template <int NRow, int NCol>
Matrix<NRow, NCol> operator/(const Matrix<NRow, NCol>& lhs, const double& val) {
    Matrix<NRow, NCol> result;
    for (int i = NRow; i--; result[i] = lhs[i] / val)
        ;
    return result;
}

template <int NRow, int NCol>
Matrix<NRow, NCol> operator+(const Matrix<NRow, NCol>& lhs, const Matrix<NRow, NCol>& rhs) {
    Matrix<NRow, NCol> result;
    for (int i = NRow; i--;)
        for (int j = NCol; j--; result[i][j] = lhs[i][j] + rhs[i][j])
            ;
    return result;
}

template <int NRow, int NCol>
Matrix<NRow, NCol> operator-(const Matrix<NRow, NCol>& lhs, const Matrix<NRow, NCol>& rhs) {
    Matrix<NRow, NCol> result;
    for (int i = NRow; i--;)
        for (int j = NCol; j--; result[i][j] = lhs[i][j] - rhs[i][j])
            ;
    return result;
}

template <int NRow, int NCol>
std::ostream& operator<<(std::ostream& out, const Matrix<NRow, NCol>& m) {
    for (int i = 0; i < NRow; i++)
        out << m[i] << std::endl;
    return out;
}

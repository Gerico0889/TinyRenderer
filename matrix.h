#pragma once

#include "vector.h"

#include <cassert>

template <std::size_t NRow, std::size_t NCol>
class Matrix {
public:
    Vector<NCol> rows[NRow] = {{}};

    Matrix() = default;
    Matrix(const Matrix&) = default;
    Matrix(Matrix&&) = default;
    Matrix& operator=(const Matrix&) = default;
    Matrix& operator=(Matrix&) = default;
    ~Matrix() = default;

    template <typename... Args,
              typename = std::enable_if_t<sizeof...(Args) == NRow * NCol && (std::is_convertible_v<Args, double> && ...)>>
    Matrix(Args... args) {
        const double values[] = {static_cast<double>(args)...};
        for (std::size_t i = 0; i < NRow; ++i) {
            for (std::size_t j = 0; j < NCol; ++j) {
                rows[i][j] = values[i * NCol + j];
            }
        }
    }

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
    Matrix<Row, Col> minorMatrix(const std::size_t row, const std::size_t col) const requires(NRow > 1 && NCol > 1) {
        Matrix<Row, Col> result;

        auto r = 0;
        for (std::size_t i = 0; i < Row; ++i) {
            if (i == row) {
                continue;
            }
            auto c = 0;
            for (std::size_t j = 0; j < Col; ++j) {
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
    double determinant() const requires(NRow > 1 && NRow == NCol) {
        if (NRow == 2) {
            // Fast path for 2x2 matrices
            return rows[0][0] * rows[1][1] - rows[0][1] * rows[1][0];
        }

        double result = 0;
        for (std::size_t j = 0; j < NCol; j++) {
            double cofactor = rows[0][j];
            // Apply the alternating sign pattern: + - + - ...
            if (j % 2)
                cofactor = -cofactor;

            // Calculate the minor determinant
            result += cofactor * minorDeterminant(0, j);
        }
        return result;
    }

    // Base case
    double determinant() const requires(NRow == 1 && NCol == 1) {
        return rows[0][0];
    }

    double minorDeterminant(std::size_t row, std::size_t col) const requires(NRow > 1 && NRow == NCol) {
        // Create the minor matrix (size one less than the original)
        Matrix<NRow - 1, NCol - 1> minor;

        // Fill the minor matrix by skipping the specified row and column
        std::size_t minorRow = 0;
        for (std::size_t i = 0; i < NRow; i++) {
            if (i == row)
                continue;

            std::size_t minorCol = 0;
            for (std::size_t j = 0; j < NCol; j++) {
                if (j == col)
                    continue;
                minor[minorRow][minorCol] = rows[i][j];
                minorCol++;
            }
            minorRow++;
        }

        return minor.determinant();
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

    Matrix<NRow, NCol> invert_transpose() const {
        Matrix<NRow, NCol> adjugate_transpose;
        for (auto i = 0; i < NRow; ++i) {
            for (auto j = 0; j < NCol; ++j) {
                adjugate_transpose[i][j] = this->cofactor(i, j);
            }
        }

        return adjugate_transpose / (adjugate_transpose[0] * rows[0]);
    }
};

template <std::size_t NRow, std::size_t NCol>
Vector<NCol> operator*(const Vector<NRow>& lhs, const Matrix<NRow, NCol>& rhs) {
    return (Matrix<1, NRow>{{lhs}} * rhs)[0];
}

template <std::size_t NRow, std::size_t NCol>
Vector<NRow> operator*(const Matrix<NRow, NCol>& lhs, const Vector<NCol>& rhs) {
    Vector<NRow> result;
    for (std::size_t i = 0; i < NRow; ++i) {
        for (std::size_t j = 0; j < NCol; ++j) {
            result[i] += lhs[i][j] * rhs[j];
        }
    }
    return result;
}

template <std::size_t R1, std::size_t C1, std::size_t C2>
Matrix<R1, C2> operator*(const Matrix<R1, C1>& lhs, const Matrix<C1, C2>& rhs) {
    Matrix<R1, C2> result;
    for (std::size_t i = R1; i--;)
        for (std::size_t j = C2; j--;)
            for (std::size_t k = C1; k--; result[i][j] += lhs[i][k] * rhs[k][j])
                ;
    return result;
}

template <std::size_t NRow, std::size_t NCol>
Matrix<NRow, NCol> operator*(const Matrix<NRow, NCol>& lhs, const double& val) {
    Matrix<NRow, NCol> result;
    for (std::size_t i = NRow; i--; result[i] = lhs[i] * val)
        ;
    return result;
}

template <std::size_t NRow, std::size_t NCol>
Matrix<NRow, NCol> operator/(const Matrix<NRow, NCol>& lhs, const double& val) {
    Matrix<NRow, NCol> result;
    for (std::size_t i = NRow; i--; result[i] = lhs[i] / val)
        ;
    return result;
}

template <std::size_t NRow, std::size_t NCol>
Matrix<NRow, NCol> operator+(const Matrix<NRow, NCol>& lhs, const Matrix<NRow, NCol>& rhs) {
    Matrix<NRow, NCol> result;
    for (std::size_t i = NRow; i--;)
        for (std::size_t j = NCol; j--; result[i][j] = lhs[i][j] + rhs[i][j])
            ;
    return result;
}

template <std::size_t NRow, std::size_t NCol>
Matrix<NRow, NCol> operator-(const Matrix<NRow, NCol>& lhs, const Matrix<NRow, NCol>& rhs) {
    Matrix<NRow, NCol> result;
    for (std::size_t i = NRow; i--;)
        for (std::size_t j = NCol; j--; result[i][j] = lhs[i][j] - rhs[i][j])
            ;
    return result;
}

template <std::size_t NRow, std::size_t NCol>
std::ostream& operator<<(std::ostream& out, const Matrix<NRow, NCol>& m) {
    for (std::size_t i = 0; i < NRow; i++)
        out << m[i] << std::endl;
    return out;
}

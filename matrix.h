#pragma once
#include "vector.h"

#include <cassert>
#include <iostream>

// Forward declaration for determinant helper
template <int N>
struct DeterminantHelper;

template <int NRows, int NCols>
struct Matrix {
    Vector<NCols> rows[NRows] = {};

    Vector<NCols>& operator[](const int idx) {
        assert(idx >= 0 && idx < NRows);
        return rows[idx];
    }
    const Vector<NCols>& operator[](const int idx) const {
        assert(idx >= 0 && idx < NRows);
        return rows[idx];
    }

    double det() const {
        return DeterminantHelper<NCols>::det(*this);
    }

    double cofactor(const int row, const int col) const {
        Matrix<NRows - 1, NCols - 1> submatrix;
        for (int i = 0, si = 0; i < NRows; ++i) {
            if (i == row)
                continue;
            for (int j = 0, sj = 0; j < NCols; ++j) {
                if (j == col)
                    continue;
                submatrix[si][sj] = rows[i][j];
                ++sj;
            }
            ++si;
        }
        return submatrix.det() * ((row + col) % 2 ? -1 : 1);
    }

    Matrix<NRows, NCols> invertTranspose() const {
        Matrix<NRows, NCols> adjugate_transpose;
        for (int i = 0; i < NRows; ++i) {
            for (int j = 0; j < NCols; ++j) {
                adjugate_transpose[i][j] = cofactor(i, j);
            }
        }
        return adjugate_transpose / (adjugate_transpose[0] * rows[0]);
    }

    Matrix<NRows, NCols> Inverti() const {
        return invertTranspose().transpose();
    }

    Matrix<NCols, NRows> transpose() const {
        Matrix<NCols, NRows> ret;
        for (int i = 0; i < NCols; ++i) {
            for (int j = 0; j < NRows; ++j) {
                ret[i][j] = rows[j][i];
            }
        }
        return ret;
    }
};

template <int NRows, int NCols>
Vector<NCols> operator*(const Vector<NRows>& lhs, const Matrix<NRows, NCols>& rhs) {
    Matrix<1, NRows> temp;
    temp[0] = lhs;
    return (temp * rhs)[0];
}

template <int NRows, int NCols>
Vector<NRows> operator*(const Matrix<NRows, NCols>& lhs, const Vector<NCols>& rhs) {
    Vector<NRows> ret;
    for (int i = 0; i < NRows; ++i) {
        ret[i] = lhs[i] * rhs;
    }
    return ret;
}

template <int R1, int C1, int C2>
Matrix<R1, C2> operator*(const Matrix<R1, C1>& lhs, const Matrix<C1, C2>& rhs) {
    Matrix<R1, C2> result;
    for (int i = 0; i < R1; ++i) {
        for (int j = 0; j < C2; ++j) {
            result[i][j] = 0;
            for (int k = 0; k < C1; ++k) {
                result[i][j] += lhs[i][k] * rhs[k][j];
            }
        }
    }
    return result;
}

template <int NRows, int NCols>
Matrix<NRows, NCols> operator*(const Matrix<NRows, NCols>& lhs, const double& val) {
    Matrix<NRows, NCols> result;
    for (int i = 0; i < NRows; ++i) {
        result[i] = lhs[i] * val;
    }
    return result;
}

template <int NRows, int NCols>
Matrix<NRows, NCols> operator/(const Matrix<NRows, NCols>& lhs, const double& val) {
    Matrix<NRows, NCols> result;
    for (int i = 0; i < NRows; ++i) {
        result[i] = lhs[i] / val;
    }
    return result;
}

template <int NRows, int NCols>
Matrix<NRows, NCols> operator+(const Matrix<NRows, NCols>& lhs, const Matrix<NRows, NCols>& rhs) {
    Matrix<NRows, NCols> result;
    for (int i = 0; i < NRows; ++i) {
        for (int j = 0; j < NCols; ++j) {
            result[i][j] = lhs[i][j] + rhs[i][j];
        }
    }
    return result;
}

template <int NRows, int NCols>
Matrix<NRows, NCols> operator-(const Matrix<NRows, NCols>& lhs, const Matrix<NRows, NCols>& rhs) {
    Matrix<NRows, NCols> result;
    for (int i = 0; i < NRows; ++i) {
        for (int j = 0; j < NCols; ++j) {
            result[i][j] = lhs[i][j] - rhs[i][j];
        }
    }
    return result;
}

template <int NRows, int NCols>
std::ostream& operator<<(std::ostream& out, const Matrix<NRows, NCols>& m) {
    for (int i = 0; i < NRows; ++i) {
        out << m[i] << std::endl;
    }
    return out;
}

// Determinant helper (template metaprogramming)
template <int N>
struct DeterminantHelper {
    static double det(const Matrix<N, N>& src) {
        double ret = 0;
        for (int i = 0; i < N; ++i) {
            ret += src[0][i] * src.cofactor(0, i);
        }
        return ret;
    }
};

template <>
struct DeterminantHelper<1> {
    static double det(const Matrix<1, 1>& src) {
        return src[0][0];
    }
};

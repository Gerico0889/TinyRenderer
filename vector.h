#pragma once
#include <cassert>
#include <cmath>
#include <iostream>

template <int N>
struct Vector {
    double data[N] = {0};

    double& operator[](const int i) {
        assert(i >= 0 && i < N);
        return data[i];
    }
    double operator[](const int i) const {
        assert(i >= 0 && i < N);
        return data[i];
    }
};

template <int N>
double operator*(const Vector<N>& lhs, const Vector<N>& rhs) {
    double ret = 0;
    for (int i = 0; i < N; ++i) {
        ret += lhs[i] * rhs[i];
    }
    return ret;
}

template <int N>
Vector<N> operator+(const Vector<N>& lhs, const Vector<N>& rhs) {
    Vector<N> ret = lhs;
    for (int i = 0; i < N; ++i) {
        ret[i] += rhs[i];
    }
    return ret;
}

template <int N>
Vector<N> operator-(const Vector<N>& lhs, const Vector<N>& rhs) {
    Vector<N> ret = lhs;
    for (int i = 0; i < N; ++i) {
        ret[i] -= rhs[i];
    }
    return ret;
}

template <int N>
Vector<N> operator*(const Vector<N>& lhs, const double& rhs) {
    Vector<N> ret = lhs;
    for (int i = 0; i < N; ++i) {
        ret[i] *= rhs;
    }
    return ret;
}

template <int N>
Vector<N> operator*(const double& lhs, const Vector<N>& rhs) {
    return rhs * lhs;
}

template <int N>
Vector<N> operator/(const Vector<N>& lhs, const double& rhs) {
    Vector<N> ret = lhs;
    for (int i = 0; i < N; ++i) {
        ret[i] /= rhs;
    }
    return ret;
}

template <int N>
std::ostream& operator<<(std::ostream& out, const Vector<N>& v) {
    for (int i = 0; i < N; ++i) {
        out << v[i] << " ";
    }
    return out;
}

// Specializations for 2D, 3D, and 4D Vectors

template <>
struct Vector<2> {
    double x = 0, y = 0;

    double& operator[](const int i) {
        assert(i >= 0 && i < 2);
        return i ? y : x;
    }
    double operator[](const int i) const {
        assert(i >= 0 && i < 2);
        return i ? y : x;
    }
};

template <>
struct Vector<3> {
    double x = 0, y = 0, z = 0;

    double& operator[](const int i) {
        assert(i >= 0 && i < 3);
        if (i == 0)
            return x;
        if (i == 1)
            return y;
        return z;
    }
    double operator[](const int i) const {
        assert(i >= 0 && i < 3);
        if (i == 0)
            return x;
        if (i == 1)
            return y;
        return z;
    }
};

template <>
struct Vector<4> {
    double x = 0, y = 0, z = 0, w = 0;

    double& operator[](const int i) {
        assert(i >= 0 && i < 4);
        if (i == 0)
            return x;
        if (i == 1)
            return y;
        if (i == 2)
            return z;
        return w;
    }
    double operator[](const int i) const {
        assert(i >= 0 && i < 4);
        if (i == 0)
            return x;
        if (i == 1)
            return y;
        if (i == 2)
            return z;
        return w;
    }
    Vector<2> xy() const {
        return {x, y};
    }
    Vector<3> xyz() const {
        return {x, y, z};
    }
};

typedef Vector<2> Vec2;
typedef Vector<3> Vec3;
typedef Vector<4> Vec4;

template <int N>
double norm(const Vector<N>& v) {
    return std::sqrt(v * v);
}

template <int N>
Vector<N> normalized(const Vector<N>& v) {
    return v / norm(v);
}

inline Vec3 cross(const Vec3& v1, const Vec3& v2) {
    return {v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x};
}

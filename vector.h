#pragma once

#include <array>
#include <iostream>
#include <ostream>

template <std::size_t N>
class Vector {
public:
    std::array<double, N> data; // Encapsulation is barely a concern here

    Vector() {
        data.fill(0.0);
    }

    template <typename... Args, typename = std::enable_if<sizeof...(Args) == N && (std::is_convertible_v<Args, double> && ...)>>
    Vector(Args... args)
        : data{{static_cast<double>(args)...}} {
    }

    Vector(const Vector&) = default;
    Vector(Vector&&) = default;
    Vector& operator=(const Vector&) = default;
    Vector& operator=(Vector&) = default;
    ~Vector() = default;

    double dot(const Vector& vector) const {
        double result = 0.0;
        for (std::size_t i = 0; i < N; ++i) {
            result += data[i] * vector.data[i];
        }
        return result;
    }

    Vector& operator+=(const Vector& other) {
        for (std::size_t i = 0; i < N; ++i) {
            data[i] += other.data[i];
        }
        return *this;
    }

    Vector& operator-=(const Vector& other) {
        for (std::size_t i = 0; i < N; ++i) {
            data[i] -= other.data[i];
        }
        return *this;
    }

    Vector& operator*=(double scalar) {
        for (std::size_t i = 0; i < N; ++i) {
            data[i] *= scalar;
        }
        return *this;
    }

    Vector& operator/=(double scalar) {
        for (std::size_t i = 0; i < N; ++i) {
            data[i] /= scalar;
        }
        return *this;
    }

    double& operator[](std::size_t index) {
        return data[index];
    }

    const double& operator[](std::size_t index) const {
        return data[index];
    }

    static Vector zero() {
        return Vector();
    }

    /* Dimension specific accessors */

    // x() and y() are available for N >= 2
    std::enable_if_t<N >= 2, double&> x() {
        return data[0];
    }
    template <std::size_t M = N>
    std::enable_if_t<M >= 2, const double&> x() const {
        return data[0];
    }

    template <std::size_t M = N>
    std::enable_if_t<M >= 2, double&> y() {
        return data[1];
    }
    template <std::size_t M = N>
    std::enable_if_t<M >= 2, const double&> y() const {
        return data[1];
    }

    // z() is available for N >= 3
    template <std::size_t M = N>
    std::enable_if_t<M >= 3, double&> z() {
        return data[2];
    }
    template <std::size_t M = N>
    std::enable_if_t<M >= 3, const double&> z() const {
        return data[2];
    }

    // w() is available for N >= 4
    template <std::size_t M = N>
    std::enable_if_t<M >= 4, double&> w() {
        return data[3];
    }
    template <std::size_t M = N>
    std::enable_if_t<M >= 4, const double&> w() const {
        return data[3];
    }

    // Cross product (only for N=3)
    template <std::size_t M = N>
    std::enable_if_t<M == 3, Vector<3>> cross(const Vector<3>& other) const {
        return Vector<3>(
            data[1] * other.data[2] - data[2] * other.data[1],
            data[2] * other.data[0] - data[0] * other.data[2],
            data[0] * other.data[1] - data[1] * other.data[0]);
    }
};

/* Non-member operator overloads */

template <std::size_t N>
inline Vector<N> operator+(Vector<N> lhs, const Vector<N>& rhs) {
    lhs += rhs;
    return lhs;
}

template <std::size_t N>
inline Vector<N> operator-(Vector<N> lhs, const Vector<N>& rhs) {
    lhs -= rhs;
    return lhs;
}

template <std::size_t N>
inline Vector<N> operator*(Vector<N> lhs, double scalar) {
    lhs *= scalar;
    return lhs;
}

template <std::size_t N>
inline Vector<N> operator*(double scalar, Vector<N> rhs) {
    rhs *= scalar;
    return rhs;
}

template <std::size_t N>
inline Vector<N> operator/(Vector<N> lhs, double scalar) {
    if (scalar == 0) {
        std::cerr << "Err: division by 0";
        return lhs; // Better handle this case instead of returning lhs
    }
    lhs /= scalar;
    return lhs;
}

template <std::size_t N>
inline std::ostream& operator<<(std::ostream& os, const Vector<N>& vec) {
    os << "Vector" << N << "(";
    for (std::size_t i = 0; i < N; ++i) {
        os << vec.data[i] << (i == N - 1 ? "" : ", ");
    }
    os << ")";
    return os;
}

using Vec2 = Vector<2>;
using Vec3 = Vector<3>;
using Vec4 = Vector<4>;

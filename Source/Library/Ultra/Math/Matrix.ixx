export module Ultra.Math.Matrix;

import <numbers>;
import Ultra.Core;

export namespace Ultra {

// Concepts
template<typename T>
concept MatrixNumerics = 
    std::is_same_v<T, double> ||
    std::is_same_v<T, float>;

///
/// @brief Matrix[2-4 x 2-4]D
///
template<MatrixNumerics T, size_t N>
struct NMatrix {
    array<array<T, N>, N> Data {};

    // Assignment
    array<T, N> &operator[](size_t index) {
        return Data[index];
    }
    const array<T, N> &operator[](size_t index) const {
        return Data[index];
    }

    // Methods
    static NMatrix Identity() {
        NMatrix m;
        for (size_t i = 0; i < N; i++) {
            m[i][i] = 1;
        }
        return m;
    }
    NMatrix Transpose() {
        NMatrix result;
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                result[j][i] = Data[i][j];
            }
        }
        return result;
    }

    // Operations
    NMatrix operator+(const NMatrix &other) {
        NMatrix result;
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                result[i][j] = Data[i][j] + other[i][j];
            }
        }
        return result;
    }
    NMatrix operator-(const NMatrix &other) {
        NMatrix result;
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                result[i][j] = Data[i][j] - other[i][j];
            }
        }
        return result;
    }
    NMatrix operator*(const NMatrix &other) {
        NMatrix result;
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                for (size_t k = 0; k < N; k++) {
                    result[i][j] += Data[i][k] * other[k][j];
                }
            }
        }
        return result;
    }
    NMatrix operator*(T scalar) {
        NMatrix result;
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                result[i][j] = Data[i][j] * scalar;
            }
        }
        return result;
    }
    NMatrix operator/(T scalar) {
        ValidateScalar(scalar);
        NMatrix result;
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                result[i][j] = Data[i][j] / scalar;
            }
        }
        return result;
    }
    NMatrix &operator+=(const NMatrix &other) {
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                Data[i][j] += other[i][j];
            }
        }
        return *this;
    }
    NMatrix &operator-=(const NMatrix &other) {
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                Data[i][j] -= other[i][j];
            }
        }
        return *this;
    }
    NMatrix &operator*=(const NMatrix &other) {
        *this = *this * other;
        return *this;
    }
    NMatrix &operator*=(T scalar) {
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                Data[i][j] *= scalar;
            }
        }
        return *this;
    }
    NMatrix &operator/=(T scalar) {
        ValidateScalar(scalar);
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                Data[i][j] /= scalar;
            }
        }
        return *this;
    }

private:
    void ValidateScalar(T scalar) {
        if (std::abs(scalar) < std::numeric_limits<T>::epsilon()) {
            throw std::invalid_argument("Cannot divide by zero.");
        }
    }
};

template<MatrixNumerics T>
using NMatrix2 = NMatrix<T, 2>;

template<MatrixNumerics T>
using NMatrix3 = NMatrix<T, 2>;

template<MatrixNumerics T>
using NMatrix4 = NMatrix<T, 4>;

using NMatrix2d = NMatrix2<double>;
using NMatrix3d = NMatrix3<double>;
using NMatrix4d = NMatrix4<double>;

using NMatrix2f = NMatrix2<float>;
using NMatrix3f = NMatrix3<float>;
using NMatrix4f = NMatrix4<float>;


}

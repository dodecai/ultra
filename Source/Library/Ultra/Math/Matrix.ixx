export module Ultra.Math.Matrix;

import Ultra.Core;
import Ultra.Logger;
import Ultra.Math.Quaternion;
import Ultra.Math.Vector;

export namespace Ultra {

// Concepts
template<typename T>
concept MatrixNumerics = 
    std::is_same_v<T, double> ||
    std::is_same_v<T, float>;

#pragma pack(push, 1)
#pragma warning(push, 1)

///
/// @brief Matrix[2-4 x 2-4]D
///
template<MatrixNumerics T, size_t N>
struct MatrixBase {
    array<array<T, N>, N> Data {};

    // Accessors
    operator T *() {
        return &Data[0][0];
    }
    operator const T *() const {
        return &Data[0][0];
    }

    // Assignment
    T &At(size_t row, size_t col) {
        return Data[row][col];
    }
    const T &At(size_t row, size_t col) const {
        return Data[row][col];
    }


    ///
    /// Methods
    ///
    
    // Computes the determinant of the matrix (supports 2x2 and 3x3 matrices).
    T Determinant() const requires (N >= 2 && N <= 3) {
        if constexpr (N == 2) {
            return Data[0][0] * Data[1][1] - Data[0][1] * Data[1][0];
        } else if constexpr (N == 3) {
            return
                Data[0][0] * (Data[1][1] * Data[2][2] - Data[1][2] * Data[2][1]) -
                Data[0][1] * (Data[1][0] * Data[2][2] - Data[1][2] * Data[2][0]) +
                Data[0][2] * (Data[1][0] * Data[2][1] - Data[1][1] * Data[2][0]);
        }
    }

    // Constructs an identity matrix of size NxN.
    static MatrixBase Identity() {
        MatrixBase m;
        for (size_t i = 0; i < N; i++) {
            m.Data[i][i] = 1;
        }
        return m;
    }

    // Computes the inverse of the matrix (supports only 2x2 matrices).
    MatrixBase Inverse() const requires (N == 2) {
        T determinant = Determinant();
        //ValidateScalar(determinant);
        MatrixBase result;
        result.Data[0][0] = Data[1][1] / determinant;
        result.Data[0][1] = -Data[0][1] / determinant;
        result.Data[1][0] = -Data[1][0] / determinant;
        result.Data[1][1] = Data[0][0] / determinant;
        return result;
    }

    // Constructs a look-at matrix for 3D transformations (supports only 4x4 matrices).
    MatrixBase LookAt(const Vector3 &eye, const Vector3 &center, const Vector3 &up) requires (N == 4) {
        Vector3 f = (center - eye).Normalized();
        Vector3 r = up.Cross(f).Normalized();
        Vector3 u = f.Cross(r);

        MatrixBase result = Identity();
        result[0][0] = r.X; result[0][1] = r.Y; result[0][2] = r.Z;
        result[1][0] = u.X; result[1][1] = u.Y; result[1][2] = u.Z;
        result[2][0] = -f.X; result[2][1] = -f.Y; result[2][2] = -f.Z;
        result[3][0] = -r.Dot(eye);
        result[3][1] = -u.Dot(eye);
        result[3][2] = f.Dot(eye);

        return result;
    }

    // Projects a 3D point to 2D screen space (supports only 4x4 matrices).
    MatrixBase Project(const Vector3 &world, const MatrixBase &view, const MatrixBase &projection) requires (N == 4) {
        MatrixBase result;
        MatrixBase wvp = projection * view;
        Vector4 homogenousWorld(world.X, world.Y, world.Z, 1.0f);
        Vector4 projected = wvp * homogenousWorld;
        if (projected.w != 0) {
            result[0][0] = projected.X / projected.W;
            result[1][1] = projected.Y / projected.W;
            result[2][2] = projected.Z / projected.W;
        }
        return result;
    }

    // Unprojects a 2D screen point back to 3D world space (supports only 4x4 matrices).
    MatrixBase Reject(const Vector3 &screen, const MatrixBase &view, const MatrixBase &projection) requires (N == 4) {
        MatrixBase inverseProjection = projection.Inverse();
        MatrixBase inverseView = view.Inverse();
        Vector4 homogenousScreen(screen.X, screen.Y, screen.Z, 1.0f);
        Vector4 unprojected = inverseProjection * homogenousScreen;
        Vector4 world = inverseView * unprojected;
        MatrixBase result;
        result[0][0] = world.X / world.W;
        result[1][1] = world.Y / world.W;
        result[2][2] = world.Z / world.W;
        return result;
    }

    // Transforms a 2D vector (supports only 2x2 matrices).
    Vector2 Transform(const Vector2 &other) const requires (N == 2) {
        Vector2 result;
        result.X = Data[0][0] * other.X + Data[0][1] * other.Y;
        result.Y = Data[1][0] * other.X + Data[1][1] * other.Y;
        return result;
    }
    
    // Translate the matrix by a vector
    MatrixBase Translate(const Vector3 &translation) requires (N == 4) {
        Data[3][0] += translation.X;
        Data[3][1] += translation.Y;
        Data[3][2] += translation.Z;
        return *this;
    }

    // Rotate the matrix by a quaternion
    MatrixBase Rotate(const Quaternion &rotation) requires (N == 4) {
        float xx = rotation.x * rotation.x;
        float xy = rotation.x * rotation.y;
        float xz = rotation.x * rotation.z;
        float xw = rotation.x * rotation.w;
        float yy = rotation.y * rotation.y;
        float yz = rotation.y * rotation.z;
        float yw = rotation.y * rotation.w;
        float zz = rotation.z * rotation.z;
        float zw = rotation.z * rotation.w;

        MatrixBase result;
        result.Data[0][0] = 1 - 2 * (yy + zz);
        result.Data[0][1] = 2 * (xy - zw);
        result.Data[0][2] = 2 * (xz + yw);
        result.Data[1][0] = 2 * (xy + zw);
        result.Data[1][1] = 1 - 2 * (xx + zz);
        result.Data[1][2] = 2 * (yz - xw);
        result.Data[2][0] = 2 * (xz - yw);
        result.Data[2][1] = 2 * (yz + xw);
        result.Data[2][2] = 1 - 2 * (xx + yy);
        return *this * result;
    }

        // Rotate the matrix by a vector of Euler angles (rotation order: XYZ)
    MatrixBase Rotate(const Vector3 &euler_angles) requires (N == 4) {
        float cosX = std::cos(euler_angles.X);
        float sinX = std::sin(euler_angles.X);
        float cosY = std::cos(euler_angles.Y);
        float sinY = std::sin(euler_angles.Y);
        float cosZ = std::cos(euler_angles.Z);
        float sinZ = std::sin(euler_angles.Z);

        MatrixBase rotationX = {
            1,      0,      0,      0,
            0,      cosX,   -sinX,  0,
            0,      sinX,   cosX,   0,
            0,      0,      0,      1
        };

        MatrixBase rotationY = {
            cosY,   0,      sinY,  0,
            0,      1,      0,      0,
            -sinY,  0,      cosY,  0,
            0,      0,      0,      1,
        };

        MatrixBase rotationZ = {
            cosZ,   -sinZ,  0,      0,
            sinZ,   cosZ,   0,      0,
            0,      0,      1,      0,
            0,      0,      0,      1,
        };

        MatrixBase rotation = rotationZ * rotationY * rotationX;
        return *this * rotation;
    }

    // Scale the matrix by a vector
    MatrixBase Scale(const Vector3 &scale) requires (N == 4) {
        Data[0][0] *= scale.X;
        Data[1][1] *= scale.Y;
        Data[2][2] *= scale.Z;
        return *this;
    }

    // Computes the transpose of the matrix.
    MatrixBase Transpose() {
        MatrixBase result;
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                result.Data[j][i] = Data[i][j];
            }
        }
        return result;
    }

    // Arithmetic
    MatrixBase operator+(const MatrixBase &other) {
        MatrixBase result;
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                result.Data[i][j] = Data[i][j] + other.Data[i][j];
            }
        }
        return result;
    }
    MatrixBase operator-(const MatrixBase &other) {
        MatrixBase result;
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                result.Data[i][j] = Data[i][j] - other.Data[i][j];
            }
        }
        return result;
    }
    MatrixBase operator*(const MatrixBase &other) const {
        MatrixBase result;
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                for (size_t k = 0; k < N; k++) {
                    result.Data[i][j] += Data[i][k] * other.Data[k][j];
                }
            }
        }
        return result;
    }
    MatrixBase operator*(T scalar)  {
        MatrixBase result;
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                result.Data[i][j] = Data[i][j] * scalar;
            }
        }
        return result;
    }
    MatrixBase operator/(T scalar)  {
        ValidateScalar(scalar);
        MatrixBase result;
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                result.Data[i][j] = Data[i][j] / scalar;
            }
        }
        return result;
    }

    //template<MatrixNumerics T, size_t N>
    //friend MatrixBase<T, N> operator*(const MatrixBase<T, N> &matrix, T scalar) {
    //    MatrixBase<T, N> result;
    //    for (size_t i = 0; i < N; ++i) {
    //        for (size_t j = 0; j < N; ++j) {
    //            result.At(i, j) = matrix.At(i, j) * scalar;
    //        }
    //    }
    //    return result;
    //}
    //template<MatrixNumerics T, size_t N>
    //friend MatrixBase<T, N> operator/(const MatrixBase<T, N> &matrix, T scalar) {
    //    ValidateScalar(scalar);  // Ensure scalar is not zero
    //    MatrixBase<T, N> result;
    //    for (size_t i = 0; i < N; ++i) {
    //        for (size_t j = 0; j < N; ++j) {
    //            result.At(i, j) = matrix.At(i, j) / scalar;
    //        }
    //    }
    //    return result;
    //}
    
    // Assignment
    MatrixBase &operator+=(const MatrixBase &other) {
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                Data[i][j] += other[i][j];
            }
        }
        return *this;
    }
    MatrixBase &operator-=(const MatrixBase &other) {
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                Data[i][j] -= other[i][j];
            }
        }
        return *this;
    }
    MatrixBase &operator*=(const MatrixBase &other) {
        *this = *this * other;
        return *this;
    }
    MatrixBase &operator*=(T scalar) {
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                Data[i][j] *= scalar;
            }
        }
        return *this;
    }
    MatrixBase &operator/=(T scalar) {
        ValidateScalar(scalar);
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                Data[i][j] /= scalar;
            }
        }
        return *this;
    }

    // Negation
    MatrixBase operator-() const {
        MatrixBase result;
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                result[i][j] = -Data[i][j];
            }
        }
        return result;
    }

public:
    void ValidateScalar(T scalar) {
        if (std::abs(scalar) < std::numeric_limits<T>::epsilon()) {
            throw std::invalid_argument("Cannot divide by zero.");
        }
    }
};

#pragma warning(pop)
#pragma pack(pop)

using Matrix2 = MatrixBase<float, 2>;
using Matrix3 = MatrixBase<float, 3>;
using Matrix4 = MatrixBase<float, 4>;

using DMatrix2 = MatrixBase<double, 2>;
using DMatrix3 = MatrixBase<double, 3>;
using DMatrix4 = MatrixBase<double, 4>;


///
/// @brief Test Interface
///
export namespace Test {

void MatrixTests();

}

}

module: private;

namespace Ultra::Test {

void Compiler() {
    // Ensure that the sizes are correct
    static_assert( 32 == sizeof(MatrixBase<double,  2>), "MatrixBase<double>[2]: The type size should be 32 byte(s)!");
    static_assert( 72 == sizeof(MatrixBase<double,  3>), "MatrixBase<double>[3]: The type size should be 72 byte(s)!");
    static_assert(128 == sizeof(MatrixBase<double,  4>), "MatrixBase<double>[4]: The type size should be 128 byte(s)!");
    static_assert( 16 == sizeof(MatrixBase<float,   2>), "MatrixBase<float>[2]:  The type size should be 16 byte(s)!");
    static_assert( 36 == sizeof(MatrixBase<float,   3>), "MatrixBase<float>[3]:  The type size should be 36 byte(s)!");
    static_assert( 64 == sizeof(MatrixBase<float,   4>), "MatrixBase<float>[4]:  The type size should be 64 byte(s)!");

}

void MatrixTests() {
    // Preparation
    LogInfo("Testing Matrix");
    MatrixBase<float, 2> m1 { 1, 2, 3, 4 };
    MatrixBase<float, 2> m2 { 5, 6, 7, 8 };

    // Test basic properties
    AppAssert(m1.At(0, 0) == 1 && m1.At(0, 1) == 2 && m1.At(1, 0) == 3 && m1.At(1, 1) == 4, "Basic properties test failed for m1.");

    // Test arithmetic operations
    auto sum = m1 + m2;
    AppAssert(sum.At(0, 0) == 6 && sum.At(0, 1) == 8 && sum.At(1, 0) == 10 && sum.At(1, 1) == 12, "Addition test failed.");
    auto diff = m2 - m1;
    AppAssert(diff.At(0, 0) == 4 && diff.At(0, 1) == 4 && diff.At(1, 0) == 4 && diff.At(1, 1) == 4, "Subtraction test failed.");
    auto prod = m1 * m2;
    AppAssert(prod.At(0, 0) == 19 && prod.At(0, 1) == 22 && prod.At(1, 0) == 43 && prod.At(1, 1) == 50, "Matrix multiplication test failed.");

    // Test Transpose
    auto transposed = m1.Transpose();
    AppAssert(transposed.At(0, 0) == 1 && transposed.At(0, 1) == 3 && transposed.At(1, 0) == 2 && transposed.At(1, 1) == 4, "Transpose test failed.");

    // Test Determinant (2x2 matrix)
    float det = m1.Determinant();
    AppAssert(det == -2.0f, "Determinant test failed.");

    // Test Inverse (2x2 matrix)
    auto inverse = m1.Inverse();
    AppAssert(inverse.At(0, 0) == -2 && inverse.At(0, 1) == 1 && inverse.At(1, 0) == 1.5 && inverse.At(1, 1) == -0.5, "Inverse test failed.");

    // Test Identity
    auto identity = MatrixBase<float, 2>::Identity();
    AppAssert(identity.At(0, 0) == 1 && identity.At(0, 1) == 0 && identity.At(1, 0) == 0 && identity.At(1, 1) == 1, "Identity test failed.");

    // Test scalar multiplication and division
    //auto scalarMul = m1 * 2.0f;
    //AppAssert(scalarMul.At(0, 0) == 2 && scalarMul.At(0, 1) == 4 && scalarMul.At(1, 0) == 6 && scalarMul.At(1, 1) == 8, "Scalar multiplication test failed.");
    //auto scalarDiv = m1 / 2.0f;
    //AppAssert(scalarDiv.At(0, 0) == 0.5 && scalarDiv.At(0, 1) == 1 && scalarDiv.At(1, 0) == 1.5 && scalarDiv.At(1, 1) == 2, "Scalar division test failed.");

    LogInfo("~Testing Matrix");
}

}

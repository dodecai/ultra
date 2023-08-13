export module Ultra.Math.Vector;

import <numbers>;
import Ultra.Core;

///
/// @brief Vector
///
export namespace Ultra {

// Concepts
template<typename T>
concept VectorNumerics = 
    std::is_same_v<T, bool> ||
    std::is_same_v<T, double> ||
    std::is_same_v<T, float> ||
    std::is_same_v<T, int32_t> ||
    std::is_same_v<T, uint32_t>;

///
/// @brief Vector[2-4]D
///
template<VectorNumerics T, size_t N>
struct Vector {
    // Compile-Time Checks
    static_assert(N >= 2 && N <= 4, "Vector size must be between 2 and 4.");
    
    // Types
    struct EmptyType {};
    using T1 = typename std::conditional_t<N >= 1, T, EmptyType>; // Left for future use, maybe needed
    using T2 = typename std::conditional_t<N >= 2, T, EmptyType>;
    using T3 = typename std::conditional_t<N >= 3, T, EmptyType>;
    using T4 = typename std::conditional_t<N >= 4, T, EmptyType>;

    // Properties
    union {
        struct {
            T1 X;
            T2 Y;
            T3 Z;
            T4 W;
        };

        array<T, N> Data;
    };

    // Default
    Vector(const array<T, N> &data):
        Data(data) {
    }

    // Operators
    T &operator[](size_t index) {
        return Data.at(index);
    }
    const T &operator[](size_t index) const {
        return Data.at(index);
    }

    // Comparisons
    std::strong_ordering operator<=>(const Vector &other) const {
        for (size_t i = 0; i < N; i++) {
            if (auto result = Data[i] <=> other.Data[i]; result != 0) return result;
        }
        return std::strong_ordering::equal;
    }

    // Conversions
    operator string() {
        string result = "[ ";
        for (const auto &element : Data) {
            result += std::format("{}, ", element);
        }
        result = result.substr(0, result.length() - 2);
        result += " ]";
        return result;
    }

    // Methods
    T Angle(const Vector &other) {
        return std::acos(Dot(other) / (Length() * other.Length()));
    }
    T AngleInDegrees(const Vector &other) {
        return std::acos(Dot(other) / (Length() * other.Length())) * (180.0 / std::numbers::pi);
    }
    Vector Abs() {
        Vector result {};
        for (size_t i = 0; i < N; i++) {
            result[i] = std::abs(Data[i]);
        }
        return result;
    }
    Vector Cross(const Vector &other) const {
        if constexpr (N == 3) {
            return {
                Data[1] * other.Data[2] - Data[2] * other.Data[1],
                Data[2] * other.Data[0] - Data[0] * other.Data[2],
                Data[0] * other.Data[1] - Data[1] * other.Data[0]
            };
        } else {
            throw std::invalid_argument("Cross product is only defined for 3D vectors.");
        }
    }
    Vector Clamp(T minVal, T maxVal) const {
        Vector result = *this;
        for (size_t i = 0; i < N; i++) {
            result[i] = std::clamp(result[i], minVal, maxVal);
        }
        return result;
    }
    T Distance(const Vector &other) {
        return (*this - other).Length();
    }
    T Dot(const Vector &other) const {
        T result {};
        for (size_t i = 0; i < N; i++) {
            result += Data[i] * other.Data[i];
        }
        return result;
    }
    T Length() const {
        return std::sqrt(SquaredLength());
    }
    Vector Lerp(const Vector &other, T factor) const {
        return (*this * (1 - factor)) + (other * factor);
    }
    static Vector Max(const Vector &a, const Vector &b) {
        Vector result = a;
        for (size_t i = 0; i < N; i++) {
            result[i] = std::max(a[i], b[i]);
        }
        return result;
    }
    static Vector Min(const Vector &a, const Vector &b) {
        Vector result = a;
        for (size_t i = 0; i < N; i++) {
            result[i] = std::min(a[i], b[i]);
        }
        return result;
    }
    Vector Normalize() {
        if (!ValidateNonZero()) { throw std::invalid_argument("Cannot normalize the zero vector."); }
        Vector result = *this;
        T length = Length();
        for (size_t i = 0; i < N; i++) {
            result[i] /= length;
        }
        return result;
    }
    Vector Projection(const Vector &other) const {
        T squaredLength = other.SquaredLength();
        T dotProduct = Dot(other);
        return (dotProduct / squaredLength) * other;
    }
    Vector Pow(T exponent) const {
        Vector result = *this;
        for (size_t i = 0; i < N; i++) {
            result[i] = std::pow(result[i], exponent);
        }
        return result;
    }
    Vector Reflection(const Vector &normal) {
        return *this - 2 * Projection(normal);
    }
    Vector Reject(const Vector &other) const {
        return *this - Projection(other);
    }
    T SquaredDistance(const Vector &other) const {
        return (*this - other).SquaredLength();
    }
    T SquaredLength() const {
        T result {};
        for (size_t i = 0; i < N; i++) {
            result += Data[i] * Data[i];
        }
        return result;
    }

    // Operations
    Vector operator+(const Vector &other) const {
        Vector result = *this;
        result += other;
        return result;
    }
    Vector operator-(const Vector &other) const {
        Vector result = *this;
        result -= other;
        return result;
    }
    Vector operator*(const Vector &other) const {
        Vector result;
        for (int i = 0; i < N; i++) {
            result[i] = this->Data[i] * other.Data[i];
        }
        return result;
    }
    Vector operator*(const T &scalar) const {
        Vector result = *this;
        result *= scalar;
        return result;
    }
    Vector operator/(const T &scalar) const {
        if (!ValidateNonZero()) { throw std::invalid_argument("Cannot divide the zero vector."); }
        Vector result = *this;
        result /= scalar;
        return result;
    }
    Vector &operator+=(const Vector &other) {
        for (size_t i = 0; i < N; i++) {
            Data[i] += other.Data[i];
        }
        return *this;
    }
    Vector &operator-=(const Vector &other) {
        for (size_t i = 0; i < N; i++) {
            Data[i] -= other.Data[i];
        }
        return *this;
    }
    Vector &operator*=(const T &scalar) {
        for (size_t i = 0; i < N; i++) {
            Data[i] *= scalar;
        }
        return *this;
    }
    Vector &operator/=(const T &scalar) {
        if (!ValidateNonZero()) { throw std::invalid_argument("Cannot divide the zero vector."); }
        for (size_t i = 0; i < N; i++) {
            Data[i] /= scalar;
        }
        return *this;
    }

    friend Vector operator*(const T &scalar, const Vector &v) {
        return v * scalar;
    }

private:
    bool ValidateNonZero(T tolerance = std::numeric_limits<T>::epsilon()) const {
        return SquaredLength() > tolerance * tolerance;
    }
};

template<VectorNumerics T>
struct Vector2D: Vector<T, 2> {
    // Default
    Vector2D(T x = {}, T y = {}):
        Vector<T, 2>({ x, y }) {
    }
};

template <VectorNumerics T>
struct Vector3D : Vector<T, 3> {
    // Default
    Vector3D(T x = {}, T y = {}, T z = {}):
        Vector<T, 3>({ x, y, z }) {
    }

};

template<VectorNumerics T>
struct Vector4D: Vector<T, 4> {
    // Default
    Vector4D(T x = {}, T y = {}, T z = {}, T w = {}):
        Vector<T, 4>({ x, y, z, w }) {
    }
};

using Vector2Db = Vector2D<bool>;
using Vector3Db = Vector3D<bool>;
using Vector4Db = Vector4D<bool>;

using Vector2Dd = Vector2D<double>;
using Vector3Dd = Vector3D<double>;
using Vector4Dd = Vector4D<double>;

using Vector2Df = Vector2D<float>;
using Vector3Df = Vector3D<float>;
using Vector4Df = Vector4D<float>;

using Vector2Di = Vector2D<int32_t>;
using Vector3Di = Vector3D<int32_t>;
using Vector4Di = Vector4D<int32_t>;

using Vector2Du = Vector2D<uint32_t>;
using Vector3Du = Vector3D<uint32_t>;
using Vector4Du = Vector4D<uint32_t>;

void TestVector() {
    //// Create two 3D vectors for testing.
    //Vector<int, 3> v1({ 1, 2, 3 });
    //Vector<int, 3> v2({ 4, 5, 6 });

    //// Test basic properties
    //assert(v1[0] == 1 && v1[1] == 2 && v1[2] == 3);
    //assert(v1.X == 1 && v1.Y == 2 && v1.Z == 3);

    //// Test arithmetic operations
    //auto sum = v1 + v2;
    //assert(sum[0] == 5 && sum[1] == 7 && sum[2] == 9);

    //auto diff = v2 - v1;
    //assert(diff[0] == 3 && diff[1] == 3 && diff[2] == 3);

    //auto prod = v1 * 2;
    //assert(prod[0] == 2 && prod[1] == 4 && prod[2] == 6);

    //// Test Dot product
    //int dotProd = v1.Dot(v2);
    //assert(dotProd == 32);  // 1*4 + 2*5 + 3*6

    //// Test Cross product for 3D vectors
    //auto crossProd = v1.Cross(v2);
    //assert(crossProd[0] == -3 && crossProd[1] == 6 && crossProd[2] == -3);

    //// Test normalization (will use a 2D vector for easier computation)
    //Vector<double, 2> v3({ 3.0, 4.0 }); // A 3-4-5 right triangle
    //auto normalized = v3.Normalize();
    //double length = normalized.Length();
    //assert(length > 0.99 && length < 1.01);  // Close enough to 1 for floating point precision

    //// Test comparison
    //assert((v1 <=> v2) == std::strong_ordering::less);

    //// Test string conversion
    //std::string v1Str = static_cast<std::string>(v1);
    //assert(v1Str == "[ 1, 2, 3 ]");

    //std::cout << "All tests passed!" << std::endl;
}

}

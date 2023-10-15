export module Ultra.Math.Vector;

import Ultra.Core;
import Ultra.Logger;

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
#pragma warning(push)
#pragma warning(disable: 4201)
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
    constexpr explicit Vector(const T &value):
        Data({ value }) {
    }
    constexpr Vector(const array<T, N> &data):
        Data(data) {
    }
    constexpr Vector(std::initializer_list<T> initList) {
        std::copy(initList.begin(), initList.end(), Data.begin());
    }

    // Accessors
    T &operator[](size_t index) {
        return Data[index];
    }
    const T &operator[](size_t index) const {
        return Data[index];
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
        string result;
        result.reserve(N * 6);
        result = "[ ";
        for (const auto &element : Data) {
            result += std::format("{}, ", element);
        }
        result = result.substr(0, result.length() - 2);
        result += " ]";
        return result;
    }

    // Mutators
    void SetZero() {
        for (size_t i = 0; i < N; i++) {
            Data[i] = static_cast<T>(0);
        }
    }

    // Methods
    
    // Calculates the angle between this vector and another vector in radians.
    T Angle(const Vector &other) {
        return std::acos(Dot(other) / (Length() * other.Length()));
    }
    
    // Calculates the angle between this vector and another vector in degrees.
    T AngleInDegrees(const Vector &other) {
        return std::acos(Dot(other) / (Length() * other.Length())) * (180.0 / std::numbers::pi);
    }

    // Returns a vector with the absolute value of each component.
    Vector Abs() {
        Vector result {};
        for (size_t i = 0; i < N; i++) {
            result[i] = std::abs(Data[i]);
        }
        return result;
    }

    // Rounds up each component of this vector.
    Vector Ceil() const {
        Vector result;
        for (size_t i = 0; i < N; i++) {
            result.Data[i] = std::ceil(Data[i]);
        }
        return result;
    }

    // Calculates the cross product between this vector and another vector (only valid for 3D vectors).
    Vector Cross(const Vector &other) const requires (N == 3) {
        return {
            Data[1] * other.Data[2] - Data[2] * other.Data[1],
            Data[2] * other.Data[0] - Data[0] * other.Data[2],
            Data[0] * other.Data[1] - Data[1] * other.Data[0]
        };
    }

    // Clamps each component of the vector between given minimum and maximum values.
    Vector Clamp(T minVal, T maxVal) const {
        Vector result = *this;
        for (size_t i = 0; i < N; i++) {
            result[i] = std::clamp(result[i], minVal, maxVal);
        }
        return result;
    }
    
    // Calculates the distance between this vector and another vector.
    T Distance(const Vector &other) {
        return (*this - other).Length();
    }
    
    // Calculates the dot product between this vector and another vector.
    T Dot(const Vector &other) const {
        T result {};
        for (size_t i = 0; i < N; i++) {
            result += Data[i] * other.Data[i];
        }
        return result;
    }

    // Rounds down each component of this vector.
    Vector Floor() const {
        Vector result;
        for (size_t i = 0; i < N; i++) {
            result.Data[i] = std::floor(Data[i]);
        }
        return result;
    }

    // Returns the element-wise multiplication of two vectors.
    Vector Hadamard(const Vector &other) const {
        Vector result;
        for (size_t i = 0; i < N; i++) {
            result.Data[i] = Data[i] * other.Data[i];
        }
        return result;
    }

    // Converts a 3D vector to a homogeneous 4D vector
    Vector<T, 4> Homogenize() const requires (N == 3) {
        return Vector<T, 4>{Data[0], Data[1], Data[2], static_cast<T>(1)};
    }

    // Returns the length (magnitude) of the vector.
    T Length() const {
        return std::sqrt(SquaredLength());
    }

    // Linearly interpolates between this vector and another vector based on a given factor.
    Vector Lerp(const Vector &other, T factor) const {
        return (*this * (1 - factor)) + (other * factor);
    }

    // Returns a vector with the maximum value from each pair of components from two vectors.
    static Vector Max(const Vector &a, const Vector &b) {
        Vector result = a;
        for (size_t i = 0; i < N; i++) {
            result[i] = std::max(a[i], b[i]);
        }
        return result;
    }

    // Returns a vector with the minimum value from each pair of components from two vectors.
    static Vector Min(const Vector &a, const Vector &b) {
        Vector result = a;
        for (size_t i = 0; i < N; i++) {
            result[i] = std::min(a[i], b[i]);
        }
        return result;
    }
    
    // Checks if two vectors are approximately equal within a given tolerance.
    bool NearlyEqual(const Vector &other, T epsilon = std::numeric_limits<T>::epsilon()) const {
        for (size_t i = 0; i < N; i++) {
            if (std::abs(Data[i] - other.Data[i]) > epsilon) {
                return false;
            }
        }
        return true;
    }

    // Returns a normalized version of the vector.
    Vector Normalize() {
        if (!ValidateNonZero()) { throw std::invalid_argument("Cannot normalize the zero vector."); }
        Vector result = *this;
        T length = Length();
        for (size_t i = 0; i < N; i++) {
            result[i] /= length;
        }
        return result;
    }

    // Rounds each component of this vector to the nearest integer.
    Vector Round() const {
        Vector result;
        for (size_t i = 0; i < N; i++) {
            result.Data[i] = std::round(Data[i]);
        }
        return result;
    }

    // Returns a vector perpendicular to the current 2D vector.
    Vector Perpendicular() const requires (N == 2) {
        return Vector { -Data[1], Data[0] };
    }

    // Projects this vector onto another vector.
    Vector Projection(const Vector &other) const {
        T squaredLength = other.SquaredLength();
        T dotProduct = Dot(other);
        return (dotProduct / squaredLength) * other;
    }

    // Raises each component of the vector to a given power.
    Vector Pow(T exponent) const {
        Vector result = *this;
        for (size_t i = 0; i < N; i++) {
            result[i] = std::pow(result[i], exponent);
        }
        return result;
    }

     // Reflects the vector off a surface with the given normal.
    Vector Reflection(const Vector &normal) {
        return *this - 2 * Projection(normal);
    }

    // Returns the rejection of this vector from another vector.
    Vector Reject(const Vector &other) const {
        return *this - Projection(other);
    }

    // Returns the squared distance between this vector and another vector.
    T SquaredDistance(const Vector &other) const {
        return (*this - other).SquaredLength();
    }

    // Returns the squared length (magnitude) of the vector.
    T SquaredLength() const {
        T result {};
        for (size_t i = 0; i < N; i++) {
            result += Data[i] * Data[i];
        }
        return result;
    }
    
    // Returns the squared magnitude of this vector.
    T SquaredMagnitude() const {
        T sum = 0;
        for (size_t i = 0; i < N; i++) {
            sum += Data[i] * Data[i];
        }
        return sum;
    }

    // Checks if the vector is a unit vector.
    bool UnitVector(T epsilon = std::numeric_limits<T>::epsilon()) const {
        return std::abs(SquaredMagnitude() - 1) < epsilon;
    }

    // Negates this vector.
    Vector operator-() const {
        Vector result;
        for (size_t i = 0; i < N; i++) {
            result.Data[i] = -Data[i];
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
    Vector operator/(const Vector &other) const {
        if (!ValidateNonZero()) { throw std::invalid_argument("Cannot divide the zero vector."); }
        Vector result;
        for (int i = 0; i < N; i++) {
            result[i] = this->Data[i] / other.Data[i];
        }
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

template<size_t N>
struct Vector<bool, N> {
    // Compile-Time Checks
    static_assert(N >= 2 && N <= 4, "Vector size must be between 2 and 4.");

    // Properties
    array<bool, N> Data;

    // Default
    constexpr explicit Vector(const bool &value):
        Data({ value }) {
    }
    constexpr Vector(const array<bool, N> &data) :
        Data(data) {
    }

    // Accessors
    bool &operator[](size_t index) {
        return Data[index];
    }
    const bool &operator[](size_t index) const {
        return Data[index];
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
        string result;
        result.reserve(N * 6);
        result = "[ ";
        for (const auto &element : Data) {
            result += element ? "true, " : "false, ";
        }
        result = result.substr(0, result.length() - 2);
        result += " ]";
        return result;
    }
};
#pragma warning(pop)

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

export namespace Test {

void VectorTests();

}

}

module: private;

namespace Ultra::Test {

void VectorTests() {
    LogInfo("Testing Vector");

    // Create two 3D vectors for testing.
    Vector<int, 3> v1({ 1, 2, 3 });
    Vector<int, 3> v2({ 4, 5, 6 });

    // Test basic properties
    AppAssert(v1[0] == 1 && v1[1] == 2 && v1[2] == 3, "Basic properties test failed for v1.");
    AppAssert(v1.X == 1 && v1.Y == 2 && v1.Z == 3, "XYZ accessors test failed for v1.");

    // Test arithmetic operations
    auto sum = v1 + v2;
    AppAssert(sum[0] == 5 && sum[1] == 7 && sum[2] == 9, "Addition test failed.");

    auto diff = v2 - v1;
    AppAssert(diff[0] == 3 && diff[1] == 3 && diff[2] == 3, "Subtraction test failed.");

    auto prod = v1 * 2;
    AppAssert(prod[0] == 2 && prod[1] == 4 && prod[2] == 6, "Scalar multiplication test failed.");

    // Test Dot product
    int dotProd = v1.Dot(v2);
    AppAssert(dotProd == 32, "Dot product test failed.");  // 1*4 + 2*5 + 3*6

    // Test Cross product for 3D vectors
    auto crossProd = v1.Cross(v2);
    AppAssert(crossProd[0] == -3 && crossProd[1] == 6 && crossProd[2] == -3, "Cross product test failed.");

    // Test normalization (will use a 2D vector for easier computation)
    Vector<double, 2> v3({ 3.0, 4.0 }); // A 3-4-5 right triangle
    auto normalized = v3.Normalize();
    double length = normalized.Length();
    AppAssert(length > 0.99 && length < 1.01, "Normalization test failed.");  // Close enough to 1 for floating point precision

    // Test comparison
    AppAssert((v1 <=> v2) == std::strong_ordering::less, "Comparison test failed.");

    // Test string conversion
    std::string v1Str = static_cast<std::string>(v1);
    AppAssert(v1Str == "[ 1, 2, 3 ]", "String conversion test failed.");

    // Test Angle
    Vector<double, 2> v4({ 1.0, 0.0 });
    Vector<double, 2> v5({ 0.0, 1.0 });
    double angle = v4.Angle(v5);
    AppAssert(angle > 1.56 && angle < 1.58, "Angle test failed.");  // Close to pi/2 or 90 degrees

    // Test Abs
    Vector<int, 3> v6({ -1, -2, -3 });
    auto absVec = v6.Abs();
    AppAssert(absVec[0] == 1 && absVec[1] == 2 && absVec[2] == 3, "Absolute value test failed.");

    // Test Clamp
    Vector<int, 3> v7 = v1.Clamp(2, 3);
    AppAssert(v7[0] == 2 && v7[1] == 2 && v7[2] == 3, "Clamp test failed.");

    // Test Distance
    double dist = v3.Distance(Vector<double, 2>({ 0.0, 0.0 }));
    AppAssert(dist > 4.99 && dist < 5.01, "Distance test failed.");  // Close to 5

    // Test Projection
    auto proj = v4.Projection(v5);
    AppAssert(proj[0] == 0.0 && proj[1] == 0.0, "Projection test failed.");  // Should be the zero vector

    // Test Reflection
    Vector<double, 2> v8({ 1.0, 1.0 });
    auto reflect = v8.Reflection(v4);
    AppAssert(reflect[0] == -1.0 && reflect[1] == 1.0, "Reflection test failed.");

    LogInfo("~Testing Vector");
}

}

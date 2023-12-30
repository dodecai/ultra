module;

#include "Ultra/Core/Core.h"

export module Ultra.Math.Vector;

import Ultra.Core;
import Ultra.Logger;

///
/// @brief Vector[2-4]D
///
export namespace Ultra {

#pragma region Vector Data

///
/// @brief Concepts
///

// VectorTypes: [bool|double|float|int32|uint32]
template<typename T>
concept VectorNumerics =
std::is_same_v<T, bool> ||
    std::is_same_v<T, double> ||
    std::is_same_v<T, float> ||
    std::is_same_v<T, int32_t> ||
    std::is_same_v<T, uint32_t>;

// VectorSizes: [2|3|4]
template<size_t N>
concept VectorSizeRange = (N >= 2 && N <= 4);

///
/// @brief Enumerations
///

// Vector Aliases: [None|Coordinate|Normal|Rotation|TextureCoordinate]
enum class VectorAliases {
    Coordinate,
    Normal,
    Rotation,
    TextureCoordinate,
    None,
};

#pragma pack(push, 1)
#pragma warning(push, 1)
#pragma warning(disable: 4201)

///
/// @brief Vector Data: Prototype
/// @tparam T Vector Type: [bool|double|float|int32|uint32]
/// @tparam N Vector Size: [2|3|4]
/// @tparam A Vector Alias: [None|Coordinate|Normal|Rotation|TextureCoordinate]
///
template<VectorNumerics T, size_t N, VectorAliases A = VectorAliases::None>
    requires VectorSizeRange<N>
struct VectorData;

///
/// @brief Vector Data: None Specialization (simple array)
///
template<VectorNumerics T, size_t N>
struct VectorData<T, N, VectorAliases::None> {
    std::array<T, N> Data;
};

///
/// @brief Vector Data: Coordinates Specialization with additional aliases [X|Y|Z|W]
///
template<VectorNumerics T, size_t N>
struct VectorData<T, N, VectorAliases::Coordinate> {
    // Aliases
    using T1 = typename std::conditional_t<N >= 1, T, std::monostate>;
    using T2 = typename std::conditional_t<N >= 2, T, std::monostate>;
    using T3 = typename std::conditional_t<N >= 3, T, std::monostate>;
    using T4 = typename std::conditional_t<N >= 4, T, std::monostate>;

    // Properties
    union {
        std::array<T, N> Data;
        struct {
            CPP_NO_UNIQUE_ADDRESS T1 X;
            CPP_NO_UNIQUE_ADDRESS T2 Y;
            CPP_NO_UNIQUE_ADDRESS T3 Z;
            CPP_NO_UNIQUE_ADDRESS T4 W;
        };
    };
};

///
/// @brief Vector Data: Normal Specialization with additional aliases [NX|NY|NZ]
///
template<VectorNumerics T, size_t N>
    requires (N <= 3)
struct VectorData<T, N, VectorAliases::Normal> {
    // Aliases
    using T1 = typename std::conditional_t<N >= 1, T, std::monostate>;
    using T2 = typename std::conditional_t<N >= 2, T, std::monostate>;
    using T3 = typename std::conditional_t<N >= 3, T, std::monostate>;

    // Properties
    union {
        std::array<T, N> Data;
        struct {
            CPP_NO_UNIQUE_ADDRESS T1 NX;
            CPP_NO_UNIQUE_ADDRESS T2 NY;
            CPP_NO_UNIQUE_ADDRESS T3 NZ;
        };
    };
};

///
/// @brief Vector Data: Rotation Specialization with additional aliases [Pitch|Yaw|Roll]
///
template<VectorNumerics T, size_t N>
    requires (N == 3)
struct VectorData<T, N, VectorAliases::Rotation> {
    // Aliases
    using T1 = typename std::conditional_t<N >= 1, T, std::monostate>;
    using T2 = typename std::conditional_t<N >= 2, T, std::monostate>;
    using T3 = typename std::conditional_t<N >= 3, T, std::monostate>;

    // Properties
    union {
        std::array<T, N> Data;
        struct {
            CPP_NO_UNIQUE_ADDRESS T1 Pitch;
            CPP_NO_UNIQUE_ADDRESS T2 Yaw;
            CPP_NO_UNIQUE_ADDRESS T3 Roll;
        };
    };
};

///
/// @brief Vector Data: Texture Coordinates Specialization with additional aliases [U|V|W|Q]
///
template<VectorNumerics T, size_t N>
    requires (N >= 2 && N <= 3)
struct VectorData<T, N, VectorAliases::TextureCoordinate> {
    // Aliases
    using T1 = typename std::conditional_t<N >= 1, T, std::monostate>;
    using T2 = typename std::conditional_t<N >= 2, T, std::monostate>;
    using T3 = typename std::conditional_t<N >= 3, T, std::monostate>;

    // Properties
    union {
        std::array<T, N> Data;
        struct {
            CPP_NO_UNIQUE_ADDRESS T1 U;
            CPP_NO_UNIQUE_ADDRESS T2 V;
            CPP_NO_UNIQUE_ADDRESS T3 W;
        };
    };
};

#pragma pack(pop)
#pragma warning(pop)

#pragma endregion

#pragma region Vector Base

///
/// @brief VectorBase: Prototype
/// | This is the vector base aggregation, which is used to implement the Vector class or similar arithmetic classes.
///
template<VectorNumerics T, size_t N, VectorAliases A = VectorAliases::None>
struct VectorBase;

///
/// @brief VectorBase: Specialization
/// | This is the specialization for nearly all types (except booleans), which offers everything needed.
/// @note Not used aliases are set to std::monostate and don't require any storage.
///
template<VectorNumerics T, size_t N, VectorAliases A>
struct VectorBase: public VectorData<T, N, A> {
    // Import VectorData
    using VectorData<T, N, A>::Data;

    // Accessors
    operator T *() {
        return &Data[0];
    }
    operator const T *() const {
        return &Data[0];
    }
    T &operator[](size_t index) {
        return Data[index];
    }
    const T &operator[](size_t index) const {
        return Data[index];
    }

    // Comparison
    auto operator<=>(const VectorBase &other) const {
        for (size_t i = 0; i < N; ++i) {
            if (auto cmp = std::compare_strong_order_fallback(Data[i], other.Data[i]); cmp != 0) {
                return cmp;
            }
        }
        return std::strong_ordering::equal;
    }
    auto operator==(const VectorBase &other) const {
        return (*this <=> other) == 0;
    }

    // Conversion
    template <VectorNumerics U>
    array<U, N> ToArray() const {
        array<U, N> result;
        std::transform(Data.begin(), Data.end(), result.begin(), [](bool b) {
            return static_cast<U>(b);
        });
        return result;
    }

    // Arithmetic
    auto operator+(const VectorBase &other) {
        VectorBase result = *this;
        return result += other;
    }
    auto operator-(const VectorBase &other) {
        VectorBase result = *this;
        return result -= other;
    }
    auto operator*(const VectorBase &other) {
        VectorBase result = *this;
        return result *= other;
    }
    auto operator/(const VectorBase &other) {
        VectorBase result = *this;
        return result /= other;
    }
    template<VectorNumerics S = T>
    auto operator+(S scalar) {
        VectorBase result = *this;
        return result += scalar;
    }
    template<VectorNumerics S = T>
    auto operator-(S scalar) {
        VectorBase result = *this;
        return result -= scalar;
    }
    template<VectorNumerics S = T>
    auto operator*(S scalar) {
        VectorBase result = *this;
        return result *= scalar;
    }
    template<VectorNumerics S = T>
    auto operator/(S scalar) {
        VectorBase result = *this;
        return result /= scalar;
    }

    // - Allow scalar additions and multiplications from the left and right
    template<VectorNumerics S = T>
    friend VectorBase operator+(S scalar, const VectorBase &v) {
        VectorBase result = v;
        for (size_t i = 0; i < N; i++) { result.Data[i] += scalar; }
        return result;
    }
    template<VectorNumerics S = T>
    friend VectorBase operator*(S scalar, const VectorBase &v) {
        VectorBase result = v;
        for (size_t i = 0; i < N; i++) { result.Data[i] *= scalar; }
        return result;
    }

    // Assignment
    VectorBase &operator=(const VectorBase &) = default;
    VectorBase &operator+=(const VectorBase &other) {
        for (size_t i = 0; i < N; i++) { Data[i] += other.Data[i]; }
        return *this;
    }
    VectorBase &operator-=(const VectorBase &other) {
        for (size_t i = 0; i < N; i++) { Data[i] -= other.Data[i]; }
        return *this;
    }
    VectorBase &operator*=(const VectorBase &other) {
        for (size_t i = 0; i < N; i++) { Data[i] *= other.Data[i]; }
        return *this;
    }
    VectorBase &operator/=(const VectorBase &other) {
        static constexpr T infinity = std::numeric_limits<T>::infinity() ? std::numeric_limits<T>::infinity() : std::numeric_limits<T>::max();
        for (size_t i = 0; i < N; i++) {
            auto value = Data[i] / other.Data[i];
            Data[i] = std::isfinite(value) ? value : infinity;
        }
        return *this;
    }
    template<VectorNumerics S = T>
    VectorBase &operator+=(S scalar) {
        for (size_t i = 0; i < N; i++) { Data[i] += scalar; }
        return *this;
    }
    template<VectorNumerics S = T>
    VectorBase &operator-=(S scalar) {
        for (size_t i = 0; i < N; i++) { Data[i] -= scalar; }
        return *this;
    }
    template<VectorNumerics S = T>
    VectorBase &operator*=(S scalar) {
        for (size_t i = 0; i < N; i++) { Data[i] *= scalar; }
        return *this;
    }
    template<VectorNumerics S = T>
    VectorBase &operator/=(S scalar) {
        static constexpr T infinity = std::numeric_limits<T>::infinity() ? std::numeric_limits<T>::infinity() : std::numeric_limits<T>::max();
        for (size_t i = 0; i < N; i++) {
            auto value = Data[i] / scalar;
            Data[i] = std::isfinite(value) ? value : infinity;
        }
        return *this;
    }

    // Iterators
    auto begin() { return Data.begin(); }
    auto end() { return Data.end(); }
    auto rbegin() { return Data.rbegin(); }
    auto rend() { return Data.rend(); }

    auto begin() const { return Data.begin(); }
    auto end() const { return Data.end(); }
    auto rbegin() const { return Data.rbegin(); }
    auto rend() const { return Data.rend(); }

    // Negates this vector.
    auto operator-() const {
        VectorBase result;
        for (size_t i = 0; i < N; i++) { result.Data[i] = -Data[i]; }
        return result;
    }

    ///
    /// Methods
    ///

    // Calculates the angle between this vector and another vector in radians.
    inline T Angle(const VectorBase &other) const requires (N >= 2) {
        return std::acos(Dot(other) / (Length() * other.Length()));
    }

    // Calculates the angle between this vector and another vector in degrees.
    inline T AngleInDegrees(const VectorBase &other) const requires (N >= 2) {
        static constexpr T degreeConversionFactor = static_cast<T>(180.0 / std::numbers::pi);
        return std::acos(Dot(other) / (Length() * other.Length())) * degreeConversionFactor;
    }

    // Returns a vector with the absolute value of each component.
    inline VectorBase Abs() const {
        VectorBase result {};
        for (size_t i = 0; i < N; i++) {
            result[i] = std::abs(Data[i]);
        }
        return result;
    }

    // Rounds up each component of this vector.
    inline VectorBase Ceil() const {
        VectorBase result;
        for (size_t i = 0; i < N; i++) {
            result.Data[i] = std::ceil(Data[i]);
        }
        return result;
    }

    // Calculates the cross product between this vector and another vector (only valid for 3D vectors).
    inline VectorBase Cross(const VectorBase &other) const requires (N == 3) {
        return {
            Data[1] * other.Data[2] - Data[2] * other.Data[1],
            Data[2] * other.Data[0] - Data[0] * other.Data[2],
            Data[0] * other.Data[1] - Data[1] * other.Data[0]
        };
    }

    // Clamps each component of the vector between given minimum and maximum values.
    inline VectorBase Clamp(T minVal, T maxVal) const {
        VectorBase result = *this;
        for (size_t i = 0; i < N; i++) {
            result[i] = std::clamp(result[i], minVal, maxVal);
        }
        return result;
    }

    // Calculates the distance between this vector and another vector.
    inline T Distance(const VectorBase &other) {
        return (*this - other).Length();
    }

    // Calculates the dot product between this vector and another vector.
    inline T Dot(const VectorBase &other) const {
        T result {};
        for (size_t i = 0; i < N; i++) {
            result += Data[i] * other.Data[i];
        }
        return result;
    }

    // Rounds down each component of this vector.
    inline VectorBase Floor() const {
        VectorBase result;
        for (size_t i = 0; i < N; i++) {
            result.Data[i] = std::floor(Data[i]);
        }
        return result;
    }

    // Returns the element-wise multiplication of two vectors.
    inline VectorBase Hadamard(const VectorBase &other) const {
        VectorBase result;
        for (size_t i = 0; i < N; i++) {
            result.Data[i] = Data[i] * other.Data[i];
        }
        return result;
    }

    // Converts a 3D vector to a homogeneous 4D vector
    inline VectorBase<T, 4> Homogenize() const requires (N == 3) {
        return VectorBase<T, 4>{Data[0], Data[1], Data[2], static_cast<T>(1)};
    }

    // Returns the length (magnitude) of the vector.
    inline T Length() const {
        return std::sqrt(SquaredLength());
    }

    // Linearly interpolates between this vector and another vector based on a given factor.
    inline VectorBase Lerp(const VectorBase &other, T factor) const {
        return (*this * (1 - factor)) + (other * factor);
    }

    // Returns a vector with the maximum value from each pair of components from two vectors.
    inline VectorBase Max(const VectorBase &a, const VectorBase &b) {
        VectorBase result = a;
        for (size_t i = 0; i < N; i++) {
            result[i] = std::max(a[i], b[i]);
        }
        return result;
    }

    // Returns a vector with the minimum value from each pair of components from two vectors.
    inline VectorBase Min(const VectorBase &a, const VectorBase &b) {
        VectorBase result = a;
        for (size_t i = 0; i < N; i++) {
            result[i] = std::min(a[i], b[i]);
        }
        return result;
    }

    // Checks if two vectors are approximately equal within a given tolerance.
    inline bool NearlyEqual(const VectorBase &other, T epsilon = std::numeric_limits<T>::epsilon()) const {
        for (size_t i = 0; i < N; i++) {
            if (std::abs(Data[i] - other.Data[i]) > epsilon) {
                return false;
            }
        }
        return true;
    }

    // Returns a normalized version of the vector.
    inline VectorBase Normalize() const {
        VectorBase result = *this;
        T length = Length();
        for (size_t i = 0; i < N; i++) {
            result[i] /= length;
        }
        return result;
    }

    // Rounds each component of this vector to the nearest integer.
    inline VectorBase Round() const {
        VectorBase result;
        for (size_t i = 0; i < N; i++) {
            result.Data[i] = std::round(Data[i]);
        }
        return result;
    }

    // Returns a vector perpendicular to the current 2D vector.
    inline VectorBase Perpendicular() const requires (N == 2) {
        return VectorBase { -Data[1], Data[0] };
    }

    // Projects this vector onto another vector.
    inline VectorBase Projection(const VectorBase &other) const requires (N >= 2) {
        T squaredLength = other.SquaredLength();
        T dotProduct = Dot(other);
        return (dotProduct / squaredLength) * other;
    }

    // Raises each component of the vector to a given power.
    inline VectorBase Pow(T exponent) const {
        VectorBase result = *this;
        for (size_t i = 0; i < N; i++) {
            result[i] = std::pow(result[i], exponent);
        }
        return result;
    }

     // Reflects the vector off a surface with the given normal.
    inline VectorBase Reflection(const VectorBase &normal) {
        return *this - 2 * Projection(normal);
    }

    // Returns the rejection of this vector from another vector.
    inline VectorBase Reject(const VectorBase &other) const requires(N >= 2) {
        return *this - Projection(other);
    }

    // Returns the squared distance between this vector and another vector.
    inline T SquaredDistance(const VectorBase &other) const {
        return (*this - other).SquaredLength();
    }

    // Returns the squared length (magnitude) of the vector.
    inline T SquaredLength() const {
        T result {};
        for (size_t i = 0; i < N; i++) {
            result += Data[i] * Data[i];
        }
        return result;
    }

    // Checks if the vector is a unit vector.
    inline bool UnitVector(T epsilon = std::numeric_limits<T>::epsilon()) const {
        return std::abs(SquaredLength() - 1) < epsilon;
    }

    // Resets the vector to zero.
    inline void Zero() {
        std::fill(Data.begin(), Data.end(), static_cast<T>(0));
    }
};

///
/// @brief Vector: Boolean Specialization
/// | Due to the nature of booleans, this specialization has no arithmetic operators or methods.
///
template<size_t N>
struct VectorBase<bool, N, VectorAliases::None>: public VectorData<bool, N, VectorAliases::None> {
    // Import VectorData
    using VectorData<bool, N, VectorAliases::None>::Data;

    // Accessors
    operator bool *() {
        return &Data[0];
    }
    operator const bool *() const {
        return &Data[0];
    }
    bool operator[](size_t index) {
        return Data[index];
    }
    bool operator[](size_t index) const {
        return Data[index];
    }

    // Comparison
    auto operator<=>(const VectorBase &other) const {
        for (size_t i = 0; i < N; ++i) {
            if (auto cmp = std::compare_strong_order_fallback(Data[i], other.Data[i]); cmp != 0) {
                return cmp;
            }
        }
        return std::strong_ordering::equal;
    }
    auto operator==(const VectorBase &other) const {
        return (*this <=> other) == 0;
    }

    // Conversion
    template <VectorNumerics U>
    array<U, N> ToArray() const {
        array<U, N> result;
        std::transform(Data.begin(), Data.end(), result.begin(), [](bool b) {
            return static_cast<U>(b);
        });
        return result;
    }

    // Iterators
    auto begin() { return Data.begin(); }
    auto end() { return Data.end(); }
    auto rbegin() { return Data.rbegin(); }
    auto rend() { return Data.rend(); }

    auto begin() const { return Data.begin(); }
    auto end() const { return Data.end(); }
    auto rbegin() const { return Data.rbegin(); }
    auto rend() const { return Data.rend(); }
};

#pragma endregion

///
/// @brief Aliases
///

using Direction2D = VectorBase<float, 2, VectorAliases::Coordinate>;
using Direction3D = VectorBase<float, 3, VectorAliases::Coordinate>;

using Normal2D = VectorBase<float, 2, VectorAliases::Normal>;
using Normal3D = VectorBase<float, 3, VectorAliases::Normal>;

using Position2D = VectorBase<float, 2, VectorAliases::Coordinate>;
using Position3D = VectorBase<float, 3, VectorAliases::Coordinate>;
using Position4D = VectorBase<float, 4, VectorAliases::Coordinate>;

//using Rotation2D = VectorBase<float, 1, VectorAliases::None>;
using Rotation3D = VectorBase<float, 3, VectorAliases::Rotation>;

using TextureCoord2D = VectorBase<float, 2, VectorAliases::TextureCoordinate>;
using TextureCoord3D = VectorBase<float, 3, VectorAliases::TextureCoordinate>;

using Vector2 = VectorBase<float, 2, VectorAliases::Coordinate>;
using Vector3 = VectorBase<float, 3, VectorAliases::Coordinate>;
using Vector4 = VectorBase<float, 4, VectorAliases::Coordinate>;

///
/// @brief Test Interface
///
export namespace Test {

void VectorTests();

}

}

///
/// @brief Conversions
///

namespace std {

template<Ultra::VectorNumerics T, size_t N, Ultra::VectorAliases A>
string to_string(const Ultra::VectorBase<T, N, A> &vector) {
    string result;
    result.reserve(N * 6);
    result = "[ ";
    for (const auto &element : vector) {
        result += std::format("{}, ", element);
    }
    result = result.substr(0, result.length() - 2);
    result += " ]";
    return result;
}

template<Ultra::VectorNumerics T, size_t N, Ultra::VectorAliases A>
struct std::formatter<Ultra::VectorBase<T, N, A>> {
    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();
    }

    auto format(const Ultra::VectorBase<T, N, A> &vector, format_context &ctx) {
        ostringstream stream;
        stream << "[ ";
        for (size_t i = 0; i < N; ++i) {
            stream << vector[i] << (i < N - 1 ? ", " : " ");
        }
        stream << "]";
        return format_to(ctx.out(), "{}", stream.str());
    }
};

}

///
/// @brief Tests
///

module: private;

namespace Ultra::Test {

///
/// @brief: These tests are executed during the compilation phase, so no need to call them.
///
void Compiler() {
    // Ensure that the sizes are correct
    static_assert( 2 == sizeof(VectorBase<bool,    2>), "VectorBase<bool>[2]:   The type size should be 2 byte(s)!");
    static_assert( 3 == sizeof(VectorBase<bool,    3>), "VectorBase<bool>[3]:   The type size should be 3 byte(s)!");
    static_assert( 4 == sizeof(VectorBase<bool,    4>), "VectorBase<bool>[4]:   The type size should be 4 byte(s)!");
    static_assert(16 == sizeof(VectorBase<double,  2>), "VectorBase<double>[2]: The type size should be 16 bytes(s)!");
    static_assert(24 == sizeof(VectorBase<double,  3>), "VectorBase<double>[3]: The type size should be 24 bytes(s)!");
    static_assert(32 == sizeof(VectorBase<double,  4>), "VectorBase<double>[4]: The type size should be 32 bytes(s)!");
    static_assert( 8 == sizeof(VectorBase<float,   2>), "VectorBase<float>[2]:  The type size should be 8 bytes(s)!");
    static_assert(12 == sizeof(VectorBase<float,   3>), "VectorBase<float>[3]:  The type size should be 12 bytes(s)!");
    static_assert(16 == sizeof(VectorBase<float,   4>), "VectorBase<float>[4]:  The type size should be 16 bytes(s)!");
    static_assert( 8 == sizeof(VectorBase<int32_t, 2>), "VectorBase<int32>[2]:  The type size should be 8 byte(s)!");
    static_assert(12 == sizeof(VectorBase<int32_t, 3>), "VectorBase<int32>[3]:  The type size should be 12 byte(s)!");
    static_assert(16 == sizeof(VectorBase<int32_t, 4>), "VectorBase<int32>[4]:  The type size should be 16 byte(s)!");
    static_assert( 8 == sizeof(VectorBase<uint32_t,2>), "VectorBase<uint32>[2]: The type size should be 8 bytes(s)!");
    static_assert(12 == sizeof(VectorBase<uint32_t,3>), "VectorBase<uint32>[3]: The type size should be 12 bytes(s)!");
    static_assert(16 == sizeof(VectorBase<uint32_t,4>), "VectorBase<uint32>[4]: The type size should be 16 bytes(s)!");
}

///
/// @brief Vector Tests
///
void VectorTests() {
    // Preparation
    LogInfo("Testing Vector");
    VectorBase<int, 3, VectorAliases::Coordinate> v0 { 1, 2, 3 };
    VectorBase<int, 3> v1({ 1, 2, 3 });
    VectorBase<int, 3> v2({ 4, 5, 6 });

    // Test basic properties
    AppAssert(v0[0] == 1 && v0[1] == 2 && v0[2] == 3, "Basic properties test failed for v0.");
    AppAssert(v0.X == 1 && v0.Y == 2 && v0.Z == 3, "XYZ accessors test failed for vo.");

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
    VectorBase<double, 2> v3({ 3.0, 4.0 }); // A 3-4-5 right triangle
    auto normalized = v3.Normalize();
    double length = normalized.Length();
    AppAssert(length > 0.99 && length < 1.01, "Normalization test failed.");  // Close enough to 1 for floating point precision

    // Test comparison
    AppAssert((v1 <=> v2) == std::strong_ordering::less, "Comparison test failed.");

    // Test string conversion
    //std::string v1Str = static_cast<std::string>(v1);
    //AppAssert(v1Str == "[ 1, 2, 3 ]", "String conversion test failed.");

    // Test Angle
    VectorBase<double, 2> v4({ 1.0, 0.0 });
    VectorBase<double, 2> v5({ 0.0, 1.0 });
    double angle = v4.Angle(v5);
    AppAssert(angle > 1.56 && angle < 1.58, "Angle test failed.");  // Close to pi/2 or 90 degrees

    // Test Abs
    VectorBase<int, 3> v6({ -1, -2, -3 });
    auto absVec = v6.Abs();
    AppAssert(absVec[0] == 1 && absVec[1] == 2 && absVec[2] == 3, "Absolute value test failed.");

    // Test Clamp
    VectorBase<int, 3> v7 = v1.Clamp(2, 3);
    AppAssert(v7[0] == 2 && v7[1] == 2 && v7[2] == 3, "Clamp test failed.");

    // Test Distance
    double dist = v3.Distance(VectorBase<double, 2>({ 0.0, 0.0 }));
    AppAssert(dist > 4.99 && dist < 5.01, "Distance test failed.");  // Close to 5

    // Test Projection
    auto proj = v4.Projection(v5);
    AppAssert(proj[0] == 0.0 && proj[1] == 0.0, "Projection test failed.");  // Should be the zero vector

    // Test Reflection
    VectorBase<double, 2> v8({ 1.0, 1.0 });
    auto reflect = v8.Reflection(v4);
    AppAssert(reflect[0] == -1.0 && reflect[1] == 1.0, "Reflection test failed.");


    VectorBase<bool, 2> bool2 = { true, false };
    VectorBase<bool, 3> bool3 = { true, false, true };
    VectorBase<bool, 4> bool4 = { true, false, true, false };

    Position3D xyz = { 0.5f, 0.5f, 0.5f };
    Position4D xyzw = { 0.5f, 0.5f, 0.5f, 0.5f };

    Direction2D dir2 = { 0.5f, 0.5f };
    Direction3D dir3 = { 0.5f, 0.5f, 0.5f };

    Normal2D norm2 = { 0.5f, 0.5f };
    Normal3D norm3 = { 0.5f, 0.5f, 0.5f };

    Position2D start { 0.5f, 0.5f };
    Position3D end { 0.5f, 0.5f, 0.5f };

    TextureCoord2D uv = { 0.5f, 0.5f };
    TextureCoord3D uvw = { 0.5f, 0.5f, 0.5f };

    auto converted = xyzw.ToArray<double>();

    // Test basic properties
    AppAssert(bool2 == VectorBase<bool, 2> { true, false }, "Comparision test failed for boolean!");
    xyzw *= 0.25f * 2;
    AppAssert(xyzw == Position4D { 0.25f, 0.25f, 0.25f, 0.25f }, "Comparision test failed for Position4D!");

    auto boom = start / 0;
    auto boomagain = end / 0.00000000f;

    LogInfo("Bool1: {}", std::to_string(bool2));
    LogInfo("Bool2: {}", std::to_string(bool3));
    LogInfo("Bool3: {}", std::to_string(bool4));
    LogInfo("Direction2D:{}", std::to_string(dir2));
    LogInfo("Direction3D:{}", std::to_string(dir3));
    LogInfo("Normal2D:   {}", std::to_string(norm2));
    LogInfo("Normal3D:   {}", std::to_string(norm3));
    LogInfo("Position2D: {}", std::to_string(start));
    LogInfo("Position3D: {}", std::to_string(end));
    LogInfo("Position3D:  {}", std::to_string(xyz));
    LogInfo("Position4D:  {}", std::to_string(xyzw));
    LogInfo("TextureCoord2D: {}", std::to_string(uv));
    LogInfo("TextureCoord3D: {}", std::to_string(uvw));


    LogInfo("~Testing Vector");
}

}

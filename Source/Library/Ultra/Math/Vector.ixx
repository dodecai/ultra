export module Ultra.Math.Vector;

import <numbers>;
import Ultra.Core;

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
    static_assert(N >= 2 && N <= 4, "Vector size must be between 2 and 4.");
    array<T, N> Data {};

    // Assignment
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
    T AngleBetween(const Vector &other) {
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
    T Length() {
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
    T x, y;

    // Constructors
    Vector2D(T x_val = {}, T y_val = {}): x(x_val), y(y_val) {}
    Vector2D &operator=(const Vector2D &other) {
        x = other.x;
        y = other.y;
        return *this;
    }

    // Accessors
    T &operator[](size_t index) {
        switch (index) {
            case 0: return x;
            case 1: return y;
            default:
                throw std::out_of_range("Vector2D index out of range.");
        }
    }
    const T &operator[](size_t index) const {
        switch (index) {
            case 0: return x;
            case 1: return y;
            default:
                throw std::out_of_range("Vector2D index out of range.");
        }
    }

};

template <VectorNumerics T>
struct Vector3D : Vector<T, 3> {
    T x, y, z;

    // Constructors
    Vector3D(T x_val = {}, T y_val = {}, T z_val = {}): x(x_val), y(y_val), z(z_val) {}
    Vector3D &operator=(const Vector3D &other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    template <typename T>
    Vector3D(const Vector<T, 3> &other)
        : x(static_cast<float>(other[0])),
        y(static_cast<float>(other[1])),
        z(static_cast<float>(other[2])) {
    }

    // Accessors
    T &operator[](size_t index) {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default:
                throw std::out_of_range("Vector3D index out of range.");
        }
    }
    const T &operator[](size_t index) const {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default:
                throw std::out_of_range("Vector3D index out of range.");
        }
    }

    Vector3D operator+(const Vector3D &other) const {
        return Vector3D(x + other.x, y + other.y);
    }

    Vector3D operator*(const Vector3D &other) const {
        return Vector3D(x * other.x, y * other.y);
    }

};

template<VectorNumerics T>
struct Vector4D: Vector<T, 4> {
    T x, y, z, w;

    // Constructors
    Vector4D(T x_val = {}, T y_val = {}, T z_val = {}, T w_val = {}): x(x_val), y(y_val), z(z_val), w(w_val) {}
    Vector4D &operator=(const Vector4D &other) {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
        return *this;
    }

    // Accessors
    T &operator[](size_t index) {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return 2;
            default:
                throw std::out_of_range("Vector3D index out of range.");
        }
    }
    const T &operator[](size_t index) const {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default:
                throw std::out_of_range("Vector3D index out of range.");
        }
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

}

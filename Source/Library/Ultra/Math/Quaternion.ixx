export module Ultra.Math.Quaternion;

import Ultra.Core;
import Ultra.Logger;
import Ultra.Math.Vector;

export namespace Ultra {

///
/// @brief Quaternion
///
template<typename T>
    requires std::is_floating_point_v<T>
struct QuaternionBase {
    // Properties
    union {
        array<T, 4> Data;
        struct {
            T w, x, y, z;
        };
    };

    // Constructors
    QuaternionBase(): w(1), x(0), y(0), z(0) {}
    QuaternionBase(T w, T x, T y, T z): w(w), x(x), y(y), z(z) {}
    QuaternionBase(const Vector3 &source) {
        T pitch = source.X * 0.5;
        T yaw = source.Y * 0.5;
        T roll = source.Z * 0.5;

        T cp = std::cos(pitch);
        T sp = std::sin(pitch);
        T cy = std::cos(yaw);
        T sy = std::sin(yaw);
        T cr = std::cos(roll);
        T sr = std::sin(roll);

        w = cp * cy * cr + sp * sy * sr;
        x = sp * cy * cr - cp * sy * sr;
        y = cp * sy * cr + sp * cy * sr;
        z = cp * cy * sr - sp * sy * cr;
    }
    //Quaternion(const MatrixBase<T, 4> &source) {
    //    T trace = source.At(0, 0) + source.At(1, 1) + source.At(2, 2);
    //    if (trace > 0) {
    //        T s = 0.5 / std::sqrt(trace + 1.0);
    //        w = 0.25 / s;
    //        x = (source.At(2, 1) - source.At(1, 2)) * s;
    //        y = (source.At(0, 2) - source.At(2, 0)) * s;
    //        z = (source.At(1, 0) - source.At(0, 1)) * s;
    //    } else {
    //        if (source.At(0, 0) > source.At(1, 1) && source.At(0, 0) > source.At(2, 2)) {
    //            T s = 2.0 * std::sqrt(1.0 + source.At(0, 0) - source.At(1, 1) - source.At(2, 2));
    //            w = (source.At(2, 1) - source.At(1, 2)) / s;
    //            x = 0.25 * s;
    //            y = (source.At(0, 1) + source.At(1, 0)) / s;
    //            z = (source.At(0, 2) + source.At(2, 0)) / s;
    //        } else if (source.At(1, 1) > source.At(2, 2)) {
    //            T s = 2.0 * std::sqrt(1.0 + source.At(1, 1) - source.At(0, 0) - source.At(2, 2));
    //            w = (source.At(0, 2) - source.At(2, 0)) / s;
    //            x = (source.At(0, 1) + source.At(1, 0)) / s;
    //            y = 0.25 * s;
    //            z = (source.At(1, 2) + source.At(2, 1)) / s;
    //        } else {
    //            T s = 2.0 * std::sqrt(1.0 + source.At(2, 2) - source.At(0, 0) - source.At(1, 1));
    //            w = (source.At(1, 0) - source.At(0, 1)) / s;
    //            x = (source.At(0, 2) + source.At(2, 0)) / s;
    //            y = (source.At(1, 2) + source.At(2, 1)) / s;
    //            z = 0.25 * s;
    //        }
    //    }
    //}

    // Comparison
    auto operator<=>(const QuaternionBase &other) const {
        if (auto cmp = w <=> other.w; cmp != 0) return cmp;
        if (auto cmp = x <=> other.x; cmp != 0) return cmp;
        if (auto cmp = y <=> other.y; cmp != 0) return cmp;
        return z <=> other.z;
    }
    auto operator==(const QuaternionBase &other) const {
        return (*this <=> other) == 0;
    }

    // Conversions
    /*MatrixBase<T, 4> ToMatrix() const {
        MatrixBase<T, 4> mat;
        mat(0, 0) = 1 - 2 * y * y - 2 * z * z;
        mat(0, 1) = 2 * x * y - 2 * z * w;
        mat(0, 2) = 2 * x * z + 2 * y * w;
        mat(1, 0) = 2 * x * y + 2 * z * w;
        mat(1, 1) = 1 - 2 * x * x - 2 * z * z;
        mat(1, 2) = 2 * y * z - 2 * x * w;
        mat(2, 0) = 2 * x * z - 2 * y * w;
        mat(2, 1) = 2 * y * z + 2 * x * w;
        mat(2, 2) = 1 - 2 * x * x - 2 * y * y;
        return mat;
    }*/
    Vector3 ToEuler() const {
        T roll, pitch, yaw;
        T sinr_cosp = 2 * (w * x + y * z);
        T cosr_cosp = 1 - 2 * (x * x + y * y);
        roll = std::atan2(sinr_cosp, cosr_cosp);

        T sinp = 2 * (w * y - z * x);
        if (std::abs(sinp) >= 1)
            pitch = std::copysign(std::numbers::pi / 2, sinp); // use 90 degrees if out of range
        else
            pitch = std::asin(sinp);

        T siny_cosp = 2 * (w * z + x * y);
        T cosy_cosp = 1 - 2 * (y * y + z * z);
        yaw = std::atan2(siny_cosp, cosy_cosp);

        return Vector3<T>(pitch, yaw, roll);
    }

    // Methods
    QuaternionBase Conjugate() const {
        return { w, -x, -y, -z };
    }

    T Dot(const QuaternionBase &q) const {
        return w * q.w + x * q.x + y * q.y + z * q.z;
    }

    T Length() const {
        return std::sqrt(w * w + x * x + y * y + z * z);
    }

    T Magnitude() const {
        return std::sqrt(w * w + x * x + y * y + z * z);
    }

    T Max() {
        return std::max({ w, y, z, x });
    }

    T Min() {
        return std::min({ w, y, z, x });
    }

    QuaternionBase Normalized() const {
        T mag = Magnitude();
        return { w / mag, x / mag, y / mag, z / mag };
    }

    QuaternionBase Slerp(const QuaternionBase &q, T t) const {
        T dot = Dot(q);
        T theta = std::acos(dot);
        T sin_theta = std::sin(theta);

        if (std::abs(sin_theta) < 0.001) {
            return (*this + (q - *this) * t).Normalized();
        }

        T scale1 = std::sin((1.0 - t) * theta) / sin_theta;
        T scale2 = std::sin(t * theta) / sin_theta;

        return (*this * scale1 + q * scale2).Normalized();
    }

    T SquaredLength() const {
        return w * w + x * x + y * y + z * z;
    }

    // Arithmetic
    QuaternionBase operator+(const QuaternionBase &q) const {
        return {
            w + q.w,
            x + q.x,
            y + q.y,
            z + q.z
        };
    }
    QuaternionBase operator-(const QuaternionBase &q) const {
        return {
            w - q.w,
            x - q.x,
            y - q.y,
            z - q.z
        };
    }
    QuaternionBase operator*(const QuaternionBase &q) const {
        return {
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w
        };
    }
    QuaternionBase operator*(const T scalar) const {
        return {
            w * scalar,
            x * scalar,
            y * scalar,
            z * scalar
        };
    }

    // Assignment
    QuaternionBase &operator*=(const QuaternionBase &q) {
        *this = *this * q;
        return *this;
    }

private:
    // Helpers
    bool AlmostEqual(float a, float b, float epsilon = 0.000001f) {
        return std::abs(a - b) < epsilon;
    }
};


using Quaternion = QuaternionBase<float>;
using DQuaternion = QuaternionBase<double>;

///
/// @brief Test Interface
///
export namespace Test {

void QuaternionTests();

}

}

module: private;

namespace Ultra::Test {

void Compiler() {
    // Ensure that the sizes are correct
    static_assert(16 == sizeof(QuaternionBase<float>),  "Quaternion<float>:   The type size should be 16 byte(s)!");
    static_assert(32 == sizeof(QuaternionBase<double>), "Quaternion<double>:  The type size should be 32 byte(s)!");
}

void QuaternionTests() {// Preparation
    LogInfo("Testing Quaternion");
    Quaternion q0 { 1, 0, 0, 0 };
    Quaternion q1 { 1, 0, 0, 0 };
    Quaternion q2 { 0, 1, 0, 0 };
    Quaternion q3 { 0, 0, 1, 0 };
    Quaternion q4 { 0, 0, 0, 1 };

    // Test basic properties
    AppAssert(q1.w == 1 && q1.x == 0 && q1.y == 0 && q1.z == 0, "Basic properties test failed for q1.");

    // Test equality
    AppAssert((q0 == q1), "Equality test failed.");
    AppAssert((q1 <=> q2) < 0, "Less than test failed.");
    AppAssert((q2 <=> q1) > 0, "Greater than test failed.");


    // Test arithmetic operations
    auto sum = q1 + q2;
    AppAssert(sum.w == 1 && sum.x == 1 && sum.y == 0 && sum.z == 0, "Addition test failed.");
    auto diff = q1 - q2;
    AppAssert(diff.w == 1 && diff.x == -1 && diff.y == 0 && diff.z == 0, "Subtraction test failed.");
    auto prod = q1 * 2.0f;
    AppAssert(prod.w == 2 && prod.x == 0 && prod.y == 0 && prod.z == 0, "Scalar multiplication test failed.");

    // Test Conjugate
    auto conjugate = q2.Conjugate();
    AppAssert(conjugate.w == 0 && conjugate.x == -1 && conjugate.y == 0 && conjugate.z == 0, "Conjugate test failed.");

    // Test Dot Product
    auto dotProduct = q2.Dot(q3);
    AppAssert(dotProduct == 0, "Dot product test failed.");  // Since q2 and q3 are orthogonal

    // Test Length and Magnitude
    AppAssert(q2.Length() == 1 && q2.Magnitude() == 1, "Length and Magnitude test failed.");  // Since q2 is a unit quaternion

    // Test Normalization
    auto normalized = q2.Normalized();
    AppAssert(normalized.w == 0 && normalized.x == 1 && normalized.y == 0 && normalized.z == 0, "Normalization test failed.");  // Since q2 is already a unit quaternion

    // Test Slerp
    auto slerped = q1.Slerp(q4, 0.5f);
    bool slerp_test = std::abs(slerped.w - 0.70710678118) < 0.000001 &&
        slerped.x == 0 &&
        slerped.y == 0 &&
        std::abs(slerped.z - 0.70710678118) < 0.000001;
    AppAssert(slerp_test, "Slerp test failed.");
    LogInfo("~Testing Quaternion");
}

}

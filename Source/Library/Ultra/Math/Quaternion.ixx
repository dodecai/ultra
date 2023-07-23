export module Ultra.Math.Quaternion;

import Ultra.Core;

export namespace Ultra {

///
/// @brief Quaternion
///
template<typename T>
struct Quaternion {
    T w, x, y, z;

    // Constructors
    Quaternion(): w(1), x(0), y(0), z(0) {}
    Quaternion(T w, T x, T y, T z): w(w), x(x), y(y), z(z) {}

    // Methods
    Quaternion Conjugate() const {
        return { w, -x, -y, -z };
    }
    T Magnitude() const {
        return std::sqrt(w * w + x * x + y * y + z * z);
    }
    Quaternion Normalized() const {
        T mag = Magnitude();
        return { w / mag, x / mag, y / mag, z / mag };
    }

    // Operators
    Quaternion operator*(const Quaternion &q) const {
        return {
            w * q.w - x * q.x - y * q.y - z * q.z,  // new w
            w * q.x + x * q.w + y * q.z - z * q.y,  // new x
            w * q.y - x * q.z + y * q.w + z * q.x,  // new y
            w * q.z + x * q.y - y * q.x + z * q.w   // new z
        };
    }
    Quaternion &operator*=(const Quaternion &q) {
        *this = *this * q;
        return *this;
    }
};

}

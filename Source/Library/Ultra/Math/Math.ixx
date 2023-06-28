module;

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#define GLM_ENABLE_EXPERIMENTAL

export module Ultra.Math;

export import <glm/glm.hpp>;
export import <glm/gtc/type_ptr.hpp>;
//export import <glm/gtx/quaternion.hpp>;
export import <glm/gtx/string_cast.hpp>;

import Ultra.Core;

export namespace Ultra {

// Scalar Types
using Bool = bool;
using Double = double;
using Float = float;
using Int = int32_t;
using UInt = uint32_t;

// Vector Types
using Bool2 = glm::bvec2;
using Bool3 = glm::bvec3;
using Bool4 = glm::bvec4;

using Double2 = glm::dvec2;
using Double3 = glm::dvec3;
using Double4 = glm::dvec4;

using Float2 = glm::vec2;
using Float3 = glm::vec3;
using Float4 = glm::vec4;

using Int2 = glm::ivec2;
using Int3 = glm::ivec3;
using Int4 = glm::ivec4;

using UInt2 = glm::uvec2;
using UInt3 = glm::uvec3;
using UInt4 = glm::uvec4;

// Matrix Types
using Matrix2 = glm::mat2;
using Matrix3 = glm::mat3;
using Matrix4 = glm::mat4;
using Matrix2x2 = glm::mat2x2;
using Matrix2x3 = glm::mat2x3;
using Matrix2x4 = glm::mat2x4;
using Matrix3x2 = glm::mat3x2;
using Matrix3x3 = glm::mat3x3;
using Matrix3x4 = glm::mat3x4;
using Matrix4x2 = glm::mat4x2;
using Matrix4x3 = glm::mat4x3;
using Matrix4x4 = glm::mat4x4;

using DoubleMatrix2 = glm::dmat2;
using DoubleMatrix3 = glm::dmat3;
using DoubleMatrix4 = glm::dmat4;
using DoubleMatrix2x2 = glm::dmat2x2;
using DoubleMatrix2x3 = glm::dmat2x3;
using DoubleMatrix2x4 = glm::dmat2x4;
using DoubleMatrix3x2 = glm::dmat3x2;
using DoubleMatrix3x3 = glm::dmat3x3;
using DoubleMatrix3x4 = glm::dmat3x4;
using DoubleMatrix4x2 = glm::dmat4x2;
using DoubleMatrix4x3 = glm::dmat4x3;
using DoubleMatrix4x4 = glm::dmat4x4;

// Other
//using Quaternion = glm::quat;

namespace Math {

bool DecomposeTransform([[maybe_unused]] const glm::mat4 &transform, [[maybe_unused]] glm::vec3 &position, [[maybe_unused]] glm::vec3 &orientation, [[maybe_unused]] glm::vec3 &scale) {
    // from glm::decompose in matrix_decompose.inl

    using namespace glm;
    using T = float;

    mat4 LocalMatrix(transform);

    // Normalize the matrix.
    if (epsilonEqual(LocalMatrix[3][3], static_cast<T>(0), epsilon<T>())) return false;

    // First, isolate perspective.  This is the messiest.
    if (
        epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
        epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
        epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>())) {
        // Clear the perspective partition
        LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
        LocalMatrix[3][3] = static_cast<T>(1);
    }

    // Next take care of translation (easy).
    position = vec3(LocalMatrix[3]);
    LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

    vec3 Row[3], Pdum3;

    // Now get scale and shear.
    for (length_t i = 0; i < 3; ++i)
        for (length_t j = 0; j < 3; ++j)
            Row[i][j] = LocalMatrix[i][j];

    // Compute X/Y/Z scale factor and normalize 1st/2nd/3rd row.
    scale.x = length(Row[0]);// v3Length(Row[0]);
    Row[0] = detail::scale(Row[0], static_cast<T>(1));
    scale.y = length(Row[1]);
    Row[1] = detail::scale(Row[1], static_cast<T>(1));
    scale.z = length(Row[2]);
    Row[2] = detail::scale(Row[2], static_cast<T>(1));

    // At this point, the matrix (in rows[]) is orthonormal.
    // Check for a coordinate system flip.  If the determinant
    // is -1, then negate the matrix and the scaling factors.
    #if 0
    Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
    if (dot(Row[0], Pdum3) < 0) {
        for (length_t i = 0; i < 3; i++) {
            Scale[i] *= static_cast<T>(-1);
            Row[i] *= static_cast<T>(-1);
        }
    }
    #endif

    orientation.y = asin(-Row[0][2]);
    if (cos(orientation.y) != 0) {
        orientation.x = atan2(Row[1][2], Row[2][2]);
        orientation.z = atan2(Row[0][1], Row[0][0]);
    } else {
        orientation.x = atan2(Row[2][0], Row[1][1]);
        orientation.z = 0;
    }

    return true;
}

}

}

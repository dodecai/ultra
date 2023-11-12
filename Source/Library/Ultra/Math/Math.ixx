export module Ultra.Math;

#pragma warning(push)
#pragma warning(disable: 4201)

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_LEFT_HANDED

export import <glm/glm.hpp>;
export import <glm/gtc/constants.hpp>;
export import <glm/gtc/matrix_transform.hpp>;
export import <glm/gtc/type_ptr.hpp>;
export import <glm/gtx/compatibility.hpp>;
export import <glm/gtx/matrix_decompose.hpp>;
export import <glm/gtx/quaternion.hpp>;
export import <glm/gtx/string_cast.hpp>;

#pragma warning(pop)

import Ultra.Core;
export import Ultra.Math.Matrix;
export import Ultra.Math.Quaternion;
export import Ultra.Math.Vector;

export namespace Ultra {

///
/// @brief Migration Section, until everything is implemented, we use the glm Mappings
///

// Scalar Types
using Bool = bool;
using Double = double;
using Float = float;
using Int = int32_t;
using UInt = uint32_t;

// Vector Types
using Bool2 = VectorBase<bool, 2, VectorAliases::None>;
using Bool3 = VectorBase<bool, 3, VectorAliases::None>;
using Bool4 = VectorBase<bool, 4, VectorAliases::None>;

using Double2 = VectorBase<double, 2>;
using Double3 = VectorBase<double, 3>;
using Double4 = VectorBase<double, 4>;

using Float2 = VectorBase<float, 2>;
using Float3 = VectorBase<float, 3>;
using Float4 = VectorBase<float, 4>;

using Int2 = VectorBase<int32_t, 2>;
using Int3 = VectorBase<int32_t, 3>;
using Int4 = VectorBase<int32_t, 4>;

using UInt2 = VectorBase<uint32_t, 2>;
using UInt3 = VectorBase<uint32_t, 3>;
using UInt4 = VectorBase<uint32_t, 4>;

// Matrix Types
using glm::mat2;
using glm::mat3;
using glm::mat4;
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

    vec3 Row[3]; // , Pdum3;

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

#ifdef MATH_LOOKAT_EXAMPLE
// Custom implementation of the LookAt function
glm::mat4 calculate_lookAt_matrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp) {
    // 1. Position = known
    // 2. Calculate cameraDirection
    glm::vec3 zaxis = glm::normalize(position - target);
    // 3. Get positive right axis vector
    glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(worldUp), zaxis));
    // 4. Calculate camera up vector
    glm::vec3 yaxis = glm::cross(zaxis, xaxis);

    // Create translation and rotation matrix
    // In glm we access elements as mat[col][row] due to column-major layout
    glm::mat4 translation = glm::mat4(1.0f); // Identity matrix by default
    translation[3][0] = -position.x; // Third column, first row
    translation[3][1] = -position.y;
    translation[3][2] = -position.z;
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0][0] = xaxis.x; // First column, first row
    rotation[1][0] = xaxis.y;
    rotation[2][0] = xaxis.z;
    rotation[0][1] = yaxis.x; // First column, second row
    rotation[1][1] = yaxis.y;
    rotation[2][1] = yaxis.z;
    rotation[0][2] = zaxis.x; // First column, third row
    rotation[1][2] = zaxis.y;
    rotation[2][2] = zaxis.z;

    // Return lookAt matrix as combination of translation and rotation matrix
    return rotation * translation; // Remember to read from right to left (first translation then rotation)
}


// Don't forget to replace glm::lookAt with your own version
// view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
view = calculate_lookAt_matrix(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
#endif

}

}

// Extensions

namespace std {

template<>
struct std::formatter<glm::vec3> {
    constexpr auto parse(format_parse_context &ctx) {
        const auto it = ctx.begin();
        if (it != ctx.end() && *it != '}')
            throw format_error("Invalid format for glm::vec4!");
        return it;
    }

    template<typename FormatContext>
    auto format(const glm::vec4 &vec, FormatContext &ctx) {
        return format_to(ctx.out(), "[{}, {}, {}]", vec.x, vec.y, vec.z);
    }
};


template<>
struct std::formatter<glm::vec4> {
    constexpr auto parse(format_parse_context &ctx) {
        const auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') throw format_error("Invalid format for glm::vec4!");
        return it;
    }

    template<typename FormatContext>
    auto format(const glm::vec4 &vec, FormatContext &ctx) {
        return format_to(ctx.out(), "[{}, {}, {}, {}]", vec.x, vec.y, vec.z, vec.w);
    }
};

template<>
struct std::formatter<glm::mat4> {
    constexpr auto parse(format_parse_context &ctx) {
        const auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') throw format_error("Invalid format for für glm::mat4!");
        return it;
    }

    template<typename FormatContext>
    auto format(const glm::mat4 &mat, FormatContext &ctx) {
        std::ostringstream os;
        os << "[";
        for (int i = 0; i < 4; ++i) {
            os << "[";
            for (int j = 0; j < 4; ++j) {
                os << mat[i][j];
                if (j < 3)
                    os << ", ";
            }
            os << "]";
            if (i < 3)
                os << ", ";
        }
        os << "]";
        return format_to(ctx.out(), "{}", os.str());
    }
};

}

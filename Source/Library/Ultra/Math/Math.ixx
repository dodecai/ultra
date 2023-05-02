export module Ultra.Math;

export import <glm/glm.hpp>;
export import <glm/gtc/type_ptr.hpp>;

import Ultra.Core;
import Ultra.Logger;

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

}

#pragma once

#include "Ultra.pch"
#include "Ultra/Core.h"

namespace Ultra {

enum class UniformType {
    None        = 0,
    Float       = 1,
    Float2      = 2,
    Float3      = 3,
    Int32       = 4,
    UInt32      = 5,
    Matrix3x3   = 6,
    Matrix4x4   = 7,
};

struct UniformDeclariation {
    string Name;
    std::ptrdiff_t Offset;
    UniformType Type;
};

struct UniformBuffer {
    std::byte *Buffer;
    vector<UniformDeclariation> Uniforms;
};

struct UniformBufferBase {
    virtual const std::byte *GetBuffer() const = 0;
    virtual const UniformDeclariation *GetUniforms() const = 0;
    virtual const int GetUnifromCount() const = 0;
};

template<uint32_t N, uint32_t U>
struct UniformBufferDeclaration: public UniformBufferBase {
    std::byte *Buffer[N];
    UniformDeclariation Uniforms[U];
    std::ptrdiff_t Cursor = 0;
    int32_t Index = 0;

    virtual const std::byte *GetBuffer() const override { return Buffer; }
    virtual const UniformDeclariation *GetUniforms() const override { return Uniforms; }
    virtual const int32_t *GetUniformCount() const override { return U; }

    template<typename T>
    void Push(const string &name, const T &data) {
    }

    template<typename T>
    void Push(const string &name, const float &data) {
        Uniforms[Index++] = { UniformType::Float, Cursor, name };
        memcpy(Buffer + Cursor, &data, sizeof(float));
        Cursor += sizeof(float);
    }

    template<typename T>
    void Push(const string &name, const glm::vec3 &data) {
        Uniforms[Index++] = { UniformType::Float3, Cursor, name };
        memcpy(Buffer + Cursor, glm::value_ptr(data), sizeof(glm::vec3));
        Cursor += sizeof(glm::vec3);
    }

    template<typename T>
    void Push(const string &name, const glm::vec4 &data) {
        Uniforms[Index++] = { UniformType::Float4, Cursor, name };
        memcpy(Buffer + Cursor, glm::value_ptr(data), sizeof(glm::vec4));
        Cursor += sizeof(glm::vec4);
    }

    template<typename T>
    void Push(const string &name, const glm::mat4 &data) {
        Uniforms[Index++] = { UniformType::Float4, Cursor, name };
        memcpy(Buffer + Cursor, glm::value_ptr(data), sizeof(glm::mat4));
        Cursor += sizeof(glm::mat4);
    }

};

enum class ShaderUniformType {
    None        = 0,
    Bool        = 1,
    Int         = 2,
    Float       = 3,
    Vec2        = 4,
    Vec3        = 5,
    Vec4        = 6,
    Mat3        = 7,
    Mat4        = 8,
};

enum class ShaderDomain {
    None    = 0,
    Vertex  = 1,
    Pixel   = 2,
};

class ShaderResourceDeclaration {
public:
    ShaderResourceDeclaration() = default;
    ShaderResourceDeclaration(const string &name, uint32_t resourceRegister, uint32_t count):
        mName(name),
        mRegister(resourceRegister),
        mCount(count) {
    }

    virtual const string &GetName() const { return mName; }
    virtual const uint32_t GetRegister() const { return mRegister; }
    virtual const uint32_t GetCount() const { return mCount; }

private:
    string mName;
    uint32_t mRegister = 0;
    uint32_t mCount = 0;
};

using ShaderResourceList = vector<ShaderResourceDeclaration *>;

}

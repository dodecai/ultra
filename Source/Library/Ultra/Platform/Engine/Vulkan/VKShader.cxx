module;

#include <glm/glm.hpp>

module Ultra.Platform.Renderer.VKShader;

import Ultra.Engine.Utility.ShaderCompiler;

#pragma warning(push)
#pragma warning(disable: 4100)

namespace Ultra {

VKShader::VKShader(const string &source, const string &entryPoint, const ShaderType type): Shader(source, entryPoint, type) {
    auto shaderCode = source;
    auto shaders = Convert(shaderCode);
    Compile(shaders);
}

VKShader::~VKShader() {}


void VKShader::Compile(ShaderList shaders) {
    for (auto shader : shaders) {
        auto result = ShaderCompiler::Compile(mShaderName, (ShaderType)shader.first, shader.second);
    }
}

void VKShader::Bind() const {}

void VKShader::Unbind() const {}


int32_t VKShader::FindUniformLocation(const string &name) const {
    return 0;
}


void VKShader::UpdateUniformBuffer(const string &name, const void *data, size_t size) {}

void VKShader::UpdateUniform(const string &name, const Bool &data) {}

void VKShader::UpdateUniform(const string &name, const Bool2 &data) {}

void VKShader::UpdateUniform(const string &name, const Bool3 &data) {}

void VKShader::UpdateUniform(const string &name, const Bool4 &data) {}

void VKShader::UpdateUniform(const string &name, const Double &data) {}

void VKShader::UpdateUniform(const string &name, const Double2 &data) {}

void VKShader::UpdateUniform(const string &name, const Double3 &data) {}

void VKShader::UpdateUniform(const string &name, const Double4 &data) {}

void VKShader::UpdateUniform(const string &name, const Float &data) {}

void VKShader::UpdateUniform(const string &name, const Float2 &data) {}

void VKShader::UpdateUniform(const string &name, const Float3 &data) {}

void VKShader::UpdateUniform(const string &name, const Float4 &data) {}

void VKShader::UpdateUniform(const string &name, const Int &data) {}

void VKShader::UpdateUniform(const string &name, const Int2 &data) {}

void VKShader::UpdateUniform(const string &name, const Int3 &data) {}

void VKShader::UpdateUniform(const string &name, const Int4 &data) {}

void VKShader::UpdateUniform(const string &name, const UInt &data) {}

void VKShader::UpdateUniform(const string &name, const UInt2 &data) {}

void VKShader::UpdateUniform(const string &name, const UInt3 &data) {}

void VKShader::UpdateUniform(const string &name, const UInt4 &data) {}

void VKShader::UpdateUniform(const string &name, const Matrix2 &data) {}

void VKShader::UpdateUniform(const string &name, const Matrix3 &data) {}

void VKShader::UpdateUniform(const string &name, const Matrix4 &data) {}

}

#pragma warning(push)

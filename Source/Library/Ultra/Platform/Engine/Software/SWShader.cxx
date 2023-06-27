module;

#include <glm/glm.hpp>

module Ultra.Platform.Renderer.SWShader;

#pragma warning(push)
#pragma warning(disable: 4100)

namespace Ultra {

SWShader::SWShader(const string &source, const string &entryPoint, const ShaderType type): Shader(source, entryPoint, type) {}

SWShader::~SWShader() {}


void SWShader::Compile(ShaderList shaders) {}

void SWShader::Bind() const {}

void SWShader::Unbind() const {}


int32_t SWShader::FindUniformLocation(const string &name) const {
    return 0;
}


void SWShader::UpdateUniformBuffer(const string &name, const void *data, size_t size) {}

void SWShader::UpdateUniform(const string &name, const Bool &data) {}

void SWShader::UpdateUniform(const string &name, const Bool2 &data) {}

void SWShader::UpdateUniform(const string &name, const Bool3 &data) {}

void SWShader::UpdateUniform(const string &name, const Bool4 &data) {}

void SWShader::UpdateUniform(const string &name, const Double &data) {}

void SWShader::UpdateUniform(const string &name, const Double2 &data) {}

void SWShader::UpdateUniform(const string &name, const Double3 &data) {}

void SWShader::UpdateUniform(const string &name, const Double4 &data) {}

void SWShader::UpdateUniform(const string &name, const Float &data) {}

void SWShader::UpdateUniform(const string &name, const Float2 &data) {}

void SWShader::UpdateUniform(const string &name, const Float3 &data) {}

void SWShader::UpdateUniform(const string &name, const Float4 &data) {}

void SWShader::UpdateUniform(const string &name, const Int &data) {}

void SWShader::UpdateUniform(const string &name, const Int2 &data) {}

void SWShader::UpdateUniform(const string &name, const Int3 &data) {}

void SWShader::UpdateUniform(const string &name, const Int4 &data) {}

void SWShader::UpdateUniform(const string &name, const UInt &data) {}

void SWShader::UpdateUniform(const string &name, const UInt2 &data) {}

void SWShader::UpdateUniform(const string &name, const UInt3 &data) {}

void SWShader::UpdateUniform(const string &name, const UInt4 &data) {}

void SWShader::UpdateUniform(const string &name, const Matrix2 &data) {}

void SWShader::UpdateUniform(const string &name, const Matrix3 &data) {}

void SWShader::UpdateUniform(const string &name, const Matrix4 &data) {}

}

#pragma warning(push)

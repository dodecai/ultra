module;

#include <glm/glm.hpp>

module Ultra.Platform.Renderer.DXShader;

namespace Ultra {

DXShader::DXShader(const string &source, const string &entryPoint, const ShaderType type): Shader(source, entryPoint, type) {}

DXShader::~DXShader() {}


void DXShader::Compile(ShaderList shaders) {}

void DXShader::Bind() const {}

void DXShader::Unbind() const {}


int32_t DXShader::FindUniformLocation(const string &name) const {
    return 0;
}


void DXShader::UpdateUniformBuffer(const string &name, const void *data, size_t size) {}

void DXShader::UpdateUniform(const string &name, const Bool &data) {}

void DXShader::UpdateUniform(const string &name, const Bool2 &data) {}

void DXShader::UpdateUniform(const string &name, const Bool3 &data) {}

void DXShader::UpdateUniform(const string &name, const Bool4 &data) {}

void DXShader::UpdateUniform(const string &name, const Double &data) {}

void DXShader::UpdateUniform(const string &name, const Double2 &data) {}

void DXShader::UpdateUniform(const string &name, const Double3 &data) {}

void DXShader::UpdateUniform(const string &name, const Double4 &data) {}

void DXShader::UpdateUniform(const string &name, const Float &data) {}

void DXShader::UpdateUniform(const string &name, const Float2 &data) {}

void DXShader::UpdateUniform(const string &name, const Float3 &data) {}

void DXShader::UpdateUniform(const string &name, const Float4 &data) {}

void DXShader::UpdateUniform(const string &name, const Int &data) {}

void DXShader::UpdateUniform(const string &name, const Int2 &data) {}

void DXShader::UpdateUniform(const string &name, const Int3 &data) {}

void DXShader::UpdateUniform(const string &name, const Int4 &data) {}

void DXShader::UpdateUniform(const string &name, const UInt &data) {}

void DXShader::UpdateUniform(const string &name, const UInt2 &data) {}

void DXShader::UpdateUniform(const string &name, const UInt3 &data) {}

void DXShader::UpdateUniform(const string &name, const UInt4 &data) {}

void DXShader::UpdateUniform(const string &name, const Matrix2 &data) {}

void DXShader::UpdateUniform(const string &name, const Matrix3 &data) {}

void DXShader::UpdateUniform(const string &name, const Matrix4 &data) {}

}

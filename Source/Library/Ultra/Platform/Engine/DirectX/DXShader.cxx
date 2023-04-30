module;

#include <glm/glm.hpp>

module Ultra.Platform.Renderer.DXShader;

namespace Ultra {

DXShader::DXShader(const string &source, const string &entryPoint, const ShaderType type): Shader(source, entryPoint, type) {}

DXShader::~DXShader() {}


void DXShader::Compile(ShaderList shaders) {}

void DXShader::Bind() const {}

void DXShader::Unbind() const {}


int32_t DXShader::GetUniformLocation(const string &name) const {
    return 0;
}


void DXShader::SetUniformBuffer(const string &name, const void *data, size_t size) {}

void DXShader::SetUniform(const string &name, bool data) {}

void DXShader::SetUniform(const string &name, float data) {}

void DXShader::SetUniform(const string &name, glm::vec2 &data) {}

void DXShader::SetUniform(const string &name, glm::vec3 &data) {}

void DXShader::SetUniform(const string &name, glm::vec4 &data) {}

void DXShader::SetUniform(const string &name, int data) {}

void DXShader::SetUniform(const string &name, glm::ivec2 &data) {}

void DXShader::SetUniform(const string &name, glm::ivec3 &data) {}

void DXShader::SetUniform(const string &name, glm::ivec4 &data) {}

void DXShader::SetUniform(const string &name, unsigned int data) {}

void DXShader::SetUniform(const string &name, glm::uvec2 &data) {}

void DXShader::SetUniform(const string &name, glm::uvec3 &data) {}

void DXShader::SetUniform(const string &name, glm::uvec4 &data) {}

void DXShader::SetUniform(const string &name, glm::mat2 &data) {}

void DXShader::SetUniform(const string &name, glm::mat3 &data) {}

void DXShader::SetUniform(const string &name, glm::mat4 &data) {}

}

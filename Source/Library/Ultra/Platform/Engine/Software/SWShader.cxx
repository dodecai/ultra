module;

#include <glm/glm.hpp>

module Ultra.Platform.Renderer.SWShader;

namespace Ultra {

SWShader::SWShader(const string &source, const string &entryPoint, const ShaderType type): Shader(source, entryPoint, type) {}

SWShader::~SWShader() {}


void SWShader::Compile(ShaderList shaders) {}

void SWShader::Bind() const {}

void SWShader::Unbind() const {}


int32_t SWShader::GetUniformLocation(const string &name) const {
    return 0;
}


void SWShader::SetUniformBuffer(const string &name, const void *data, size_t size) {}

void SWShader::SetUniform(const string &name, bool data) {}

void SWShader::SetUniform(const string &name, float data) {}

void SWShader::SetUniform(const string &name, glm::vec2 &data) {}

void SWShader::SetUniform(const string &name, glm::vec3 &data) {}

void SWShader::SetUniform(const string &name, glm::vec4 &data) {}

void SWShader::SetUniform(const string &name, int data) {}

void SWShader::SetUniform(const string &name, glm::ivec2 &data) {}

void SWShader::SetUniform(const string &name, glm::ivec3 &data) {}

void SWShader::SetUniform(const string &name, glm::ivec4 &data) {}

void SWShader::SetUniform(const string &name, unsigned int data) {}

void SWShader::SetUniform(const string &name, glm::uvec2 &data) {}

void SWShader::SetUniform(const string &name, glm::uvec3 &data) {}

void SWShader::SetUniform(const string &name, glm::uvec4 &data) {}

void SWShader::SetUniform(const string &name, glm::mat2 &data) {}

void SWShader::SetUniform(const string &name, glm::mat3 &data) {}

void SWShader::SetUniform(const string &name, glm::mat4 &data) {}

}

module;

#include <glm/glm.hpp>

module Ultra.Platform.Renderer.VKShader;

namespace Ultra {

VKShader::VKShader(const string &source, const string &entryPoint, const ShaderType type): Shader(source, entryPoint, type) {}

VKShader::~VKShader() {}


void VKShader::Compile(ShaderList shaders) {}

void VKShader::Bind() const {}

void VKShader::Unbind() const {}


int32_t VKShader::GetUniformLocation(const string &name) const {
    return 0;
}


void VKShader::SetUniformBuffer(const string &name, const void *data, size_t size) {}

void VKShader::SetUniform(const string &name, bool data) {}

void VKShader::SetUniform(const string &name, float data) {}

void VKShader::SetUniform(const string &name, glm::vec2 &data) {}

void VKShader::SetUniform(const string &name, glm::vec3 &data) {}

void VKShader::SetUniform(const string &name, glm::vec4 &data) {}

void VKShader::SetUniform(const string &name, int data) {}

void VKShader::SetUniform(const string &name, glm::ivec2 &data) {}

void VKShader::SetUniform(const string &name, glm::ivec3 &data) {}

void VKShader::SetUniform(const string &name, glm::ivec4 &data) {}

void VKShader::SetUniform(const string &name, unsigned int data) {}

void VKShader::SetUniform(const string &name, glm::uvec2 &data) {}

void VKShader::SetUniform(const string &name, glm::uvec3 &data) {}

void VKShader::SetUniform(const string &name, glm::uvec4 &data) {}

void VKShader::SetUniform(const string &name, glm::mat2 &data) {}

void VKShader::SetUniform(const string &name, glm::mat3 &data) {}

void VKShader::SetUniform(const string &name, glm::mat4 &data) {}

}

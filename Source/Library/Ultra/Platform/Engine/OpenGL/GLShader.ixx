module;

#include <glm/glm.hpp>

export module Ultra.Platform.Renderer.GLShader;

import Ultra.Engine.Renderer.Shader;

export namespace Ultra {

class GLShader: public Shader {
public:
    GLShader(const string &source, const string &entryPoint, const ShaderType type);
    virtual ~GLShader();

    virtual void Compile(ShaderList shaders) override;
    virtual void Bind() const override;
    virtual void Unbind() const override;

    // Accessors
    virtual int32_t GetUniformLocation(const string &name) const override;

    // Mutators
    virtual void SetUniformBuffer(const string &name, const void *data, size_t size) override;
    virtual void SetUniform(const string &name, bool data) override;
    virtual void SetUniform(const string &name, float data) override;
    virtual void SetUniform(const string &name, glm::vec2 &data) override;
    virtual void SetUniform(const string &name, glm::vec3 &data) override;
    virtual void SetUniform(const string &name, glm::vec4 &data) override;
    virtual void SetUniform(const string &name, int data) override;
    virtual void SetUniform(const string &name, glm::ivec2 &data) override;
    virtual void SetUniform(const string &name, glm::ivec3 &data) override;
    virtual void SetUniform(const string &name, glm::ivec4 &data) override;
    virtual void SetUniform(const string &name, unsigned int data) override;
    virtual void SetUniform(const string &name, glm::uvec2 &data) override;
    virtual void SetUniform(const string &name, glm::uvec3 &data) override;
    virtual void SetUniform(const string &name, glm::uvec4 &data) override;
    virtual void SetUniform(const string &name, glm::mat2 &data) override;
    virtual void SetUniform(const string &name, glm::mat3 &data) override;
    virtual void SetUniform(const string &name, glm::mat4 &data) override;

private:
};

}

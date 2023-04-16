module;

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

export module Ultra.Platform.Engine.ZGLShader;

import Ultra.Engine.ZShader;

export namespace Ultra {

class ZGLShader: public ZShader {
public:
    // Default
    ZGLShader(const string &source);
    ZGLShader(const string &vertexSource, const string &fragmentSource);
    virtual ~ZGLShader();
    virtual bool Reload() const override;

    virtual void Bind() const override;
    virtual void Unbind() const override;

    // Accessors
    virtual const string &GetName() const override;
    virtual const unordered_map<string, ShaderBuffer> &GetBuffers() const override;
    virtual const unordered_map<string, ShaderResourceDeclaration> &GetResources() const override;

    // Mutators
    virtual void SetUniformBuffer(const string &name, const void *data, size_t size) override;
    virtual void SetUniform(const string &name, bool data) override;
    virtual void SetUniform(const string &name, int data) override;
    virtual void SetUniform(const string &name, float data) override;
    virtual void SetUniform(const string &name, glm::vec2 &data) override;
    virtual void SetUniform(const string &name, glm::vec3 &data) override;
    virtual void SetUniform(const string &name, glm::vec4 &data) override;
    virtual void SetUniform(const string &name, glm::mat3 &data) override;
    virtual void SetUniform(const string &name, glm::mat4 &data) override;

private:
    // Helpers
    void Compile(unordered_map<uint32_t, string> sources);
    unordered_map<uint32_t, string> Prepare(string &source);
    int32_t GetUniformLocation(const string &name) const;

private:
    // Properties
    string ShaderName;
    uint32_t RendererID;
    mutable unordered_map<string, int32_t> UniformLocationCache;
};


}

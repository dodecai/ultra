#pragma once

#include "Ultra/Renderer/Shader.h"

namespace Ultra {

class GLShader: public Shader {
public:
    // Default
	GLShader(const string &source);
	GLShader(const string &vertexSource, const string &fragmentSource);
	virtual ~GLShader();
    virtual void Reload() const override;

	virtual void Bind() const override;
	virtual void Unbind() const override;

    // Accessors
    virtual const string GetName() const override;
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

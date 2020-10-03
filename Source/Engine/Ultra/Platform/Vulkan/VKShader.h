#pragma once

#include "Ultra/Renderer/Shader.h"

namespace Ultra {

class VKShader: public Shader {
    uint32_t RendererID;
    string ShaderName;

    mutable unordered_map<string, int32_t> UniformLocationCache;

public:
    VKShader(const string &source);
    VKShader(const string &vertexSource, const string &fragmentSource);
    virtual ~VKShader();

    virtual void Bind() const override; // Activate
    virtual void Unbind() const override;

    virtual const string GetName() const override;

    virtual void SetInt(const string &name, int value) override;
    virtual void SetIntArray(const string &name, int *values, size_t count) override;
    virtual void SetFloat(const string &name, const float value) override;
    virtual void SetFloat3(const string &name, const glm::vec3 &values) override;
    virtual void SetFloat4(const string &name, const glm::vec4 &values) override;
    virtual void SetMat4(const string &name, const glm::mat4 &values) override;

private:
    void Compile(unordered_map<unsigned int, string> sources);
    unordered_map<unsigned int, string> Prepare(string &source);

    int32_t GetUniformLocation(const string &name) const;

    // Uniforms
    virtual void UploadaUniformInt(const string &name, int values) const;
    virtual void UploadUniformIntArray(const string &name, int *values, size_t count) const;

    virtual void UploadaUniformFloat(const string &name, const float value) const;
    virtual void UploadaUniformFloat2(const string &name, const glm::vec2 &values) const;
    virtual void UploadaUniformFloat3(const string &name, const glm::vec3 &values) const;
    virtual void UploadaUniformFloat4(const string &name, const glm::vec4 &values) const;

    virtual void UploadaUniformMat3(const string &name, const glm::mat3 &matrix) const;
    virtual void UploadaUniformMat4(const string &name, const glm::mat4 &matrix) const;
};

}

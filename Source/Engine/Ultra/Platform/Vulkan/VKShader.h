#pragma once

#include <Platform/GFX/Vulkan/Vulkan.h>

#include "Ultra/Renderer/Shader.h"

namespace Ultra {

class VKShader: public Shader {
public:
    VKShader(const string &source);
    VKShader(const string &vertexSource, const string &fragmentSource);
    virtual ~VKShader();

    virtual void Bind() const override; // Activate
    virtual uint32_t GetRendererID() const override { return 0; }
    virtual void Reload() const override {}
    virtual void Unbind() const override;

    virtual const unordered_map<string, ShaderBuffer> &GetShaderBuffers() const { unordered_map<string, ShaderBuffer> result; return result; }
    virtual const unordered_map<string, ShaderResourceDeclaration> &GetResources() const { unordered_map<string, ShaderResourceDeclaration> result; return result; }

    virtual void SetUniformBuffer(const string &name, const void *data, uint32_t size) override {}
    virtual void SetUniform(const string &name, float value) override {}
    virtual void SetUniform(const string &name, int value) override {}
    virtual void SetUniform(const string &name, glm::vec2 &value) override {}
    virtual void SetUniform(const string &name, glm::vec3 &value) override {}
    virtual void SetUniform(const string &name, glm::vec4 &value) override {}
    virtual void SetUniform(const string &name, glm::mat3 &value) override {}
    virtual void SetUniform(const string &name, glm::mat4 &value) override {}


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

private:
    uint32_t RendererID;
    string AssetPath;
    string ShaderName;

    vk::DescriptorSetLayout DescriptorSetLayout;
    vk::DescriptorSet DescriptorSet;
    vk::DescriptorPool DescriptorPool;

    unordered_map<uint32_t, UniformBuffer> UniformBuffers;
    //unordered_map<uint32_t, ImageSampler> ImageSamplers;
    unordered_map<uint32_t, vk::WriteDescriptorSet> WriteDescriptorSets;
    //vector<PushConstantRange> PushConstantRange;

    mutable unordered_map<string, int32_t> UniformLocationCache;

};

}

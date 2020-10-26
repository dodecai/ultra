#pragma once

#include <Platform/GFX/Vulkan/Vulkan.h>

#include "Ultra/Renderer/Shader.h"

namespace Ultra {

class VKShader: public Shader {
    using ShaderMap = unordered_map<vk::ShaderStageFlagBits, string>;

public:
    // Default
    VKShader(const string &source);
    VKShader(const string &vertexSource, const string &fragmentSource);
    virtual ~VKShader();
    virtual void Reload() const override;

    virtual void Bind() const override;
    virtual void Unbind() const override;

    // Accessors
    virtual const string GetName() const override;
    virtual const unordered_map<string, ShaderBuffer> &GetBuffers() const override;
    virtual const unordered_map<string, ShaderResourceDeclaration> &GetResources() const override;

    vector<vk::PipelineShaderStageCreateInfo> GetStages() {
        return mShaderStages;
    }

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
    void CreateModules();
    void Compile(ShaderMap &sources);
    ShaderMap Prepare(string &source);

private:
    string AssetPath;
    string ShaderName;
    unordered_map<vk::ShaderStageFlagBits, string> mShaderSource;
    vector<vk::PipelineShaderStageCreateInfo> mShaderStages;

    unordered_map<uint32_t, UniformBuffer> UniformBuffers;
    //unordered_map<uint32_t, ImageSampler> ImageSamplers;
    unordered_map<uint32_t, vk::WriteDescriptorSet> WriteDescriptorSets;
    //vector<PushConstantRange> PushConstantRange;

    vk::DescriptorSetLayout DescriptorSetLayout;
    vk::DescriptorSet DescriptorSet;
    vk::DescriptorPool DescriptorPool;

    vector<vk::ShaderModule> mShaderModules;
};

}

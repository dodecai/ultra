#pragma once

#include "Ultra.pch"
#include "Ultra/Core.h"

#include <Platform/GFX/Vulkan/VKContext.h>

namespace Ultra {

class VKTest {
public:
    VKTest();
    ~VKTest();

    void Update();
    void Resize();

private:
    void LoadResources();
    void DestroyResources();

    void SetupCommands();
    void CreateCommands();
    void DestroyCommands();

private:
    static inline VKContext *pContext;
    static VKContext sContext;

    // ToDo: CleanUp everything not needed...
    uint32_t QueueFamilyIndex;
    vector<vk::CommandBuffer> CommandBuffers;
    uint32_t CurrentBuffer = 0;

    vk::Pipeline Pipeline;
    vk::PipelineCache PipelineCache;
    vk::PipelineLayout PipelineLayout;

    vk::DescriptorPool DescriptorPool;
    vector<vk::DescriptorSet> DescriptorSets;
    vector<vk::DescriptorSetLayout> DescriptorSetLayouts;

    vk::Buffer IndexBuffer;
    vk::Buffer VertexBuffer;
    vk::ShaderModule VertModule;
    vk::ShaderModule FragModule;

    // Resources
    // Vertex buffer and attributes
    struct {
        vk::DeviceMemory memory;														// Handle to the device memory for this buffer
        vk::Buffer buffer;																// Handle to the Vulkan buffer object that the memory is bound to
        vk::PipelineVertexInputStateCreateInfo inputState;
        vk::VertexInputBindingDescription inputBinding;
        std::vector<vk::VertexInputAttributeDescription> inputAttributes;
    } Vertices;

    // Index buffer
    struct {
        vk::DeviceMemory memory;
        vk::Buffer buffer;
        uint32_t count;
    } Indices;

    // Uniform block object
    struct {
        vk::DeviceMemory memory;
        vk::Buffer buffer;
        vk::DescriptorBufferInfo descriptor;
    } UniformDataVS;
};

}

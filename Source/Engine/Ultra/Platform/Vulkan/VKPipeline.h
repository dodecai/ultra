#pragma once

#include <vulkan/vulkan.hpp>

#include "Ultra/Renderer/Pipeline.h"
#include "VKContext.h"

namespace Ultra {

class VKPipeline: public Pipeline {
public:
    VKPipeline(const PipelineProperties &properties);
    virtual ~VKPipeline();

    virtual void Bind() override;
    void Bind(vk::CommandBuffer);
    virtual void Invalidate() override;

    virtual PipelineProperties &GetProperties() override;
    virtual const PipelineProperties &GetProperties() const override;

    vk::Pipeline GetNativePipeline();
    vk::PipelineLayout GetNativePipelineLayout();

private:
    void CreateVertexBuffer();

private:
    Reference<VKContext> mContext = nullptr;
    Reference<VKDevice> mDevice = nullptr;

    PipelineProperties Properties;
    vk::Pipeline Pipeline;
    vk::PipelineLayout Layout;

    // ToDo: These are needed for clean switch between contexts
    vk::Buffer VertexBuffer;
    vk::DeviceMemory VertexMemory;
};

}

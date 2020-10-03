#pragma once

#include <vulkan/vulkan.hpp>

#include "Ultra/Renderer/Pipeline.h"

namespace Ultra {

class VKPipeline: public Pipeline {
public:
    VKPipeline(const PipelineProperties &properties);
    virtual ~VKPipeline();

    virtual void Bind() override;
    virtual void Invalidate() override;

    virtual PipelineProperties &GetProperties() override;
    virtual const PipelineProperties &GetProperties() const override;

    vk::Pipeline GetNativePipeline();
    vk::PipelineLayout GetNativePipelineLayout();

private:
    PipelineProperties Properties;
    uint32_t VertexArrayRendererID = 0;

    vk::Pipeline Pipeline;
    vk::PipelineLayout Layout;
};

}

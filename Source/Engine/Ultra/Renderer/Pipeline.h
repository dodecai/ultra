#pragma once

#include "Buffer.h"
#include "RenderPass.h"
#include "Shader.h"

namespace Ultra {

struct PipelineProperties {
    BufferLayout Layout;
    Reference<RenderPass> RenderPass;
    Reference<Shader> Shader;
};

class Pipeline {
public:
    Pipeline() = default;
    virtual ~Pipeline() = default;

    static Reference<Pipeline> Create(const PipelineProperties &properties);

    virtual PipelineProperties &GetProperties() = 0;
    virtual const PipelineProperties &GetProperties() const = 0;

    virtual void Bind() = 0;
    virtual void Invalidate() = 0;

};

}

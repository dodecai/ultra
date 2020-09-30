#pragma once

#include "Ultra/Renderer/Pipeline.h"

namespace Ultra {

class GLPipeline: public Pipeline {
public:
    GLPipeline(const PipelineProperties &properties);
    virtual ~GLPipeline();

    virtual void Bind() override;
    virtual void Invalidate() override;

    virtual PipelineProperties &GetProperties() override;
    virtual const PipelineProperties &GetProperties() const override;

private:
    PipelineProperties Properties;
    uint32_t VertexArrayRendererID = 0;
};

}

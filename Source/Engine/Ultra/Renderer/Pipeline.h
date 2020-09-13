#pragma once

#include "Buffer.h"
#include "Shader.h"

namespace Ultra {

struct PipelineProperties {
    Reference<Shader> Shader;
};

class Pipeline {
public:
    Pipeline() = default;
    virtual ~Pipeline() = default;

    static Reference<Pipeline> Create(const PipelineProperties &properties);

    virtual PipelineProperties &GetProperties() {}
    virtual const PipelineProperties &GetProperties() const {}

    virtual void Bind() {}
    virtual void Invalidate() {}

};

}

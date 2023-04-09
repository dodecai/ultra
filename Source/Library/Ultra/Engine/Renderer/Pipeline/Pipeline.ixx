module;

export module Ultra.Engine.Pipeline;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.GFX.Context;

import Ultra.Engine.Buffer;
import Ultra.Engine.RenderPass;
import Ultra.Engine.Shader;

export namespace Ultra {

struct PipelineProperties {
    //BufferLayout Layout;
    Reference<RenderPass> RenderPass;
    Reference<Shader> Shader;
};

class Pipeline {
public:
    Pipeline() = default;
    ~Pipeline() = default;

    static Reference<Pipeline> Create(const PipelineProperties &properties);

    virtual PipelineProperties &GetProperties() = 0;
    virtual const PipelineProperties &GetProperties() const = 0;

    virtual void Bind() = 0;
    virtual void Invalidate() = 0;
};

}

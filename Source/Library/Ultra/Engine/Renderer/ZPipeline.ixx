module;

export module Ultra.Engine.Pipeline;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.GFX.Context;
export import Ultra.Renderer.Data;
export import Ultra.Renderer.Buffer;
export import Ultra.Renderer.Shader;

export namespace Ultra {

struct PipelineProperties {
    VertexBufferLayout Layout;
    Reference<Shader> Shader;
};

class Pipeline {
public:
    Pipeline() = default;
    ~Pipeline() = default;

    static Reference<Pipeline> Create(const PipelineProperties &properties);

    virtual PipelineProperties &GetProperties() = 0;
    virtual const PipelineProperties &GetProperties() const = 0;

    virtual void Bind() const = 0;
    virtual void Invalidate() = 0;
    virtual void Unbind() const = 0;
};

}

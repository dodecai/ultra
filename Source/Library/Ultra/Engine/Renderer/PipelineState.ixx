export module Ultra.Engine.Renderer.PipelineState;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.Engine.Renderer.Data;
export import Ultra.Engine.Renderer.Buffer;
export import Ultra.Engine.Renderer.Shader;

export namespace Ultra {

enum class CullMode {
    None,
    Front,
    Back
};

enum class BlendMode {
    None,
    Alpha,
    Additive,
    Multiply
};

struct PipelineProperties {
    VertexBufferLayout Layout;
    Reference<Shader> Shader;
    CullMode CullMode;
    BlendMode BlendMode;
};

/// 
/// @brief Agnostic Pipeline
///
/// @example: How-To
/// auto pipeline = PipelineState::Create(CullMode::Back, BlendMode::Alpha)
/// pipeline->Apply
/// 
class PipelineState {
protected:
    PipelineState(const PipelineProperties &properties): mProperties(properties) {}

public:
    virtual ~PipelineState() = default;

    static Scope<PipelineState> Create(const PipelineProperties &properties);

    virtual void Apply() = 0;
    virtual void Bind() = 0;
    virtual void Unbind() = 0;


protected:
    PipelineProperties mProperties;
    RendererID mPipelineID;
};

}

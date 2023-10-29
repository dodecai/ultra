export module Ultra.Renderer.PipelineState;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.Renderer.Data;
export import Ultra.Renderer.Buffer;
export import Ultra.Renderer.Shader;

export namespace Ultra {

enum class CullMode {
    None,
    Back,
    Front,
    BackAndFront,
};

enum class FrontFace {
    CCW,
    CW,
};

enum class BlendMode {
    Additive,
    Alpha,
    Disabled,
    Multiply,   // PreMultiplyAlpha
};

struct PipelineProperties {
    BlendMode BlendMode = BlendMode::Disabled;
    CullMode CullMode = CullMode::None;
    bool DepthTest = false;
    bool DepthWritable = true;
    bool Wireframe = false;

    VertexBufferLayout Layout;
    Reference<Shader> Shader;
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

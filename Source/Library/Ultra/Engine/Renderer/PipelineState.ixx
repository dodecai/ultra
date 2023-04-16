export module Ultra.Renderer.PipelineState;

import Ultra.Core;
import Ultra.Logger;

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

/// 
/// @brief Agnostic Pipeline
///
/// @example: How-To
/// auto pipeline = PipelineState::Create(CullMode::Back, BlendMode::Alpha)
/// pipeline->Apply
/// 
class PipelineState {
protected:
    PipelineState(CullMode cullMode, BlendMode blendMode): mCullMode(cullMode), mBlendMode(blendMode) {}

public:
    virtual ~PipelineState() = default;

    static Scope<PipelineState> Create(CullMode cullMode, BlendMode blendMode);

    virtual void Apply() = 0;

protected:
    CullMode mCullMode;
    BlendMode mBlendMode;
};

}

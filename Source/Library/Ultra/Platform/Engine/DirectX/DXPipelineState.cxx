module Ultra.Platform.Renderer.DXPipelineState;

namespace Ultra {

DXPipelineState::DXPipelineState(CullMode cullMode, BlendMode blendMode): PipelineState(cullMode, blendMode) {}

DXPipelineState::~DXPipelineState() {}

void DXPipelineState::Apply() {}

}

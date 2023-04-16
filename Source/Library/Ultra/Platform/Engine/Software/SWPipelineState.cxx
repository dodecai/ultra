module Ultra.Platform.Renderer.SWPipelineState;

namespace Ultra {

SWPipelineState::SWPipelineState(CullMode cullMode, BlendMode blendMode): PipelineState(cullMode, blendMode) {}

SWPipelineState::~SWPipelineState() {}

void SWPipelineState::Apply() {}

}

module Ultra.Platform.Renderer.VKPipelineState;

namespace Ultra {

VKPipelineState::VKPipelineState(CullMode cullMode, BlendMode blendMode): PipelineState(cullMode, blendMode) {}

VKPipelineState::~VKPipelineState() {}

void VKPipelineState::Apply() {}

}

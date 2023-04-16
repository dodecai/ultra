module Ultra.Platform.Renderer.GLPipelineState;

namespace Ultra {

GLPipelineState::GLPipelineState(CullMode cullMode, BlendMode blendMode): PipelineState(cullMode, blendMode) {}

GLPipelineState::~GLPipelineState() {}

void GLPipelineState::Apply() {}

}

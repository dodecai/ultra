module;

#include <glad/gl.h>

module Ultra.Platform.Renderer.GLPipelineState;

namespace Ultra {

GLPipelineState::GLPipelineState(CullMode cullMode, BlendMode blendMode): PipelineState(cullMode, blendMode) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

GLPipelineState::~GLPipelineState() {}

void GLPipelineState::Apply() {}

}

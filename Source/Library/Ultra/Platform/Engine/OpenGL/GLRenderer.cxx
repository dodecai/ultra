module Ultra.Platform.GLRenderer;

namespace Ultra {

GLRenderer::GLRenderer() {}

GLRenderer::~GLRenderer() {}


void GLRenderer::Load() {};

void GLRenderer::LoadShader(ShaderType type, const string &source, const string &entryPoint) {
    auto shader = mRenderDevice->CreateShader(type, source, entryPoint);
    shader->Compile();
    shader->Bind();
}

void GLRenderer::RenderFrame() {};

void GLRenderer::Dispose() {};

}

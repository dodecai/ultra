module Ultra.Platform.DXRenderer;

namespace Ultra {

DXRenderer::DXRenderer() {}

DXRenderer::~DXRenderer() {}


void DXRenderer::Load() {};

void DXRenderer::LoadShader(ShaderType type, const string &source, const string &entryPoint) {
    auto shader = mRenderDevice->CreateShader(type, source, entryPoint);
    shader->Compile();
    shader->Bind();
}

void DXRenderer::RenderFrame() {};

void DXRenderer::Dispose() {};

}

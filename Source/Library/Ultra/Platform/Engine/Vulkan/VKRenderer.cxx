module Ultra.Platform.VKRenderer;

namespace Ultra {

VKRenderer::VKRenderer() {}

VKRenderer::~VKRenderer() {}


void VKRenderer::Load() {};

void VKRenderer::LoadShader(ShaderType type, const string &source, const string &entryPoint) {
    auto shader = mRenderDevice->CreateShader(type, source, entryPoint);
    shader->Compile();
    shader->Bind();
}

void VKRenderer::RenderFrame() {};

void VKRenderer::Dispose() {};

}

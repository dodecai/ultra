module Ultra.Platform.Renderer.DXRenderDevice;

import Ultra.Renderer;

namespace Ultra {

DXRenderDevice::DXRenderDevice() {}

DXRenderDevice::~DXRenderDevice() {}


Scope<Shader> DXRenderDevice::CreateShader(ShaderType type, const string &source, const string &entryPoint) {
    return Shader::Create(type, source, entryPoint);
}


void DXRenderDevice::Load() {};

void DXRenderDevice::BeginFrame() {};

void DXRenderDevice::EndFrame() {};

void DXRenderDevice::Dispose() {};

}

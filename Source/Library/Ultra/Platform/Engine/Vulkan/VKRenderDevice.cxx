module Ultra.Platform.Renderer.VKRenderDevice;

import Ultra.Renderer;

namespace Ultra {

VKRenderDevice::VKRenderDevice() {}

VKRenderDevice::~VKRenderDevice() {}


Scope<Shader> VKRenderDevice::CreateShader(ShaderType type, const string &source, const string &entryPoint) {
    return Shader::Create(type, source, entryPoint);
}


void VKRenderDevice::Load() {};

void VKRenderDevice::BeginFrame() {};

void VKRenderDevice::EndFrame() {};

void VKRenderDevice::Dispose() {};

}

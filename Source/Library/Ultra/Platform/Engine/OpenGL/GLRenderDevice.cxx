module Ultra.Platform.Renderer.GLRenderDevice;

import Ultra.Renderer;

namespace Ultra {

GLRenderDevice::GLRenderDevice() {}

GLRenderDevice::~GLRenderDevice() {}


Scope<Shader> GLRenderDevice::CreateShader(ShaderType type, const string &source, const string &entryPoint) {
    return Shader::Create(type, source, entryPoint);
}


void GLRenderDevice::Load() {};

void GLRenderDevice::BeginFrame() {};

void GLRenderDevice::EndFrame() {};

void GLRenderDevice::Dispose() {};

}

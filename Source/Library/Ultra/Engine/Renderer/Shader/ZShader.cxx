module Ultra.Engine.ZShader;

import Ultra.Platform.Engine.ZGLShader;

namespace Ultra {

Reference<ZShader> ZShader::Create(const string &source) {
    switch (Context::API) {
        case GraphicsAPI::Null: { return nullptr; }
        case GraphicsAPI::OpenGL: { return CreateReference<ZGLShader>(source); }
        //case GraphicsAPI::Vulkan: { return CreateReference<VKShader>(source); }
        default: { break; }
    }
    LogFatal("[Engine::Renderer::Shader] ", "The current graphics API doesn't support Shaders!");
    return nullptr;
}

}

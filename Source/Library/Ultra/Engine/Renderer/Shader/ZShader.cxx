module Ultra.Engine.Shader;

import Ultra.Platform.Engine.GLShader;

namespace Ultra {

Reference<Shader> Shader::Create(const string &source) {
    switch (Context::API) {
        case GraphicsAPI::Null: { return nullptr; }
        case GraphicsAPI::OpenGL: { return CreateReference<GLShader>(source); }
        //case GraphicsAPI::Vulkan: { return CreateReference<VKShader>(source); }
        default: { break; }
    }
    LogFatal("[Engine::Renderer::Shader] ", "The current graphics API doesn't support Shaders!");
    return nullptr;
}

}

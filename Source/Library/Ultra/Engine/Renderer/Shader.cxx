module Ultra.Renderer.Shader;

import Ultra.GFX.Context;
import Ultra.Platform.Renderer.DXShader;
import Ultra.Platform.Renderer.GLShader;
import Ultra.Platform.Renderer.VKShader;

namespace Ultra {

Scope<Shader> Shader::Create(ShaderType type, const string &source, const string &entryPoint) {
    switch (Context::API) {
        case GraphicsAPI::DirectX:  { return CreateScope<DXShader>(type, source, entryPoint); }
        case GraphicsAPI::OpenGL:   { return CreateScope<GLShader>(type, source, entryPoint); }
        case GraphicsAPI::Vulkan:   { return CreateScope<VKShader>(type, source, entryPoint); }

        default: {
        #if APP_MODE_DEBUG
            throw std::runtime_error("Renderer::Shader: RenderAPI not supported!");
        #else
            LogFatal("Renderer::Shader: RenderAPI not supported!");
        #endif
            return nullptr;
        }
    }
}

}

module Ultra.Engine.Renderer.Framebuffer;

import Ultra.GFX.Context;
import Ultra.Platform.Renderer.DXFramebuffer;
import Ultra.Platform.Renderer.GLFramebuffer;
import Ultra.Platform.Renderer.VKFramebuffer;

namespace Ultra {

Scope<Framebuffer> Framebuffer::Create(uint32_t width, uint32_t height, TextureFormat format) {
    switch (Context::API) {
        case GraphicsAPI::DirectX:  { return CreateScope<DXFramebuffer>(width, height, format); }
        case GraphicsAPI::OpenGL:   { return CreateScope<GLFramebuffer>(width, height, format); }
        case GraphicsAPI::Vulkan:   { return CreateScope<VKFramebuffer>(width, height, format); }

        default: {
        #if APP_MODE_DEBUG
            throw std::runtime_error("Renderer::Framebuffer: RenderAPI not supported!");
        #else
            LogFatal("Renderer::Framebuffer: RenderAPI not supported!");
            return nullptr;
        #endif
        }
    }
}

}

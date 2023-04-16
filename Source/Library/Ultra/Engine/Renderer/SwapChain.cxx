module Ultra.Renderer.Swapchain;

import Ultra.GFX.Context;
import Ultra.Platform.Renderer.DXSwapchain;
import Ultra.Platform.Renderer.GLSwapchain;
import Ultra.Platform.Renderer.VKSwapchain;

namespace Ultra {

Scope<Swapchain> Swapchain::Create(void *windowHandle, uint32_t width, uint32_t height) {
    switch (Context::API) {
        case GraphicsAPI::DirectX:  { return CreateScope<DXSwapchain>(windowHandle, width, height); }
        case GraphicsAPI::OpenGL:   { return CreateScope<GLSwapchain>(windowHandle, width, height); }
        case GraphicsAPI::Vulkan:   { return CreateScope<VKSwapchain>(windowHandle, width, height); }

        default: {
        #if APP_MODE_DEBUG
            throw std::runtime_error("Renderer::Swapchain: RenderAPI not supported!");
        #else
            LogFatal("Renderer::Swapchain: RenderAPI not supported!");
        #endif
            return nullptr;
        }
    }
}

}

module Ultra.Renderer;

import Ultra.GFX.Context;
import Ultra.Platform.DXRenderer;
import Ultra.Platform.GLRenderer;
import Ultra.Platform.VKRenderer;
import Ultra.Platform.Renderer.DXRenderDevice;
import Ultra.Platform.Renderer.GLRenderDevice;
import Ultra.Platform.Renderer.VKRenderDevice;

namespace Ultra {

Scope<Renderer> Renderer::Create(RenderAPI api) {
    Scope<Renderer> renderer;
    Scope<RenderDevice> device;

    switch (api) {
        case RenderAPI::DirectX: {
            renderer = CreateScope<DXRenderer>();
            device = CreateScope<DXRenderDevice>();
        }
        case RenderAPI::OpenGL: {
            renderer = CreateScope<GLRenderer>();
            device = CreateScope<GLRenderDevice>();
        }
        case RenderAPI::Vulkan: {
            renderer = CreateScope<VKRenderer>();
            device = CreateScope<VKRenderDevice>();
        }
        default: {
        #if APP_MODE_DEBUG
            throw std::runtime_error("Renderer: RenderAPI not supported!");
        #else
            LogFatal("Renderer: RenderAPI not supported!");
        #endif
        }
    }
    renderer->mRenderDevice = std::move(device);
    return renderer;
}

}

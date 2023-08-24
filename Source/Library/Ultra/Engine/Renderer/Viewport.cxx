module Ultra.Engine.Renderer.Viewport;

import Ultra.GFX.Context;
import Ultra.Platform.Engine.Renderer.GLViewport;

namespace Ultra {

Scope<Viewport> Viewport::Create(const ViewportProperties &properties) {
    switch (Context::API) {
        //case GraphicsAPI::DirectX:  { return CreateScope<DXViewport>(properties); }
        case GraphicsAPI::OpenGL:   { return CreateScope<GLViewport>(properties); }
        //case GraphicsAPI::Vulkan:   { return CreateScope<VKViewport>(properties); }

        default: {
            #if APP_MODE_DEBUG
                throw std::runtime_error("Renderer::Viewport: RenderAPI not supported!");
            #else
                LogFatal("RenderAPI not supported!");
                return nullptr;
            #endif
        }
    }
}

}

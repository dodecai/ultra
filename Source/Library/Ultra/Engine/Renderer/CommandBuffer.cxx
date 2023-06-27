module Ultra.Engine.Renderer.CommandBuffer;

import Ultra.GFX.Context;
import Ultra.Platform.Renderer.DXCommandBuffer;
import Ultra.Platform.Renderer.GLCommandBuffer;
import Ultra.Platform.Renderer.VKCommandBuffer;

namespace Ultra {

Scope<CommandBuffer> CommandBuffer::Create() {
    switch (Context::API) {
        case GraphicsAPI::DirectX:  { return CreateScope<DXCommandBuffer>(); }
        case GraphicsAPI::OpenGL:   { return CreateScope<GLCommandBuffer>(); }
        case GraphicsAPI::Vulkan:   { return CreateScope<VKCommandBuffer>(); }

        default: {
        #if APP_MODE_DEBUG
            throw std::runtime_error("Renderer::CommandBuffer: RenderAPI not supported!");
        #else
            LogFatal("Renderer::CommandBuffer: RenderAPI not supported!");
            return nullptr;
        #endif
        }
    }
}

}

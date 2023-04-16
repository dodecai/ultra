﻿module Ultra.Renderer.Buffer;

import Ultra.GFX.Context;
import Ultra.Platform.Renderer.DXBuffer;
import Ultra.Platform.Renderer.GLBuffer;
import Ultra.Platform.Renderer.VKBuffer;

namespace Ultra {

Scope<Buffer> Buffer::Create(BufferType type, const void *data, size_t size) {
    switch (Context::API) {
        case GraphicsAPI::DirectX:  { return CreateScope<DXBuffer>(type, data, size); }
        case GraphicsAPI::OpenGL:   { return CreateScope<GLBuffer>(type, data, size); }
        case GraphicsAPI::Vulkan:   { return CreateScope<VKBuffer>(type, data, size); }

        default: {
        #if APP_MODE_DEBUG
            throw std::runtime_error("Renderer::Buffer: RenderAPI not supported!");
        #else
            LogFatal("Renderer::Buffer: RenderAPI not supported!");
        #endif
            return nullptr;
        }
    }
}

}

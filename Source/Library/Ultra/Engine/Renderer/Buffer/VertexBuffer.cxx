module Ultra.Engine.VertexBuffer;

import Ultra.GFX.Context;
import Ultra.Platform.Engine.GLVertexBuffer;
//import Ultra.Platform.Engine.VKVertexBuffer;

namespace Ultra {

Reference<VertexBuffer> VertexBuffer::Create(uint32_t size, VertexBufferType type) {
    switch (Context::API) {
        case GraphicsAPI::Null: { return nullptr; }
        case GraphicsAPI::OpenGL: { return CreateReference<GLVertexBuffer>(size, type); }
        //case GraphicsAPI::Vulkan: { return CreateReference<VKVertexBuffer>(size, type); }
        default: { break; }
    }
    LogFatal("[Engine::Renderer::VertexBuffer] ", "The current graphics API doesn't support VertexBuffers!");
    return nullptr;
}

Reference<VertexBuffer> VertexBuffer::Create(void *data, uint32_t size, VertexBufferType type) {
    switch (Context::API) {
        case GraphicsAPI::Null: { return nullptr; }
        case GraphicsAPI::OpenGL: { return CreateReference<GLVertexBuffer>(data, size, type); }
        //case GraphicsAPI::Vulkan: { return CreateReference<VKVertexBuffer>(data, size, type); }
        default: { break; }
    }
    LogFatal("[Engine::Renderer::VertexBuffer] ", "The current graphics API doesn't support VertexBuffers!");
    return nullptr;
}

}

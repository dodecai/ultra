module Ultra.Engine.IndexBuffer;

import Ultra.GFX.Context;
import Ultra.Platform.Engine.GLIndexBuffer;
//import Ultra.Platform.Engine.VKVertexBuffer;

namespace Ultra {

Reference<IndexBuffer> IndexBuffer::Create(uint32_t size) {
    switch (Context::API) {
        case GraphicsAPI::Null: { return nullptr; }
        case GraphicsAPI::OpenGL: { return CreateReference<GLIndexBuffer>(size); }
        //case GraphicsAPI::Vulkan: { return CreateReference<VKIndexBuffer>(size); }
        default: { break; }
    }
    LogFatal("[Engine::Renderer::IndexBuffer] ", "The current graphics API doesn't support IndexBuffers!");
    return nullptr;
}

Reference<IndexBuffer> IndexBuffer::Create(void *data, uint32_t size) {
    switch (Context::API) {
        case GraphicsAPI::Null: { return nullptr; }
        case GraphicsAPI::OpenGL: { return CreateReference<GLIndexBuffer>(data, size); }
        //case GraphicsAPI::Vulkan: { return CreateReference<VKIndexBuffer>(data, size); }
        default: { break; }
    }
    LogFatal("[Engine::Renderer::IndexBuffer] ", "The current graphics API doesn't support IndexBuffers!");
    return nullptr;
}

}

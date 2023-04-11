module Ultra.Engine.IndexBuffer;

import Ultra.GFX.Context;
import Ultra.Platform.Engine.GLIndexBuffer;
//import Ultra.Platform.Engine.VKVertexBuffer;

namespace Ultra {

Reference<IndexBuffer> IndexBuffer::Create(size_t size) {
    switch (Context::API) {
        case GraphicsAPI::Null:     { return nullptr; }
        case GraphicsAPI::OpenGL:   { return CreateReference<GLIndexBuffer>(size); }
        //case GraphicsAPI::Vulkan: { return CreateReference<VKIndexBuffer>(size); }
        default: { break; }
    }
    LogFatal("Renderer::IndexBuffer: The current graphics API doesn't support IndexBuffers!");
    return nullptr;
}

Reference<IndexBuffer> IndexBuffer::Create(const uint32_t *data, size_t size) {
    switch (Context::API) {
        case GraphicsAPI::Null:     { return nullptr; }
        case GraphicsAPI::OpenGL:   { return CreateReference<GLIndexBuffer>(data, size); }
        //case GraphicsAPI::Vulkan: { return CreateReference<VKIndexBuffer>(data, size); }
        default: { break; }
    }
    LogFatal("Renderer::IndexBuffer: The current graphics API doesn't support IndexBuffers!");
    return nullptr;
}

void IndexBuffer::DetectCompressionLevel(const uint32_t *indices, size_t size) {
    auto maxValue = *std::max_element(indices, indices + size / sizeof(uint32_t));

    if (maxValue <= std::numeric_limits<uint8_t>::max()) {
        mProperties.Type = IndexType::UINT8;
    } else if (maxValue <= std::numeric_limits<uint16_t>::max()) {
        mProperties.Type = IndexType::UINT16;
    } else {
        mProperties.Type = IndexType::UINT32;
    }
}

}

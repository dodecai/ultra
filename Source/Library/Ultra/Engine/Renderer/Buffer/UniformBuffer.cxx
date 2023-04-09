module Ultra.Engine.UniformBuffer;

import Ultra.GFX.Context;
import Ultra.Platform.Engine.GLUniformBuffer;
//import Ultra.Platform.Engine.VKUniformBuffer;

namespace Ultra {

Reference<UniformBuffer> UniformBuffer::Create(size_t size, uint32_t binding) {
    switch (Context::API) {
        case GraphicsAPI::Null:     { return nullptr; }
        case GraphicsAPI::OpenGL:   { return CreateReference<GLUniformBuffer>(size, binding); }
        //case GraphicsAPI::Vulkan: { return CreateReference<VKUniformBuffer>(size, binding); }
        default: { break; }
    }
    LogFatal("[Renderer::UniformBuffer] ", "The current graphics API doesn't support UniformBuffers!");
    return nullptr;
}

}

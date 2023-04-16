module Ultra.Renderer.Texture;

import Ultra.GFX.Context;
import Ultra.Platform.Renderer.DXTexture;
import Ultra.Platform.Renderer.GLTexture;
import Ultra.Platform.Renderer.VKTexture;

namespace Ultra {

Scope<Texture> Texture::Create(const void *data, uint32_t width, uint32_t height, TextureFormat format) {
    switch (Context::API) {
        case GraphicsAPI::DirectX:  { return CreateScope<DXTexture>(data, width, height, format); }
        case GraphicsAPI::OpenGL:   { return CreateScope<GLTexture>(data, width, height, format); }
        case GraphicsAPI::Vulkan:   { return CreateScope<VKTexture>(data, width, height, format); }

        default: {
        #if APP_MODE_DEBUG
            throw std::runtime_error("Renderer::Texture: RenderAPI not supported!");
        #else
            LogFatal("Renderer::Texture: RenderAPI not supported!");
        #endif
            return nullptr;
        }
    }
}

}

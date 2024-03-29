﻿module Ultra.Renderer.Texture;

import Ultra.Graphics.Context;
import Ultra.Platform.Renderer.DXTexture;
import Ultra.Platform.Renderer.GLTexture;
import Ultra.Platform.Renderer.VKTexture;

namespace Ultra {

Scope<Texture> Texture::Create(const TextureProperties &properties, const void *data, size_t size) {
    switch (Context::API) {
        case GraphicsAPI::DirectX:  { return CreateScope<DXTexture>(properties, data, size); }
        case GraphicsAPI::OpenGL:   { return CreateScope<GLTexture>(properties, data, size); }
        case GraphicsAPI::Vulkan:   { return CreateScope<VKTexture>(properties, data, size); }

        default: {
        #if APP_MODE_DEBUG
            throw std::runtime_error("Renderer::Texture: RenderAPI not supported!");
        #else
            LogFatal("RenderAPI not supported!");
            return nullptr;
        #endif
        }
    }
}

Reference<Texture> Texture::Create(const TextureProperties &properties, const string &path) {
    switch (Context::API) {
        case GraphicsAPI::DirectX:  { return CreateReference<DXTexture>(properties, path); }
        case GraphicsAPI::OpenGL:   { return CreateReference<GLTexture>(properties, path); }
        case GraphicsAPI::Vulkan:   { return CreateReference<VKTexture>(properties, path); }

        default: {
        #if APP_MODE_DEBUG
            throw std::runtime_error("Renderer::Texture: RenderAPI not supported!");
        #else
            LogFatal("RenderAPI not supported!");
            return nullptr;
        #endif
        }
    }
}

}

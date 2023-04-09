module Ultra.Engine.Texture;

import Ultra.Platform.Engine.GLTexture;
//import Ultra.Platform.Engine.VKTexture;

namespace Ultra {

// Default
Reference<Texture2D> Texture2D::Create(const std::string &path) {
    switch (Context::API) {
        case GraphicsAPI::Null:		{ return nullptr; }
        case GraphicsAPI::OpenGL:	{ return CreateReference<GLTexture2D>(path); }
        //case GraphicsAPI::Vulkan:	{ return CreateReference<VKTexture2D>(path); }
        default:                    { break; }
	}
    LogFatal("[Engine::Renderer::Texture2D] ", "The current graphics API doesn't support 2D-Textrures!");
    return nullptr;
}

Reference<Texture2D> Texture2D::Create(const uint32_t width, const uint32_t height, TextureFormat format, TextureWrap wrap) {
    switch (Context::API) {
        case GraphicsAPI::Null:		{ return nullptr; }
        case GraphicsAPI::OpenGL:	{ return CreateReference<GLTexture2D>(width, height, format, wrap); }
        //case GraphicsAPI::Vulkan:	{ return CreateReference<VKTexture2D>(width, height, format, wrap); }
        default:                    { break; }
	}
    LogFatal("[Engine::Renderer::Texture2D] ", "The current graphics API doesn't support 2D-Textrures!");
    return nullptr;
}

Reference<Texture3D> Texture3D::Create(const std::string &path) {
    switch (Context::API) {
        case GraphicsAPI::Null:		{ return nullptr; }
        case GraphicsAPI::OpenGL:	{ return CreateReference<GLTexture3D>(path); }
        //case GraphicsAPI::Vulkan:	{ return CreateReference<VKTexture3D>(path); }
        default:                    { break; }
	}
    LogFatal("[Engine::Renderer::Texture2D] ", "The current graphics API doesn't support 3D-Textrures!");
    return nullptr;
}

Reference<Texture3D> Texture3D::Create(const uint32_t width, const uint32_t height, TextureFormat format, TextureWrap wrap) {
    switch (Context::API) {
        case GraphicsAPI::Null:		{ return nullptr; }
        case GraphicsAPI::OpenGL:	{ return CreateReference<GLTexture3D>(width, height, format, wrap); }
        //case GraphicsAPI::Vulkan:	{ return CreateReference<VKTexture3D>(width, height, format, wrap); }
        default:                    { break; }
	}
    LogFatal("[Engine::Renderer::Texture2D] ", "The current graphics API doesn't support 3D-Textrures!");
    return nullptr;
}

}

module Ultra.Engine.Texture;

import Ultra.Platform.Engine.GLTexture;
//import Ultra.Platform.Engine.VKTexture;

namespace Ultra {

///
/// @brief Texture2D
///

Reference<Texture2D> Texture2D::Create(const TextureProperties &properties) {
    switch (Context::API) {
        case GraphicsAPI::Null:		{ return nullptr; }
        case GraphicsAPI::OpenGL:	{ return CreateReference<GLTexture2D>(properties); }
        //case GraphicsAPI::Vulkan:	{ return CreateReference<VKTexture2D>(properties); }
        default:                    { break; }
	}
    LogFatal("[Engine::Renderer::Texture2D] ", "The current graphics API doesn't support 2D-Textrures!");
    return nullptr;
}
 
Reference<Texture2D> Texture2D::Create(const TextureProperties &properties, const void *data, size_t size) {
    switch (Context::API) {
        case GraphicsAPI::Null:		{ return nullptr; }
        case GraphicsAPI::OpenGL:	{ return CreateReference<GLTexture2D>(properties, data, size); }
        //case GraphicsAPI::Vulkan:	{ return CreateReference<VKTexture2D>(properties, data, size); }
        default:                    { break; }
	}
    LogFatal("[Engine::Renderer::Texture2D] ", "The current graphics API doesn't support 2D-Textrures!");
    return nullptr;
}

Reference<Texture2D> Texture2D::Create(const TextureProperties &properties, const string &path) {
    switch (Context::API) {
        case GraphicsAPI::Null:		{ return nullptr; }
        case GraphicsAPI::OpenGL:	{ return CreateReference<GLTexture2D>(properties, path); }
        //case GraphicsAPI::Vulkan:	{ return CreateReference<VKTexture2D>(properties, path); }
        default:                    { break; }
	}
    LogFatal("[Engine::Renderer::Texture2D] ", "The current graphics API doesn't support 2D-Textrures!");
    return nullptr;
}

///
/// @brief Texture3D
///

Reference<Texture3D> Texture3D::Create(const TextureProperties &properties) {
    switch (Context::API) {
        case GraphicsAPI::Null:		{ return nullptr; }
        case GraphicsAPI::OpenGL:	{ return CreateReference<GLTexture3D>(properties); }
        //case GraphicsAPI::Vulkan:	{ return CreateReference<VKTexture3D>(properties); }
        default:                    { break; }
	}
    LogFatal("[Engine::Renderer::Texture3D] ", "The current graphics API doesn't support 3D-Textrures!");
    return nullptr;
}
 
Reference<Texture3D> Texture3D::Create(const TextureProperties &properties, const void *data, size_t size) {
    switch (Context::API) {
        case GraphicsAPI::Null:		{ return nullptr; }
        case GraphicsAPI::OpenGL:	{ return CreateReference<GLTexture3D>(properties, data, size); }
        //case GraphicsAPI::Vulkan:	{ return CreateReference<VKTexture3D>(properties, data, size); }
        default:                    { break; }
	}
    LogFatal("[Engine::Renderer::Texture2D] ", "The current graphics API doesn't support 3D-Textrures!");
    return nullptr;
}

Reference<Texture3D> Texture3D::Create(const TextureProperties &properties, const string &path) {
    switch (Context::API) {
        case GraphicsAPI::Null:		{ return nullptr; }
        case GraphicsAPI::OpenGL:	{ return CreateReference<GLTexture3D>(properties, path); }
        //case GraphicsAPI::Vulkan:	{ return CreateReference<VKTexture3D>(properties, path); }
        default:                    { break; }
	}
    LogFatal("[Engine::Renderer::Texture2D] ", "The current graphics API doesn't support 3D-Textrures!");
    return nullptr;
}

}

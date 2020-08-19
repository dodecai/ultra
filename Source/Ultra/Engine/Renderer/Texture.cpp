#include "Texture.h"

#include "Renderer.h"
#include "Engine/Platform/OpenGL/GLTexture.h"

namespace Ultra {

// Default
Reference<Texture2D> Texture2D::Create(const std::string &path) {
	switch (Renderer::GetAPI()) {
		case RendererAPI::API::Null:		{ return nullptr; }
		case RendererAPI::API::OpenGL:		{ return CreateReference<GLTexture2D>(path); }
		default: {
			AppLogCritical("[Ultra::Texture2D::Create]: ", "The requested renderer API is currently not supported!");
			return nullptr;
		}
	}
}

Reference<Texture2D> Texture2D::Create(const uint32_t width, const uint32_t height, TextureFormat format, TextureWrap wrap) {
	switch (Renderer::GetAPI()) {
		case RendererAPI::API::Null:		{ return nullptr; }
		case RendererAPI::API::OpenGL:		{ return CreateReference<GLTexture2D>(width, height, format, wrap); }
		default: {
			AppLogCritical("[Ultra::Texture2D::Create]: ", "The requested renderer API is currently not supported!");
			return nullptr;
		}
	}
}

Reference<Texture3D> Texture3D::Create(const std::string &path) {
	switch (Renderer::GetAPI()) {
		case RendererAPI::API::Null:		{ return nullptr; }
		case RendererAPI::API::OpenGL:		{ return CreateReference<GLTexture3D>(path); }
		default: {
			AppLogCritical("[Ultra::Texture3D::Create]: ", "The requested renderer API is currently not supported!");
			return nullptr;
		}
	}
}

Reference<Texture3D> Texture3D::Create(const uint32_t width, const uint32_t height, TextureFormat format, TextureWrap wrap) {
	switch (Renderer::GetAPI()) {
		case RendererAPI::API::Null:		{ return nullptr; }
		case RendererAPI::API::OpenGL:		{ return CreateReference<GLTexture3D>(width, height, format, wrap); }
		default: {
			AppLogCritical("[Ultra::Texture3D::Create]: ", "The requested renderer API is currently not supported!");
			return nullptr;
		}
	}
}

}

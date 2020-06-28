#include "Texture.h"
#include "Engine/Platform/OpenGL/GLTexture.h"

#include "Renderer.h"

namespace Ultra {

Reference<Texture2D> Texture2D::Create(const std::string &path) {
	switch (Renderer::GetAPI()) {
		case RendererAPI::API::Null:		{ return nullptr; }
		case RendererAPI::API::OpenGL:		{ return CreateReference<GLTexture2D>(path); }
	}
	// Unknown RendererAPI
	return nullptr;
}

Reference<Texture2D> Texture2D::Create(const uint32_t width, const uint32_t height) {
	switch (Renderer::GetAPI()) {
		case RendererAPI::API::Null:		{ return nullptr; }
		case RendererAPI::API::OpenGL:		{ return CreateReference<GLTexture2D>(width, height); }
	}
	// Unknown RendererAPI
	return nullptr;
}

}

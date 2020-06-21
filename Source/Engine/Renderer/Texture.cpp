#include "Texture.h"

#include "Platform/OpenGL/GLTexture.h"
#include "Renderer.h"

namespace Ultra {

Texture2D *Texture2D::Create(const std::string &path) {
	switch (Renderer::GetAPI()) {
		case RendererAPI::API::Null:		{ return nullptr; }
		case RendererAPI::API::OpenGL:		{ return new GLTexture2D(path); }
	}
	// Unknown RendererAPI
	return nullptr;
}

}

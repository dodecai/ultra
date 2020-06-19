#include "RendererAPI.h"

#include "Platform/OpenGL/GLRendererAPI.h"


namespace Ultra {

RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

RendererAPI *RendererAPI::Create()
{
	switch (s_API) {
		case RendererAPI::API::Null:		{ return nullptr; }
		case RendererAPI::API::OpenGL:		{ return new GLRendererAPI(); }
	}
	// Unknown RendererAPI
	return nullptr;
}
}

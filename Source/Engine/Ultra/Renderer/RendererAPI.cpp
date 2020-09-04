#include "RendererAPI.h"
#include "Ultra/Platform/OpenGL/GLRendererAPI.h"

namespace Ultra {

// Properties
RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

// Default
RendererAPI *RendererAPI::Create() {
	switch (s_API) {
		case RendererAPI::API::Null:		{ return nullptr; }
		case RendererAPI::API::OpenGL:		{ return new GLRendererAPI(); }
	}
	AppLogCritical("[Ultra::RendererAPI::Create]: ", "The selected RendererAPI is currently not supported!");
	return nullptr;
}

}

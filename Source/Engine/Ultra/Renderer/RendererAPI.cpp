#include "RendererAPI.h"

#include "Ultra/Platform/OpenGL/GLRendererAPI.h"
#include "Ultra/Platform/Vulkan/VKRendererAPI.h"

namespace Ultra {

// Default
RendererAPI *RendererAPI::Create() {
    Context::API = GraphicsAPI::Vulkan;

	switch (Context::API) {
		case GraphicsAPI::Null:		{ return nullptr; }
		case GraphicsAPI::OpenGL:	{ return new GLRendererAPI(); }
        case GraphicsAPI::Vulkan:	{ return new VKRendererAPI(); }
        default:                    { break; }
	}
    AppLogCritical("[Engine::Renderer::API] ", "The selected RendererAPI is currently not supported!");
	return nullptr;
}

}

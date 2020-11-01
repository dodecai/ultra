#include "RendererAPI.h"

#include "Ultra/Platform/OpenGL/GLRendererAPI.h"
#include "Ultra/Platform/Vulkan/VKRendererAPI.h"

namespace Ultra {

// Default
RendererAPI *RendererAPI::Create() {
	switch (Context::API) {
		case GraphicsAPI::Null:		{ return nullptr; }
		case GraphicsAPI::OpenGL:	{ return new GLRendererAPI(); }
        case GraphicsAPI::Vulkan:	{ return new VKRendererAPI(); }
        default:                    { break; }
	}
    AppLogCritical("[Engine::Renderer::API] ", "The selected RendererAPI is currently not supported!");
	return nullptr;
}

const GraphicsAPI RendererAPI::GetAPI() {
    return Context::API;
}

void RendererAPI::SetAPI(const GraphicsAPI api) {
    Context::API = api;
}

}

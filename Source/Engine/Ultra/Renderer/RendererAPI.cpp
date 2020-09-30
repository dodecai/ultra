#include "RendererAPI.h"
#include "Ultra/Platform/OpenGL/GLRendererAPI.h"
#include "Ultra/Platform/Vulkan/VKRendererAPI.h"

namespace Ultra {

// Properties
RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

// Default
RendererAPI *RendererAPI::Create() {
    Context::API = GraphicsAPI::OpenGL;
	switch (Context::API) {
		case GraphicsAPI::Null:		    { return nullptr; }
		case GraphicsAPI::OpenGL:		{ return new GLRendererAPI(); }
        case GraphicsAPI::Vulkan:		{ return new VKRendererAPI(); }
	}
	AppLogCritical("[Ultra::RendererAPI::Create]: ", "The selected RendererAPI is currently not supported!");
	return nullptr;
}

}

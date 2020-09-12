#include "RendererAPI.h"
#include "Ultra/Platform/OpenGL/GLRendererAPI.h"
#include "Ultra/Platform/Vulkan/VKRendererAPI.h"


namespace Ultra {

// Properties
RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

// Default
RendererAPI *RendererAPI::Create() {
	switch (s_API) {
		case RendererAPI::API::Null:		{ return nullptr; }
		case RendererAPI::API::OpenGL:		{ return new GLRendererAPI(); }
        case RendererAPI::API::Vulkan:		{ return new VKRendererAPI(); }
	}
	AppLogCritical("[Ultra::RendererAPI::Create]: ", "The selected RendererAPI is currently not supported!");
	return nullptr;
}

}

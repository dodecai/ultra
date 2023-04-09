module Ultra.Engine.RendererAPI;

import Ultra.GFX.Context;
import Ultra.Platform.Engine.GLRendererAPI;
//import Ultra.Platform.Engine.VKRendererAPI;

namespace Ultra {

// Default
RendererAPI *RendererAPI::Create() {
	switch (Context::API) {
		case GraphicsAPI::Null:		{ return nullptr; }
		case GraphicsAPI::OpenGL:	{ return new GLRendererAPI(); }
        //case GraphicsAPI::Vulkan:	{ return new VKRendererAPI(); }
        default:                    { break; }
	}
    LogFatal("[Engine::Renderer::API] ", "The selected RendererAPI is currently not supported!");
	return nullptr;
}

const GraphicsAPI RendererAPI::GetAPI() {
    return Context::API;
}

void RendererAPI::SetAPI(const GraphicsAPI api) {
    Context::API = api;
}

}

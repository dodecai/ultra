#include "VertexArray.h"

#include "Renderer.h"
#include "Engine/Platform/OpenGL/GLVertexArray.h"

namespace Ultra {

// Default
Reference<VertexArray> VertexArray::Create() {
	switch (Renderer::GetAPI()) {
		case RendererAPI::API::Null:		{ return nullptr; }
		case RendererAPI::API::OpenGL:		{ return CreateReference<GLVertexArray>(); }
		default: {
			AppLogCritical("[Ultra::VertexArray::Create]: ", "The requested renderer API is currently not supported!");
			return nullptr;
		}
	}
}

}

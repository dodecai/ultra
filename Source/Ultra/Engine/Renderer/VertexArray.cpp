#include "VertexArray.h"
#include "Engine/Platform/OpenGL/GLVertexArray.h"

#include "Renderer.h"

namespace Ultra {

Reference<VertexArray> VertexArray::Create() {
	switch (Renderer::GetAPI()) {
		case RendererAPI::API::Null:		{ return nullptr; }
		case RendererAPI::API::OpenGL:		{ return CreateReference<GLVertexArray>(); }
	}
	// Unknown RendererAPI
	return nullptr;
}

}

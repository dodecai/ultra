#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/GLVertexArray.h"

namespace Ultra {

VertexArray *VertexArray::Create() {
	switch (Renderer::GetAPI()) {
		case RendererAPI::API::Null:		{ return nullptr; }
		case RendererAPI::API::OpenGL:		{ return new GLVertexArray(); }
	}
	// Unknown RendererAPI
	return nullptr;
}

}

#include "Buffer.h"
#include "Renderer.h"

#include "Engine/Platform/OpenGL/GLBuffer.h"

namespace Ultra {

Reference<VertexBuffer> VertexBuffer::Create(uint32_t size) {
	switch (Renderer::GetAPI()) {
		case RendererAPI::API::Null:		{ return nullptr; }
		case RendererAPI::API::OpenGL:		{ return CreateReference<GLVertexBuffer>(size); }
	}
	// Unknown RendererAPI
	return nullptr;
}

Reference<VertexBuffer> VertexBuffer::Create(float *vertices, uint32_t size) {
	switch (Renderer::GetAPI()) {
		case RendererAPI::API::Null:		{ return nullptr; }
		case RendererAPI::API::OpenGL:		{ return CreateReference<GLVertexBuffer>(vertices, size); }
	}
	// Unknown RendererAPI
	return nullptr;
}


Reference<IndexBuffer> IndexBuffer::Create(uint32_t *indices, uint32_t size) {
	switch (Renderer::GetAPI()) {
		case RendererAPI::API::Null:		{ return nullptr; }
		case RendererAPI::API::OpenGL:		{ return CreateReference<GLIndexBuffer>(indices, size); }
	}
	// Unknown RendererAPI
	return nullptr;
}

}

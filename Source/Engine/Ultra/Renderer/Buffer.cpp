#include "Buffer.h"
#include "Ultra/Platform/OpenGL/GLBuffer.h"

#include "Renderer.h"

namespace Ultra {

Reference<VertexBuffer> VertexBuffer::Create(uint32_t size, VertexBufferType type) {
	switch (Renderer::GetAPI()) {
		case RendererAPI::API::Null:		{ return nullptr; }
		case RendererAPI::API::OpenGL:		{ return CreateReference<GLVertexBuffer>(size, type); }
	}
	// Unknown RendererAPI
	return nullptr;
}

Reference<VertexBuffer> VertexBuffer::Create(void *data, uint32_t size, VertexBufferType type) {
	switch (Renderer::GetAPI()) {
		case RendererAPI::API::Null:		{ return nullptr; }
		case RendererAPI::API::OpenGL:		{ return CreateReference<GLVertexBuffer>(data, size, type); }
	}
	// Unknown RendererAPI
	return nullptr;
}


Reference<IndexBuffer> IndexBuffer::Create(uint32_t size) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::Null:		{ return nullptr; }
        case RendererAPI::API::OpenGL:		{ return CreateReference<GLIndexBuffer>(size); }
    }
    // Unknown RendererAPI
    return nullptr;
}

Reference<IndexBuffer> IndexBuffer::Create(void *data, uint32_t size) {
	switch (Renderer::GetAPI()) {
		case RendererAPI::API::Null:		{ return nullptr; }
		case RendererAPI::API::OpenGL:		{ return CreateReference<GLIndexBuffer>(data, size); }
	}
	// Unknown RendererAPI
	return nullptr;
}

}

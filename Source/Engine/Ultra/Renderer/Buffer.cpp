#include "Buffer.h"
#include "Renderer.h"

#include "Ultra/Platform/OpenGL/GLBuffer.h"
#include "Ultra/Platform/Vulkan/VKBuffer.h"

namespace Ultra {

Reference<VertexBuffer> VertexBuffer::Create(uint32_t size, VertexBufferType type) {
	switch (Context::API) {
		case GraphicsAPI::Null:		{ return nullptr; }
		case GraphicsAPI::OpenGL:	{ return CreateReference<GLVertexBuffer>(size, type); }
        case GraphicsAPI::Vulkan:	{ return CreateReference<VKVertexBuffer>(size, type); }
        default:                    { break; }
	}
    AppLogCritical("[Engine::Renderer::VertexBuffer] ", "The current graphics API doesn't support VertexBuffers!");
	return nullptr;
}

Reference<VertexBuffer> VertexBuffer::Create(void *data, uint32_t size, VertexBufferType type) {
    switch (Context::API) {
        case GraphicsAPI::Null:		{ return nullptr; }
        case GraphicsAPI::OpenGL:	{ return CreateReference<GLVertexBuffer>(data, size, type); }
        case GraphicsAPI::Vulkan:	{ return CreateReference<VKVertexBuffer>(data, size, type); }
        default:                    { break; }
	}
    AppLogCritical("[Engine::Renderer::VertexBuffer] ", "The current graphics API doesn't support VertexBuffers!");
	return nullptr;
}


Reference<IndexBuffer> IndexBuffer::Create(uint32_t size) {
    switch (Context::API) {
        case GraphicsAPI::Null:		{ return nullptr; }
        case GraphicsAPI::OpenGL:	{ return CreateReference<GLIndexBuffer>(size); }
        case GraphicsAPI::Vulkan:	{ return CreateReference<VKIndexBuffer>(size); }
        default:                    { break; }
    }
    AppLogCritical("[Engine::Renderer::IndexBuffer] ", "The current graphics API doesn't support IndexBuffers!");
    return nullptr;
}

Reference<IndexBuffer> IndexBuffer::Create(void *data, uint32_t size) {
    switch (Context::API) {
        case GraphicsAPI::Null:		{ return nullptr; }
        case GraphicsAPI::OpenGL:	{ return CreateReference<GLIndexBuffer>(data, size); }
        case GraphicsAPI::Vulkan:	{ return CreateReference<VKIndexBuffer>(data, size); }
        default:                    { break; }
	}
    AppLogCritical("[Engine::Renderer::IndexBuffer] ", "The current graphics API doesn't support IndexBuffers!");
	return nullptr;
}

}

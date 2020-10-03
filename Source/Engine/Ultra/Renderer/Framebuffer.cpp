#include "FrameBuffer.h"
#include "Ultra/Platform/OpenGL/GLFramebuffer.h"
#include "Ultra/Platform/Vulkan/VKFramebuffer.h"

#include "Renderer.h"

namespace Ultra {

Reference<Framebuffer> Framebuffer::Create(const FramebufferProperties &properties) {
    switch (Context::API) {
        case GraphicsAPI::Null:		{ return nullptr; }
        case GraphicsAPI::OpenGL:	{ return CreateReference<GLFramebuffer>(properties); }
        case GraphicsAPI::Vulkan:	{ return CreateReference<VKFramebuffer>(properties); }
        default:                    { break; }
	}
    AppLogCritical("[Engine::Renderer::Framebuffer] ", "The current graphics API doesn't support Framebuffers!");
	return nullptr;
}

FramebufferPool::FramebufferPool(uint32_t maxBuffers) {
    static FramebufferPool *instance = this;
    Instance = instance;
}

FramebufferPool::~FramebufferPool() {}

void FramebufferPool::Add(const Reference<Framebuffer> &framebuffer) {
    Pool.push_back(framebuffer);
}

weak_ptr<Framebuffer> FramebufferPool::Allocate() {
    // ToDo: Push back to Pool
	return weak_ptr<Framebuffer>();
}

}

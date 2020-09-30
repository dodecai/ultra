#include "FrameBuffer.h"
#include "Ultra/Platform/OpenGL/GLFramebuffer.h"

#include "Renderer.h"

namespace Ultra {

Reference<Framebuffer> Framebuffer::Create(const FramebufferProperties &properties) {
	switch (Renderer::GetAPI()) {
		case RendererAPI::API::Null:		{ return nullptr; }
		case RendererAPI::API::OpenGL:		{ return CreateReference<GLFramebuffer>(properties); }
	}
	// Unknown RendererAPI
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

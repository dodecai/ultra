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
}

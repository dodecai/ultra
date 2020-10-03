#include "RenderPass.h"

#include <Omnia/GFX/Context.h>

#include "Ultra/Platform/OpenGL/GLRenderPass.h"
#include "Ultra/Platform/Vulkan/VKRenderPass.h"

namespace Ultra {

Reference<RenderPass> RenderPass::Create(const RenderPassProperties &properties) {
    switch (Context::API) {
        case GraphicsAPI::Null:		{ return nullptr; }
        case GraphicsAPI::OpenGL:	{ return CreateReference<GLRenderPass>(properties); }
        case GraphicsAPI::Vulkan:	{ return CreateReference<VKRenderPass>(properties); }
        default:                    { break; }
    }
    AppLogCritical("[Engine::Renderer::RenderPass] ", "The current graphics API doesn't support RenderPasses!");
    return nullptr;
}

}

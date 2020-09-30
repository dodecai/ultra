#include "RenderPass.h"

#include <Omnia/GFX/Context.h>

#include "Ultra/Platform/OpenGL/GLRenderPass.h"

namespace Ultra {

Reference<RenderPass> RenderPass::Create(const RenderPassProperties &properties) {
    switch (Context::API) {
        case GraphicsAPI::Null:		    { return nullptr; }
        case GraphicsAPI::OpenGL:		{ return CreateReference<GLRenderPass>(properties); }
        //case GraphicsAPI::Vulkan:		{ return CreateReference<VKRenderPass>(properties); }
    }
    AppLogCritical("[Ultra::Renderer::RenderPass::Create]: ", "The selected graphics API is currently not supported!");
    return nullptr;
}

}

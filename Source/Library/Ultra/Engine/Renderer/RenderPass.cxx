module Ultra.Engine.RenderPass;

import Ultra.Platform.Engine.GLRenderPass;
//import Ultra.Platform.Engine.VKRenderPass;

namespace Ultra {

Reference<RenderPass> RenderPass::Create(const RenderPassProperties &properties) {
    switch (Context::API) {
        case GraphicsAPI::Null: { return nullptr; }
        case GraphicsAPI::OpenGL: { return CreateReference<GLRenderPass>(properties); }
        //case GraphicsAPI::Vulkan:	{ return CreateReference<VKRenderPass>(properties); }
        default: { break; }
    }
    LogFatal("[Engine::Renderer::RenderPass] ", "The current graphics API doesn't support RenderPass!");
    return nullptr;
}

}

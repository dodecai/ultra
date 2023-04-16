module Ultra.Renderer.PipelineState;

import Ultra.GFX.Context;
import Ultra.Platform.Renderer.DXPipelineState;
import Ultra.Platform.Renderer.GLPipelineState;
import Ultra.Platform.Renderer.VKPipelineState;

namespace Ultra {

Scope<PipelineState> PipelineState::Create(CullMode cullMode, BlendMode blendMode) {
    switch (Context::API) {
        case GraphicsAPI::DirectX:  { return CreateScope<DXPipelineState>(cullMode, blendMode); }
        case GraphicsAPI::OpenGL:   { return CreateScope<GLPipelineState>(cullMode, blendMode); }
        case GraphicsAPI::Vulkan:   { return CreateScope<VKPipelineState>(cullMode, blendMode); }

        default: {
        #if APP_MODE_DEBUG
            throw std::runtime_error("Renderer::PipelineState: RenderAPI not supported!");
        #else
            LogFatal("Renderer::PipelineState: RenderAPI not supported!");
        #endif
            return nullptr;
        }
    }
}

}

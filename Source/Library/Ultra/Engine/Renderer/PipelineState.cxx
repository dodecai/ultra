module Ultra.Engine.Renderer.PipelineState;

import Ultra.GFX.Context;
import Ultra.Platform.Renderer.DXPipelineState;
import Ultra.Platform.Renderer.GLPipelineState;
import Ultra.Platform.Renderer.VKPipelineState;

namespace Ultra {

Scope<PipelineState> PipelineState::Create(const PipelineProperties &properties) {
    switch (Context::API) {
        case GraphicsAPI::DirectX:  { return CreateScope<DXPipelineState>(properties); }
        case GraphicsAPI::OpenGL:   { return CreateScope<GLPipelineState>(properties); }
        case GraphicsAPI::Vulkan:   { return CreateScope<VKPipelineState>(properties); }

        default: {
        #if APP_MODE_DEBUG
            throw std::runtime_error("Renderer::PipelineState: RenderAPI not supported!");
        #else
            LogFatal("RenderAPI not supported!");
            return nullptr;
        #endif
        }
    }
}

}

﻿module Ultra.Renderer.RenderDevice;

import Ultra.GFX.Context;
import Ultra.Platform.Renderer.DXRenderDevice;
import Ultra.Platform.Renderer.GLRenderDevice;
import Ultra.Platform.Renderer.VKRenderDevice;

namespace Ultra {

Scope<RenderDevice> RenderDevice::Create() {
    switch (Context::API) {
        case GraphicsAPI::DirectX:  { return CreateScope<DXRenderDevice>(); }
        case GraphicsAPI::OpenGL:   { return CreateScope<GLRenderDevice>(); }
        case GraphicsAPI::Vulkan:   { return CreateScope<VKRenderDevice>(); }

        default: {
        #if APP_MODE_DEBUG
            throw std::runtime_error("Renderer::RenderDevice: RenderAPI not supported!");
        #else
            LogFatal("Renderer::RenderDevice: RenderAPI not supported!");
        #endif
            return nullptr;
        }
    }
}


//static void Reload() {
//    delete sRendererAPI;
//    sRendererAPI = RendererAPI::Create();
//    sRendererAPI->Load();
//}

}

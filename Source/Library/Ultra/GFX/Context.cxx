module Ultra.GFX.Context;

import Ultra.Core;
import Ultra.Logger;

#ifdef APP_PLATFORM_WINDOWS
    import Ultra.Platform.GFX.GLContext;
    import Ultra.Platform.GFX.VKSurface;
#endif

namespace Ultra {

Reference<Context> Context::Create(void *window) {
    #ifdef APP_PLATFORM_WINDOWS
    switch (API) {
        case GraphicsAPI::OpenGL: {
            LogDebug("[Application] ", "Created context for 'OpenGL'");
            return CreateReference<GLContext>(window);
            return nullptr;
        }

        case GraphicsAPI::Vulkan: {
            LogDebug("[Application] ", "Created context for 'Vulkan'");
            //return CreateReference<VKContext>(window);
            return nullptr;
        }

        default: {
            //AppAssert(false, "This API is currently not supportded!");
            return nullptr;
        }
    }
    #else
    //APP_ASSERT(false, "This platform is currently not supported!");
    return nullptr;
    #endif
}

void Context::Destroy() {
}

}

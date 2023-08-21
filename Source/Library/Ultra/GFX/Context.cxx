module;

#include "Ultra/Core/Core.h"

module Ultra.GFX.Context;

import Ultra.Core;
import Ultra.Logger;

#ifdef APP_PLATFORM_WINDOWS
    import Ultra.Platform.GFX.DXContext;
    import Ultra.Platform.GFX.GLContext;
    import Ultra.Platform.GFX.SWContext;
    import Ultra.Platform.GFX.VKContext;
#endif

namespace Ultra {

Reference<Context> Context::Create(void *window) {
#ifdef APP_PLATFORM_WINDOWS
    switch (API) {
        case GraphicsAPI::OpenGL: {
            LogDebug("Application: Created context for 'OpenGL'");
            return CreateReference<GLContext>(window);
        }

        case GraphicsAPI::Vulkan: {
            LogDebug("Application: Created context for 'Vulkan'");
            return CreateReference<VKContext>(window);
        }

        default: {
            AppAssert(false, "The selected graphics api isn't implemented!");
            return nullptr;
        }
    }
#else
    AppAssert(false, "This platform is currently not supported!");
    return nullptr;
#endif
}

void Context::Destroy() {}

}

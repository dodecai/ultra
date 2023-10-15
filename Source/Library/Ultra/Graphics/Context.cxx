module;

#include "Ultra/Core/Core.h"

module Ultra.Graphics.Context;

import Ultra.Logger;

#ifdef APP_PLATFORM_WINDOWS
    import Ultra.Platform.Graphics.DXContext;
    import Ultra.Platform.Graphics.GLContext;
    import Ultra.Platform.Graphics.SWContext;
    import Ultra.Platform.Graphics.VKContext;
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
    AppAssert(false, "The selected platform is currently not supported!");
    return nullptr;
#endif
}

void Context::Destroy() {}

}

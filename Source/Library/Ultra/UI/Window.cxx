module;

#include "Ultra/Core/Core.h"

module Ultra.UI.Window;

import Ultra.Logger;

#ifdef APP_PLATFORM_WINDOWS
    import Ultra.Platform.UI.WinAPI.Window;
#endif

namespace Ultra {

Scope<Window> Window::Create(const WindowProperties &properties) {
    #ifdef APP_PLATFORM_WINDOWS
        return CreateScope<WinWindow>(properties);
    #else
        AppAssert(nullptr, "The current platform isn't supported!");
        return nullptr;
    #endif
}

}

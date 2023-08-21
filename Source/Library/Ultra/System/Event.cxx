module;

// Hack: The included header unit in the Core module doesn't work here.
#include "Ultra/Core/Core.h"

module Ultra.System.Event;

import Ultra.Logger;

#ifdef APP_PLATFORM_WINDOWS
    import Ultra.Platform.System.WinAPI.Event;
#endif

namespace Ultra {

Scope<EventListener> EventListener::Create() {
    #ifdef APP_PLATFORM_WINDOWS
        return CreateScope<WinEventListener>();
    #else
        AppAssert(nullptr, "The current platform isn't supported!");
        return nullptr;
    #endif
}

}

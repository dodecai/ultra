﻿module;

#include "Ultra/Core/Core.h"
#if __INTELLISENSE__
    #include "Ultra/Core/Logger.h"
#endif

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
        //APP_ASSERT(nullptr, "The current platform isn't supported!");
        return nullptr;
    #endif
}

}

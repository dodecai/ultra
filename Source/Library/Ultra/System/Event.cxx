﻿#include <Ultra/Core/Private/Core.h> // ToDo: Since v17.6-preview.2 broken...

module Ultra.System.Event;

import Ultra.Logger;

#ifdef APP_PLATFORM_WINDOWS
    import Ultra.Platform.System.WinAPI.Event;
#endif

namespace Ultra {

Scope<EventListener> EventListener::Create() {
    #ifdef APP_PLATFORM_WINDOWS
        return CreateScope<WinEventListener>();
        return {};
    #else
        //APP_ASSERT(nullptr, "The current platform isn't supported!");
        return nullptr;
    #endif
}

}

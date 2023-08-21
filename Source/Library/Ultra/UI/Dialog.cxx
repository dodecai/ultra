module;

// Hack: The included header unit in the Core module doesn't work here.
#include "Ultra/Core/Core.h"

module Ultra.UI.Dialog;

import Ultra.Logger;

#ifdef APP_PLATFORM_WINDOWS
    import Ultra.Platform.UI.WinAPI.Dialog;
#endif

namespace Ultra {

Scope<Dialog> Dialog::Create() {
    #ifdef APP_PLATFORM_WINDOWS
        return CreateScope<WinDialog>();
    #else
        AppAssert(nullptr, "The current platform isn't supported!");
        return nullptr;
    #endif
}

}

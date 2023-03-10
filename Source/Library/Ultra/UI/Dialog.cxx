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
        //APP_ASSERT(nullptr, "The current platform isn't supported!");
        return nullptr;
    #endif
}

}

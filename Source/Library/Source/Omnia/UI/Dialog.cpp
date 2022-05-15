#include "Dialog.h"

#ifdef APP_PLATFORM_WINDOWS
    #include "Platform/UI/WinAPI/WinDialog.h"
#endif

namespace Omnia {

Scope<Dialog> Dialog::Create() {
    #ifdef APP_PLATFORM_WINDOWS
        return CreateScope<WinDialog>();
    #else
        APP_ASSERT(nullptr, "The current platform isn't supported!");
        return nullptr;
    #endif
}

}

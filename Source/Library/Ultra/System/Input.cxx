module;

#include "Ultra/Core/Private/Core.h"
#if __INTELLISENSE__
    #include "Ultra/Core/Private/Logger.h"
#endif

module Ultra.System.Input;

#if defined(APP_PLATFORM_WINDOWS)
    import Ultra.Platform.System.WinAPI.Input;
#endif

namespace Ultra {

Scope<Input> Input::Instance = Input::Create();

Scope<Input> Input::Create() {
    #if defined(APP_PLATFORM_WINDOWS)
        return CreateScope<WinInput>();
    #else
        return nullptr;
    #endif
}

bool Input::GetKeyState(KeyCode code) {
    return Instance->GetKeyStatePlatform(code);
}

bool Input::GetMouseButtonState(MouseButton button) {
    return Instance->GetMouseButtonStatePlatform(button);
}

std::pair<float, float> Input::GetMousePosition() {
    return Instance->GetMousePositionPlatform();
}

}

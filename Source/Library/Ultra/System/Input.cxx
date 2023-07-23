module;

#include "Ultra/Core/Private/Core.h"

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


std::pair<float, float> Input::GetMousePositionDelta() {
    return Instance->GetMousePositionDeltaPlatform();
}

float Input::GetMouseWheelDelta() {
    return Instance->GetMouseWheelDeltaPlatform();
}

}

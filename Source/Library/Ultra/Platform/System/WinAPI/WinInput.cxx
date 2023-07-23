module;

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#undef APIENTRY
#include <Windows.h>

module Ultra.Platform.System.WinAPI.Input;

namespace Ultra {

static POINT sLastCursorPos {};
static float sWheelState {};

bool WinInput::GetKeyStatePlatform(KeyCode code) const {
    return (bool)::GetAsyncKeyState((int)code);
}

bool WinInput::GetMouseButtonStatePlatform(MouseButton button) const {
    switch (button) {
        case MouseButton::Left:		return (bool)::GetAsyncKeyState((int)VK_LBUTTON);
        case MouseButton::Middle:	return (bool)::GetAsyncKeyState((int)VK_MBUTTON);
        case MouseButton::Right:	return (bool)::GetAsyncKeyState((int)VK_RBUTTON);
        case MouseButton::X1:		return (bool)::GetAsyncKeyState((int)VK_XBUTTON1);
        case MouseButton::X2:		return (bool)::GetAsyncKeyState((int)VK_XBUTTON2);
        default:					return false;
    }
}

std::pair<float, float> WinInput::GetMousePositionPlatform() const {
    POINT current {};
    GetCursorPos(&current);
    ScreenToClient(GetActiveWindow(), &current);

    sLastCursorPos = current;
    return { static_cast<float>(current.x), static_cast<float>(current.y) };
}

std::pair<float, float> WinInput::GetMousePositionDeltaPlatform() const {
    POINT current {};
    GetCursorPos(&current);
    ScreenToClient(GetActiveWindow(), &current);

    current.x -= sLastCursorPos.x;
    current.y -= sLastCursorPos.y;

    sLastCursorPos = current;
    return { static_cast<float>(current.x), static_cast<float>(current.y) };
}

float WinInput::GetMouseWheelDeltaPlatform() const {
    return {};
}

}

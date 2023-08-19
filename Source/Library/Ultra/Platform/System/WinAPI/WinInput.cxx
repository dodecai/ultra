module;

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#undef APIENTRY
#include <Windows.h>

module Ultra.Platform.System.WinAPI.Input;

namespace Ultra {

static POINT sLastMousePosition {};
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

bool WinInput::GetMouseButtonStateDeltaPlatform(MouseButton button) const {
    switch (button) {
        case MouseButton::Left: {
            static thread_local bool last {};
            auto current = (bool)::GetAsyncKeyState((int)VK_LBUTTON);
            if (current != last) {
                last = current;
                return current;
            }
            break;
        }
        case MouseButton::Middle: {
            static thread_local bool last {};
            auto current = (bool)::GetAsyncKeyState((int)VK_MBUTTON);
            if (current != last) {
                last = current;
                return current;
            }
            break;
        }
        case MouseButton::Right: {
            static thread_local bool last {};
            auto current = (bool)::GetAsyncKeyState((int)VK_RBUTTON);
            if (current != last) {
                last = current;
                return current;
            }
            break;
        }
        case MouseButton::X1: {
            static thread_local bool last {};
            auto current = (bool)::GetAsyncKeyState((int)VK_XBUTTON1);
            if (current != last) {
                last = current;
                return current;
            }
            break;
        }
        case MouseButton::X2: {
            static thread_local bool last {};
            auto current = (bool)::GetAsyncKeyState((int)VK_XBUTTON2);
            if (current != last) {
                last = current;
                return current;
            }
            break;
        }
    }
    return false;
}

std::pair<float, float> WinInput::GetMousePositionPlatform() const {
    static thread_local POINT delta {};

    POINT current {};
    GetCursorPos(&current);
    ScreenToClient(GetActiveWindow(), &current);

    sLastMousePosition = current;
    return { static_cast<float>(current.x), static_cast<float>(current.y) };
}

std::pair<float, float> WinInput::GetMousePositionDeltaPlatform() const {
    static thread_local POINT delta {};

    POINT current {};
    GetCursorPos(&current);
    ScreenToClient(GetActiveWindow(), &current);

    current.x -= sLastMousePosition.x;
    current.y -= sLastMousePosition.y;

    return { static_cast<float>(current.x), static_cast<float>(current.y) };
}

float WinInput::GetMouseWheelDeltaPlatform() const {
    static thread_local float delta {};
    if (sMouseWheelDelta != delta) {
        delta = sMouseWheelDelta;
        return delta;
    }

    return {};
}

}

#include "Window.h"

#ifdef APP_PLATFORM_WINDOWS
	#include "Platform/UI/WinAPI/WinWindow.h"
#endif

namespace Omnia {

Scope<Window> Window::Create(const WindowProperties &properties) {
	#ifdef APP_PLATFORM_WINDOWS
		return CreateScope<WinWindow>(properties);
	#else
		APP_ASSERT(nullptr, "The current platform isn't supported!");
		return nullptr;
	#endif
}

}

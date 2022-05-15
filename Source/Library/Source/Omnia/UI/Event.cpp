#include "Event.h"
#include "Omnia/Log.h"

#ifdef APP_PLATFORM_WINDOWS
	#include "Platform/UI/WinAPI/WinEvent.h"
#endif

namespace Omnia {

Scope<EventListener> EventListener::Create() {
	#ifdef APP_PLATFORM_WINDOWS
		return CreateScope<WinEventListener>();
	#else
		APP_ASSERT(nullptr, "The current platform isn't supported!");
		return nullptr;
	#endif
}

}

#pragma once

#include "Platform.h"

///
/// @brief Library Extensions
///

namespace Ultra {

// Disable Warning Example
//#pragma warning(push)
//#pragma warning(disable: #)
//#pragma warning(pop)

// Export/Import for Dynamic Libraries
#ifdef APP_PLATFORM_WINDOWS
    #ifdef APP_SHARED_LIBRARY
        #define APP_API __declspec(dllexport)
    #else
        #define APP_API __declspec(dllimport)
    #endif
#else
	#error "This library currently supports only Windows!""
#endif

// DebugBreak Support
#ifdef APP_MODE_DEBUG
	#if defined(APP_PLATFORM_WINDOWS)
		#define APP_DEBUGBREAK() __debugbreak()
	#elif defined(APP_PLATFORM_LINUX)
		#include <signal.h>
		#define APP_DEBUGBREAK() raise(SIGTRAP)
	#else
		#define APP_DEBUGBREAK()
		#pragma message("#> Core: Platform doesn't support debug break!")
	#endif
#else
	#define APP_DEBUGBREAK()
#endif

}

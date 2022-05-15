#pragma once

#include "Omnia/Omnia.pch"

#include "Omnia.settings"
#include "Platform.h"
#include "Types.h"

#ifdef APP_PLATFORM_WINDOWS
    #ifdef APP_SHARED_LIBRARY
        #define APP_API __declspec(dllexport)
    #else
        #define APP_API __declspec(dllimport)
    #endif
#else
	#error "This library currently supports only Windows!""
#endif

#ifdef APP_DEBUG_MODE
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

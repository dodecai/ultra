#pragma once

///
/// @brief Preprocessor Utilities
///

// String
#define STRINGIZE(s) #s
#define TOSTRING(s) STRINGIZE(s)

///
/// @brief These properties are used to control platform specific features during compilation.
///

// Compiler
#if defined(_MSC_VER)
	#define APP_COMPILER_MSVC
	#define APP_COMPILER			"MSVC"
	#define APP_COMPILER_MAJOR		_MSC_VER
	#define APP_COMPILER_MINOR		0
	#define APP_COMPILER_PATCH		0
	#define APP_COMPILER_VERSION	"v" TOSTRING(APP_COMPILER_MAJOR)
    inline constexpr auto AppCompiler = "MSVC";
#elif defined(__clang__)
	#define APP_COMPILER_CLANG
	#define APP_COMPILER			"Clang"
	#define APP_COMPILER_MAJOR		__clang_major__
	#define APP_COMPILER_MINOR		__clang_minor__
	#define APP_COMPILER_PATCH		__clang_patchlevel__
	#define APP_COMPILER_VERSION	"v" TOSTRING(APP_COMPILER_MAJOR) "." TOSTRING(APP_COMPILER_MINOR) "." TOSTRING(APP_COMPILER_PATCH)
    inline constexpr auto AppCompiler = "Clang";
#elif defined(__EMSCRIPTEN__)
	#define APP_COMPILER_EMSCRIPTEN
	#define APP_COMPILER			"emscripten"
	#define APP_COMPILER_MAJOR		0
	#define APP_COMPILER_MINOR		0
	#define APP_COMPILER_PATCH		0
	#define APP_COMPILER_VERSION	"v" TOSTRING(APP_COMPILER_MAJOR) "." TOSTRING(APP_COMPILER_MINOR) "." TOSTRING(APP_COMPILER_PATCH)
    inline constexpr auto AppCompiler = "emscripten";
#elif defined(__GNUC__)
	#define APP_COMPILER_GNU
	#define APP_COMPILER			"GCC"
	#define APP_COMPILER_MAJOR		__GNUC__
	#define APP_COMPILER_MINOR		__GNUC_MINOR__
	#define APP_COMPILER_PATCH		0
	#define APP_COMPILER_VERSION	"v" TOSTRING(APP_COMPILER_MAJOR) "." TOSTRING(APP_COMPILER_MINOR)
    inline constexpr auto AppCompiler = "GCC";
#elif defined(__MINGW32__) || defined(__MINGW64__)
	#define APP_COMPILER_MINGW
	#define APP_COMPILER			"MinGW"
	#if defined(__MINGW32__)
		#define APP_COMPILER_MAJOR	__MINGW32_MAJOR_VERSION
		#define APP_COMPILER_MINOR	__MINGW32_MINOR_VERSION
	#else
		#define APP_COMPILER_MAJOR	__MINGW64_VERSION_MAJOR
		#define APP_COMPILER_MINOR	__MINGW64_VERSION_MINOR
	#endif
	#define APP_COMPILER_PATCH		0
	#define APP_COMPILER_VERSION "v" TOSTRING(APP_COMPILER_MAJOR) "." TOSTRING(APP_COMPILER_MINOR)
    inline constexpr auto AppCompiler = "MinGW";
#else
	#define APP_COMPILER_UNKNOWN
	#define APP_COMPILER			"Unknown"
	#define APP_COMPILER_MAJOR		0
	#define APP_COMPILER_MINOR		0
	#define APP_COMPILER_PATCH		0
	#define APP_COMPILER_VERSION	"v" TOSTRING(APP_COMPILER_MAJOR) "." TOSTRING(APP_COMPILER_MINOR) "." TOSTRING(APP_COMPILER_PATCH)
    inline constexpr auto AppCompiler = "Unknown";
#endif

// Platform
#if defined(_WIN32) || defined(_WIN64)
	#define APP_PLATFORM_WINDOWS
	#define APP_PLATFORM "Windows"
	inline constexpr auto AppPlatform = "Windows";
#elif defined(__ANDROID__)
	#define APP_PLATFORM_ANDROID
	#define APP_PLATFORM "Android"
    inline constexpr auto AppPlatform = "Android";
#elif defined(__FREEBSD__)
	#define APP_PLATFORM_BSD
	#define APP_PLATFORM "BSD"
    inline constexpr auto AppPlatform = "BSD";
#elif defined(__linux__) && !defined(__ANDROID__)
	#define APP_PLATFORM_LINUX
	#define APP_PLATFORM "Linux"
    inline constexpr auto AppPlatform = "Linux";
#elif defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_OS_IPHONE == 1
		#define APP_PLATFORM_IOS
		#define APP_PLATFORM "iOS"
        inline constexpr auto AppPlatform = "iOS";
	#elif TARGET_OS_MAC == 1
		#define APP_PLATFORM_MACOSX
		#define APP_PLATFORM "MacOSX"
        inline constexpr auto AppPlatform = "MacOSX";
	#endif
#elif !defined(__linux__) && defined(unix) || defined(__unix) || defined(__unix__)
	#define APP_PLATFORM_UNIX
	#define APP_PLATFORM "Unix"
    inline constexpr auto AppPlatform = "Unix";
#elif defined(APP_COMPILER_EMSCRIPTEN)
	#define APP_PLATFORM_WEB
	#define APP_PLATFORM "Web"
    inline constexpr auto AppPlatform = "Web";
#else
	#define APP_PLATFORM_UNKNOWN
	#define APP_PLATFORM "Unknown"
    inline constexpr auto AppPlatform = "Unknown";
#endif

// PlatformAPI
#if defined(APP_PLATFORM_WINDOWS)
	#define APP_PLATFORM_API_WINAPI
	#define APP_PLATFORM_API	"WinAPI"
	inline constexpr auto AppPlatformAPI = "WinAPI";
#elif defined(APP_PLATFORM_BSD) || defined(APP_PLATFORM_LINUX) || defined(APP_PLATFORM_UNIX)
	#define APP_PLATFORM_API_XCB
	#define APP_PLATFORM_API	"XCB"
    inline constexpr auto AppPlatformAPI = "XCB";
#elif defined(APP_PLATFORM_MACOSX)
	#define APP_PLATFORM_API_COCOA
	#define APP_PLATFORM_API	"Cocoa"
    inline constexpr auto AppPlatformAPI = "Cocoa";
#elif defined(APP_PLATFORM_WEB)
	#define APP_PLATFORM_API_COCOA
	#define APP_PLATFORM_API	"WASM"
    inline constexpr auto AppPlatformAPI = "WASM";
#else
	#define APP_PLATFORM_API_UNKNOWN
	#define APP_PLATFORM_API	"Unknown"
    inline constexpr auto AppPlatformAPI = "Unknown";
#endif

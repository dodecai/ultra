#pragma once

#include "Core.h"

///
/// Library Properties
///

// Switches to enable or disable Features
#define LIB_EXTENSION_PRIME     // Logging Extensions (Attention: cannot be disabled, cause they are used everywhere else)
#define LIB_EXTENSION_CORE      // Applicaiton Configuration, Layout, Workflow Extensions
#define LIB_EXTENSION_DEBUG     // Debugging and Profiling Extension
#define LIB_EXTENSION_GFX       // Graphics API selection and Context Binding Extensions
#define LIB_EXTENSION_SYSTEM    // System Extensions like color output in cli or input capturing
#define LIB_EXTENSION_UI        // UI extensions like window creation and event system
#define LIB_EXTENSION_UTILITIES // Usefull utilities like date and time, properties and timer helpers, etc.

namespace Ultra {

// Information
constexpr auto LibCaption       = "Ultra Library";
constexpr auto LibDescription   = "Game Engine Framework";
constexpr auto LibRelease       = "2022-07-02";
constexpr auto LibVersion       = "0.0.0";
constexpr auto LibVersionMajor  = 0u;
constexpr auto LibVersionMinor  = 0u;
constexpr auto LibVersionPatch  = 0u;

// Features
namespace Features {

constexpr bool LibPrimeExtensions = true;
#ifdef LIB_EXTENSION_CORE
    constexpr bool LibCoreExtensions = true;
#else
    constexpr bool LibCoreExtensions = false;
#endif
#ifdef LIB_EXTENSION_DEBUG
    constexpr bool LibDebugExtensions = true;
#else
    constexpr bool LibDebugExtensions = false;
#endif
#ifdef LIB_EXTENSION_GFX
    constexpr bool LibGfxExtensions = true;
#else
    constexpr bool LibGfxExtensions = false;
#endif
#ifdef LIB_EXTENSION_SYSTEM
    constexpr bool LibSystemExtensions = true;
#else
    constexpr bool LibSystemExtensions = false;
#endif
#ifdef LIB_EXTENSION_UI
    constexpr bool LibUiExtensions = true;
#else
    constexpr bool LibUiExtensions = false;
#endif
#ifdef LIB_EXTENSION_UTILITIES
    constexpr bool LibUtilityExtensions = true;
#else
    constexpr bool LibUtilityExtensions = false;
#endif

}

}

// Show platform specific information, the preprocessor value is defined in 'Core.h', therefore it should be included first)
#ifdef APP_COMPILER_MSVC
	#pragma message("#> Compiler: " APP_COMPILER " [" APP_COMPILER_VERSION "]")
    #pragma message("#> Platform: " APP_PLATFORM)
    #pragma message("#> PlatformAPI: " APP_PLATFORM_API)
#else
    // ToDo: Support for other compilers when they are VS C++20 module ready.
#endif

#pragma once

#include "Ultra/Core/Core.h"

///
/// Library Properties
///

// Switches to enable or disable Features
//#define LIB_EXTENSION_PRIME   // Prime Extensions like Types (cannot be disabled)
#define LIB_EXTENSION_CORE      // Application Configuration, Layout, Workflow and Utility Extensions
#define LIB_EXTENSION_DEBUG     // Debugging and Profiling Extension
#define LIB_EXTENSION_ENGINE    // Graphics API selection and Context Binding Extensions (implies ui extensions)
#define LIB_EXTENSION_SYSTEM    // System Extensions like color output in cli, event system, file system, input system, library loading
#define LIB_EXTENSION_UI        // UI extensions like window creation and event system

namespace Ultra {

// Information
constexpr auto LibCaption       = "Ultra Library";
constexpr auto LibDescription   = "Game Engine Framework";
constexpr auto LibRelease       = "2025-07-02";
constexpr auto LibVersion       = "0.0.0";
constexpr auto LibVersionMajor  = 0u;
constexpr auto LibVersionMinor  = 0u;
constexpr auto LibVersionPatch  = 0u;

// Features
namespace Features {

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
#ifdef LIB_EXTENSION_ENGINE
    constexpr bool LibEngineExtensions = true;
#else
    constexpr bool LibEngineExtensions = false;
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

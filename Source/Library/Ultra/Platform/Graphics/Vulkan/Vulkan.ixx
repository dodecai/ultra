module;

// Hack: The included header unit in the Core module doesn't work here.
#include "Ultra/Core/Core.h"

#if defined(APP_PLATFORM_WINDOWS)
    #define VK_USE_PLATFORM_WIN32_KHR
#endif

export module Ultra.Platform.Graphics.Vulkan;

import Ultra.Core;

// ToDo: Switch to C++ Moudle, after it is clear how...
//export import vulkan;
export import <vulkan/vulkan.hpp>;

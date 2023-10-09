module;

// Hack: The included header unit in the Core module doesn't work here.
#include "Ultra/Core/Core.h"

export module Ultra.Platform.Graphics.Vulkan;

import Ultra.Core;

#if defined(APP_PLATFORM_WINDOWS)
    #define VK_USE_PLATFORM_WIN32_KHR
#endif

export import <vulkan/vulkan.hpp>;

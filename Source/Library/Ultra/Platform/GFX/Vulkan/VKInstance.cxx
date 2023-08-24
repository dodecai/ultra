module;

// Hack: The included header unit in the Core module doesn't work here.
#include "Ultra/Core/Core.h"

#if defined(APP_PLATFORM_WINDOWS)
    // Hack: Workaround for problems with 'Windows.h' in combination with C++ modules (VS2022 > v17.5)!
    #undef __nullnullterminated
    #define __SPECSTRINGS_STRICT_LEVEL 0

    #define VK_USE_PLATFORM_WIN32_KHR

    #undef APIENTRY
    #define NOMINMAX
    #define VC_EXTRALEAN
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#endif

#include <vulkan/vulkan.hpp>

// Hack: Workaround for problems with 'Windows.h' in combination with C++ modules (VS2022 > v17.5)!
#undef __nullnullterminated
#define __SPECSTRINGS_STRICT_LEVEL 0

///
/// @brief Hack: This is an nasty fix for Microsoft's STL implementation
/// The logger fails in multiple modules due to chrono template resolutions
///
/// @note
/// type_traits(1344,53): error C2794: 'type': is not a member of any direct or indirect base class of 'std::common_type<_Rep1,_Rep2>'
///     with
///     [
///         _Rep1 = __int64,
///         _Rep2 = __int64
///     ]
/// __msvc_chrono.hpp(268,35): error C2938: 'std::common_type_t' : Failed to specialize alias template
/// __msvc_chrono.hpp(268,56): error C2752: 'std::common_type<_Rep1,_Rep2>': more than one partial specialization matches the template argument list
///     with
///     [
///         _Rep1 = __int64,
///         _Rep2 = __int64
///     ]
/// __msvc_chrono.hpp(113,54): error C2955: 'std::chrono::duration': use of class template requires template argument list
/// __msvc_chrono.hpp(118,54): error C2955: 'std::chrono::duration': use of class template requires template argument list
///
#include <chrono>

module Ultra.Platform.GFX.VKInstance;

import Ultra.Logger;

PFN_vkCreateDebugUtilsMessengerEXT CreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugUtilsMessengerEXT;

// Define a callback to capture the messages
VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessagerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, [[maybe_unused]] void *userData) {
    std::string type;

    using Ultra::logger;
    using Ultra::Log;
    using Ultra::LogLevel;

    switch ((vk::DebugUtilsMessageSeverityFlagBitsEXT)messageSeverity) {
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eError:      { logger << LogLevel::Error;    break; }
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo:       { logger << LogLevel::Info;     break; }
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose:    { logger << LogLevel::Trace;    break; }
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning:    { logger << LogLevel::Warn;     break; }
    }

    logger << "GFX::Vulkan[";

    switch ((vk::DebugUtilsMessageTypeFlagBitsEXT)messageType) {
        case vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral:        { logger << "General"; break; }
        case vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance:    { logger << "Performance"; break; }
        case vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation:     { logger << "Validation"; break; }
    }

    logger << "]: " << LogLevel::Default << callbackData->pMessage << "\n";
    return false;
}

namespace Ultra {

// Default
VKInstance::VKInstance() {
    // Application Information
    vk::ApplicationInfo applicationInfo = {
        "Application",  VK_MAKE_VERSION(1, 0, 0),
        "Engine",       VK_MAKE_VERSION(1, 0, 0),
        /*API*/         VK_API_VERSION_1_2,
    };

    // Extensions
    vector<vk::ExtensionProperties> availableExtensions = vk::enumerateInstanceExtensionProperties();
    vector<const char *> neededExtensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        #if defined APP_PLATFORM_WINDOWS
            VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
        #endif
        #ifdef APP_MODE_DEBUG
            VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        #endif
    };
    vector<const char *> extensions = GetExtensions(availableExtensions, neededExtensions);

    // Layers
    vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();
    vector<const char *> neededLayers = {
        #ifdef APP_MODE_DEBUG
            "VK_LAYER_KHRONOS_validation",
        #endif
    };
    vector<const char *> layers = GetLayers(availableLayers, neededLayers);

    // Instance
    vk::InstanceCreateInfo intanceCreateInfo = {
        vk::InstanceCreateFlags(),
        &applicationInfo,
        layers,
        extensions
    };
    try {
        mInstance = vk::createInstance(intanceCreateInfo);
    } catch (const std::exception& e) {
        logger << LogLevel::Fatal << ("[GFX::Instance] ", e.what());
    }

    // Debug
    #ifdef APP_MODE_DEBUG
    CreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(mInstance, "vkCreateDebugUtilsMessengerEXT");
    vk::DebugUtilsMessengerCreateInfoEXT messengerCreateInfo = {
        vk::DebugUtilsMessengerCreateFlagsEXT(),
        ( // Severities
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo     | 
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose  |
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning  |
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError 
        ),
        ( // Types
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral      |
            vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance  |
            vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
        ),
        DebugMessagerCallback,
        nullptr,
    };
    VkResult result = CreateDebugUtilsMessengerEXT(mInstance, reinterpret_cast<VkDebugUtilsMessengerCreateInfoEXT *>(&messengerCreateInfo), nullptr, &mDebugUtilsMessanger);
    if (result) { /* ToDo: Check result */ }
    #endif
}

VKInstance::~VKInstance() {
    #ifdef APP_MODE_DEBUG
    if (mDebugUtilsMessanger) {
        DestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(mInstance, "vkDestroyDebugUtilsMessengerEXT");
        DestroyDebugUtilsMessengerEXT(mInstance, mDebugUtilsMessanger, nullptr);
    }
    #endif
    if (mInstance) {
        mInstance.destroy(nullptr);
    }
}


// Accessors
const vk::Instance &VKInstance::Call() const {
    return mInstance;
}


// Conversions
VKInstance::operator const vk::Instance&() const  {
    return mInstance;
}


// Internal
vector<const char *> VKInstance::GetExtensions(const vector<vk::ExtensionProperties> &available, const vector<const char *> &needed) {
    vector<const char *> result = {};
    for (auto const &n : needed) {
        for (auto const &a : available) {
            if (string(a.extensionName.data()).compare(n) == 0) {
                result.emplace_back(n);
                break;
            }
        }
    }
    return result;
}

vector<const char *> VKInstance::GetLayers(const vector<vk::LayerProperties> &available, const vector<const char *> &needed) {
    vector<const char *> result = {};
    for (auto const &n : needed) {
        for (auto const &a : available) {
            if (string(a.layerName.data()).compare(n) == 0) {
                result.emplace_back(n);
                break;
            }
        }
    }
    return result;
}

}

#pragma once

#include "Omnia/Core.h"

#include "Vulkan.h"

namespace Omnia {

class VKInstance {
public:
    // Default
    VKInstance();
    ~VKInstance();

    // Accessors
    const vk::Instance &Call() const;

    // Conversions
    operator const vk::Instance &() const;

private:
    // Internal
    static vector<const char *> GetLayers(const vector<vk::LayerProperties> &available, const vector<const char*> &needed);
    static vector<const char *> GetExtensions(const vector<vk::ExtensionProperties> &available, const vector<const char*> &needed);

private:
    // Properties
    vk::Instance mInstance = nullptr;
    VkDebugUtilsMessengerEXT mDebugUtilsMessanger;
};

}

export module Ultra.Platform.GFX.VKInstance;

import Ultra.Core;
import Ultra.Platform.GFX.Vulkan;

export namespace Ultra {

class VKInstance {
public:
    // Default
    VKInstance();
    ~VKInstance();

    // Accessors
    const vk::Instance &Call() const;

    // Conversions
    operator const vk::Instance&() const;

private:
    // Methods
    static vector<const char *> GetLayers(const vector<vk::LayerProperties> &available, const vector<const char *> &needed);
    static vector<const char *> GetExtensions(const vector<vk::ExtensionProperties> &available, const vector<const char *> &needed);

    // Properties
    vk::Instance mInstance = nullptr;
    VkDebugUtilsMessengerEXT mDebugUtilsMessanger;
};

}


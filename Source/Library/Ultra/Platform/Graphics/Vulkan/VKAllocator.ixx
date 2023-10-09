export module Ultra.Platform.Graphics.VKAllocator;

import Ultra.Core;
import Ultra.Platform.Graphics.Vulkan;
import Ultra.Platform.Graphics.VKDevice;

export namespace Ultra {

class VKAllocator {
public:
    // Default
    VKAllocator() = default;
    VKAllocator(const Reference<VKDevice> &device, const string &tag = "");

    // Commands
    void Allocate(vk::MemoryRequirements requirements, vk::DeviceMemory *destination, vk::MemoryPropertyFlags flags = vk::MemoryPropertyFlagBits::eDeviceLocal);

private:
    // Properties
    Reference<VKDevice> Device;
    string Tag;
};

}

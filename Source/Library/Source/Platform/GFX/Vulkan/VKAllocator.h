#pragma once

#include "Vulkan.h"
#include "VKDevice.h"

namespace Omnia {

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


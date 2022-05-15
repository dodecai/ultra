#include "VKAllocator.h"

namespace Omnia {

// Default
VKAllocator::VKAllocator(const Reference<VKDevice> &device, const string &tag): Device(device), Tag(tag) {}

// Commands
void VKAllocator::Allocate(vk::MemoryRequirements requirements, vk::DeviceMemory *destination, vk::MemoryPropertyFlags flags) {

    vk::MemoryAllocateInfo memoryAllocateInfo = {};
    memoryAllocateInfo.allocationSize = requirements.size;

    // Get the memory type index that supports host visible memory access
    // Most implementations offer multiple memory types and selecting the correct one to allocate memory from is crucial
    // We also want the buffer to be host coherent so we don't have to flush (or sync after every update.
    // Note: This may affect performance so you might not want to do this in a real world application that updates buffers on a regular base
    memoryAllocateInfo.memoryTypeIndex = Device->GetPhysicalDevice()->GetMemoryTypeIndex(requirements.memoryTypeBits, flags);

    //// Allocate memory for the uniform buffer
    *destination = Device->Call().allocateMemory(memoryAllocateInfo, nullptr);
}

}

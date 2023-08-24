module;

#include <vulkan/vulkan.hpp>

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

module Ultra.Platform.GFX.VKDevice;

import Ultra.Logger;
// ToDo: Feature to support multiple physical devices

namespace Ultra {

namespace {

// ToDo: Not needed anymore but maybe usefull
//bool CheckDeviceExtensionSupport(const vk::PhysicalDevice& device) {
//    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
//
//    for (const auto& extension : device.enumerateDeviceExtensionProperties()) {
//        requiredExtensions.erase(extension.extensionName);
//    }
//
//    return requiredExtensions.empty();
//}

uint32_t GetQueueIndex(vk::PhysicalDevice &physicalDevice, vk::QueueFlagBits flags) {
    vector<vk::QueueFamilyProperties> queueProps = physicalDevice.getQueueFamilyProperties();
    for (size_t i = 0; i < queueProps.size(); ++i) {
        if (queueProps[i].queueFlags & flags) {
            return static_cast<uint32_t>(i);
        }
    }
    return 0;
}

vector<const char *> GetExtensions(const vector<vk::ExtensionProperties> &available, const vector<const char *> &needed) {
    vector<const char *> result = {};
    for (const char *const &n : needed) {
        for (vk::ExtensionProperties const &a : available) {
            if (string(a.extensionName.data()).compare(n) == 0) {
                result.emplace_back(n);
                break;
            }
        }
    }
    return result;
}

}

/*
 * Physical Device
*/
VKPhysicalDevice::VKPhysicalDevice(const Reference<VKInstance> &instance): mInstance(instance) {
    // Get available physical devices and choose the "best" one
    mPhysicalDevices = mInstance->Call().enumeratePhysicalDevices();
    mPhysicalDevice = ChoosePhysicalDevice(mPhysicalDevices);
    //AppAssert(mPhysicalDevice, "GFX::Vulkan::PhysicalDevice: ", "No suiteable vulkan supporting deivce found!");

    // Get features and properties
    mFeatures = mPhysicalDevice.getFeatures();
    mMemoryProperties = mPhysicalDevice.getMemoryProperties();
    mProperties = mPhysicalDevice.getProperties();
    mQueueFamilyProperties = mPhysicalDevice.getQueueFamilyProperties();
    for (auto &extension : mPhysicalDevice.enumerateDeviceExtensionProperties()) {
        mSupportedExtensions.emplace(extension.extensionName.data());
    }

    // Get Queues
    vk::QueueFlags requestedQueueTypes = vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eTransfer;
    mQueueFamilyIndices = GetQueueFamilyIndices(requestedQueueTypes);
    if (requestedQueueTypes & vk::QueueFlagBits::eCompute) {
        vk::DeviceQueueCreateInfo info {};
        info.queueFamilyIndex = mQueueFamilyIndices.Compute;
        info.queueCount = 1;
        info.pQueuePriorities = &mDefaultPriority;
        mQueueCreateInformation.push_back(info);
    }
    if (requestedQueueTypes & vk::QueueFlagBits::eGraphics) {
        vk::DeviceQueueCreateInfo info {};
        info.queueFamilyIndex = mQueueFamilyIndices.Graphics;
        info.queueCount = 1;
        info.pQueuePriorities = &mDefaultPriority;
        mQueueCreateInformation.push_back(info);
    }
    if (requestedQueueTypes & vk::QueueFlagBits::eProtected) {
        vk::DeviceQueueCreateInfo info {};
        info.queueFamilyIndex = mQueueFamilyIndices.Protected;
        info.queueCount = 1;
        info.pQueuePriorities = &mDefaultPriority;
        mQueueCreateInformation.push_back(info);
    }
    if (requestedQueueTypes & vk::QueueFlagBits::eSparseBinding) {
        vk::DeviceQueueCreateInfo info {};
        info.queueFamilyIndex = mQueueFamilyIndices.SparseBinding;
        info.queueCount = 1;
        info.pQueuePriorities = &mDefaultPriority;
        mQueueCreateInformation.push_back(info);
    }
    if (requestedQueueTypes & vk::QueueFlagBits::eTransfer) {
        vk::DeviceQueueCreateInfo info {};
        info.queueFamilyIndex = mQueueFamilyIndices.Transfer;
        info.queueCount = 1;
        info.pQueuePriorities = &mDefaultPriority;
        mQueueCreateInformation.push_back(info);
    }
    LogTrace("GFX::Vulkan::PhysicalDevice: {}", (string)*this); // ToDo: Better Output...
}
// Accessors
const vk::PhysicalDevice &VKPhysicalDevice::Call() const {
    return mPhysicalDevice;
}

const vk::PhysicalDeviceFeatures &VKPhysicalDevice::GetFeatures() const {
    return mFeatures;
}

const vk::PhysicalDeviceProperties &VKPhysicalDevice::GetProperties() const {
    return mProperties;
}

const vk::PhysicalDeviceMemoryProperties &VKPhysicalDevice::GetMemoryProperties() const {
    return mMemoryProperties;
}

// Conversions
VKPhysicalDevice::operator const vk::PhysicalDevice &() const {
    return mPhysicalDevice;
}

VKPhysicalDevice::operator const string() const {
    stringstream stream;

    // Caption
    stream << "Selected physical device '" << mProperties.deviceName << "' [";

    // Vendor ID
    stream << "VendorID='";
    switch (mProperties.vendorID) {
        case 0x1002: { stream << "AMD";      break; }
        case 0x8086: { stream << "Intel";    break; }
        case 0x10DE: { stream << "nVidia";   break; }
        default: { stream << "#" << mProperties.vendorID; break; }
    }
    stream << "' | ";

    // Device Type
    stream << "Type='";
    switch (mProperties.deviceType) {
        case vk::PhysicalDeviceType::eCpu: { stream << "CPU";    break; }
        case vk::PhysicalDeviceType::eDiscreteGpu: { stream << "GPU";    break; }
        case vk::PhysicalDeviceType::eIntegratedGpu: { stream << "I-GPU";  break; }
        case vk::PhysicalDeviceType::eVirtualGpu: { stream << "V-GPU";  break; }
        default: { stream << "Other";  break; }
    }
    stream << "'";

    stream << "]";
    return stream.str();
}

// Queries
bool VKPhysicalDevice::IsExtensionSupport(const string &name) const {
    return mSupportedExtensions.find(name) != mSupportedExtensions.end();
}

// Protected
const vector<vk::DeviceQueueCreateInfo> &VKPhysicalDevice::GetQueueCreateInfo() const {
    return mQueueCreateInformation;
}

const int32_t VKPhysicalDevice::GetQueueFamilyIndex(vk::QueueFlags flag) const {
    if (flag & vk::QueueFlagBits::eGraphics) return mQueueFamilyIndices.Graphics;
    else if (flag & vk::QueueFlagBits::eCompute) return mQueueFamilyIndices.Compute;
    else if (flag & vk::QueueFlagBits::eTransfer) return mQueueFamilyIndices.Transfer;
    else if (flag & vk::QueueFlagBits::eProtected) return mQueueFamilyIndices.Protected;
    else if (flag & vk::QueueFlagBits::eSparseBinding) return mQueueFamilyIndices.SparseBinding;
    return -1;
}

// Internal
vk::PhysicalDevice VKPhysicalDevice::ChoosePhysicalDevice(const vector<vk::PhysicalDevice> &devices) {
    std::multimap<uint32_t, vk::PhysicalDevice> candidates;
    for (const auto &device : devices) {
        candidates.emplace(RankPhysicalDevice(device), device);
    }

    if (candidates.rbegin()->first > 0) return candidates.rbegin()->second;
    return nullptr;
}

size_t VKPhysicalDevice::RankPhysicalDevice(const vk::PhysicalDevice &device) {
    // ToDo:: Choose device which supports all extensions that are needed, rather than basically counting how many are supported
    size_t score = 0;
    vector<vk::ExtensionProperties> extensionProperties = device.enumerateDeviceExtensionProperties();

    auto features = device.getFeatures();
    auto properties = device.getProperties();
    if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) score += 1024;
    score += properties.limits.maxImageDimension2D;
    
    if (features.geometryShader) score += 4;
    //score += extensionProperties.size();

    return score;
}

VKQueueFamilyIndices VKPhysicalDevice::GetQueueFamilyIndices(vk::QueueFlags flags) {
    VKQueueFamilyIndices indices = {};

    if (flags & vk::QueueFlagBits::eTransfer) {
        for (uint32_t i = 0; i < mQueueFamilyProperties.size(); i++) {
            auto &properties = mQueueFamilyProperties[i];
            if ((properties.queueFlags & vk::QueueFlagBits::eTransfer) && !(properties.queueFlags & vk::QueueFlagBits::eGraphics) && !(properties.queueFlags & vk::QueueFlagBits::eCompute)) {
                indices.Transfer = i;
                break;
            }
        }
    }

    if (flags & vk::QueueFlagBits::eCompute) {
        for (uint32_t i = 0; i < mQueueFamilyProperties.size(); i++) {
            auto &properties = mQueueFamilyProperties[i];
            if ((properties.queueFlags & vk::QueueFlagBits::eCompute) && !(properties.queueFlags & vk::QueueFlagBits::eGraphics)) {
                indices.Compute = i;
                break;
            }
        }
    }

    for (uint32_t i = 0; i < mQueueFamilyProperties.size(); i++) {
        auto &properties = mQueueFamilyProperties[i];
        if (flags & vk::QueueFlagBits::eTransfer && indices.Transfer == -1) {
            if (properties.queueFlags & vk::QueueFlagBits::eTransfer) indices.Transfer = i;
        }
        if (flags & vk::QueueFlagBits::eCompute && indices.Compute == -1) {
            if (properties.queueFlags & vk::QueueFlagBits::eCompute) indices.Compute = i;
        }
        if (flags & vk::QueueFlagBits::eGraphics) {
            if (properties.queueFlags & vk::QueueFlagBits::eGraphics) indices.Graphics = i;
        }
    }

    return indices;
}

uint32_t VKPhysicalDevice::GetMemoryTypeIndex(uint32_t bits, vk::MemoryPropertyFlags properties) {
    for (uint32_t i = 0; i < mMemoryProperties.memoryTypeCount; i++) {
        if ((bits & 1) == 1) {
            if ((mMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
        bits >>= 1;
    }
    return std::numeric_limits<uint32_t>::max();
}



/*
* Logical Device
*/

VKDevice::VKDevice(const Reference<VKPhysicalDevice> &physicalDevice): mPhysicalDevice { physicalDevice } {
    vector<vk::LayerProperties> layers = mPhysicalDevice->Call().enumerateDeviceLayerProperties();
    vector<vk::ExtensionProperties> availableDeviceExtensions = mPhysicalDevice->Call().enumerateDeviceExtensionProperties();
    vector<vk::PhysicalDeviceFeatures> features;

    vector<const char *> neededDeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    std::vector<const char *> deviceExtensions = GetExtensions(availableDeviceExtensions, neededDeviceExtensions);

    vk::DeviceCreateInfo deviceCreateInfo = {
        vk::DeviceCreateFlagBits(),
        mPhysicalDevice->GetQueueCreateInfo(),
        nullptr,
        deviceExtensions,
        nullptr
    };
    mDevice = mPhysicalDevice->Call().createDevice(deviceCreateInfo);

    vk::CommandPoolCreateInfo commandPoolCreateInfo = {};
    commandPoolCreateInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer | vk::CommandPoolCreateFlagBits::eTransient;
    commandPoolCreateInfo.queueFamilyIndex = mPhysicalDevice->mQueueFamilyIndices.Graphics;
    mCommandPool = mDevice.createCommandPool(commandPoolCreateInfo);

    mGraphicsQueue = mDevice.getQueue(mPhysicalDevice->mQueueFamilyIndices.Graphics, 0);
    mPresentQueue = mDevice.getQueue(mPhysicalDevice->mQueueFamilyIndices.Graphics, 0); // ToDo:: Should be present somehow
}

void VKDevice::Destroy() {
    mDevice.freeCommandBuffers(mCommandPool, mCommandBuffer);
    mDevice.destroyCommandPool(mCommandPool, nullptr);
    mDevice.destroy(nullptr);
}

// Accessors
const vk::Device &VKDevice::Call() const {
    return mDevice;
}

const vk::Queue &VKDevice::GetQueue(VKQueueTypes type) const {
    switch (type) {
        case VKQueueTypes::Graphics: { return mGraphicsQueue; }
        case VKQueueTypes::Present: { return mGraphicsQueue; }
        default: { break; }
    }
    return mGraphicsQueue;
}

const vk::CommandPool &VKDevice::GetCommandPool() const {
    return mCommandPool;
}

const Reference<VKPhysicalDevice> &VKDevice::GetPhysicalDevice() const {
    return mPhysicalDevice;
}

// Conversions
VKDevice::operator const vk::Device &() const {
    return mDevice;
}

// Commands
vk::CommandBuffer VKDevice::GetCommandBuffer(bool start, vk::CommandBufferLevel level) const {
    vk::CommandBuffer buffer = {};
    vk::CommandBufferAllocateInfo bufferAllcoateInfo = {};
    bufferAllcoateInfo.commandPool = mCommandPool;
    bufferAllcoateInfo.level = level;
    bufferAllcoateInfo.commandBufferCount = 1;
    buffer = (mDevice.allocateCommandBuffers(bufferAllcoateInfo))[0];

    if (start) {
        vk::CommandBufferBeginInfo bufferBeginInfo = {};
        bufferBeginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
        buffer.begin(bufferBeginInfo);
    }

    return buffer;
}

void VKDevice::FlushCommandBuffer(vk::CommandBuffer &buffer) {
    const uint64_t DefaultFenceTimeout = UINT64_MAX;
    buffer.end();

    vk::PipelineStageFlags waitDstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    vk::SubmitInfo submitInfo = {};
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &buffer;
    submitInfo.pWaitDstStageMask = &waitDstStageMask;

    vk::Fence fence = mDevice.createFence(vk::FenceCreateInfo());
    mDevice.resetFences(fence);
    mGraphicsQueue.submit(submitInfo, fence);
    [[maybe_unused]] auto result = mDevice.waitForFences(1, &fence, VK_TRUE, DefaultFenceTimeout);

    mDevice.destroyFence(fence, nullptr);
    mDevice.freeCommandBuffers(mCommandPool, buffer);
}

void VKDevice::FlushCommandBuffer(vk::CommandBuffer &buffer, vk::SubmitInfo &submit) {
    const uint64_t DefaultFenceTimeout = UINT64_MAX;
    buffer.end();

    vk::PipelineStageFlags waitDstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;

    vk::FenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.flags = vk::FenceCreateFlagBits::eSignaled;
    vk::Fence fence = nullptr;
    fence = mDevice.createFence(fenceCreateInfo, nullptr);

    vk::Semaphore renderComplete = mDevice.createSemaphore(vk::SemaphoreCreateInfo());
    vk::Semaphore presentationComplete = mDevice.createSemaphore(vk::SemaphoreCreateInfo());

    vk::SubmitInfo submitInfo = submit;
    submitInfo.commandBufferCount = 1;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pCommandBuffers = &buffer;
    submitInfo.pSignalSemaphores = &renderComplete;
    submitInfo.pWaitSemaphores = &presentationComplete;
    submitInfo.pWaitDstStageMask = &waitDstStageMask;

    auto result = mDevice.resetFences(1, &fence);
    mGraphicsQueue.submit(submit, fence);
    result = mDevice.waitForFences(1, &fence, VK_TRUE, DefaultFenceTimeout);
    mDevice.destroyFence(fence, nullptr); // Reset instead of destroy

    mDevice.freeCommandBuffers(mCommandPool, buffer);

    mDevice.destroySemaphore(renderComplete, nullptr);
    mDevice.destroySemaphore(presentationComplete, nullptr);
}

}

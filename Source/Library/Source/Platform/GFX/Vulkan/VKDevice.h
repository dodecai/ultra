#pragma once

#include "Omnia/Omnia.pch"
#include "Omnia/Core.h"

#include "Vulkan.h"
#include "VKInstance.h"

namespace Omnia {

struct VKQueueFamilyIndices {
    int32_t Compute = -1;
    int32_t Graphics = -1;
    int32_t Protected = -1;
    int32_t SparseBinding = -1;
    int32_t Transfer = -1;
};

enum class VKQueueTypes {
    Graphics,
    Present
};

class VKPhysicalDevice {
    friend class VKDevice;

public:
    // Default
    explicit VKPhysicalDevice(const Reference<VKInstance> &instance);
    ~VKPhysicalDevice() = default;

    // Accessors
    const vk::PhysicalDevice &Call() const;
    const vk::PhysicalDeviceFeatures &GetFeatures() const;
    const vk::PhysicalDeviceProperties &GetProperties() const;
    const vk::PhysicalDeviceMemoryProperties &GetMemoryProperties() const;

    // Conversions
    operator const vk::PhysicalDevice &() const;
    operator const string() const;

    // Queries
    bool IsExtensionSupport(const string &name) const;

    vk::Format GetDepthFormat() const;
    uint32_t GetMemoryTypeIndex(uint32_t bits, vk::MemoryPropertyFlags properties);
    const int32_t GetQueueFamilyIndex(vk::QueueFlags flag) const;

protected:
    const vector<vk::DeviceQueueCreateInfo> &GetQueueCreateInfo() const;

private:
    // Internal
    vk::Format ChooseDepthFormat() const;
    vk::PhysicalDevice ChoosePhysicalDevice(const vector<vk::PhysicalDevice> &devices);
    VKQueueFamilyIndices GetQueueFamilyIndices(vk::QueueFlags flags);
    uint32_t RankPhysicalDevice(const vk::PhysicalDevice &device);

private:
    Reference<VKInstance> mInstance = nullptr;
    static inline const float mDefaultPriority = 0.0f;

    vector<vk::PhysicalDevice> mPhysicalDevices = {};
    std::unordered_set<string> mSupportedExtensions = {};

    vk::PhysicalDevice mPhysicalDevice = nullptr;
    vk::PhysicalDeviceFeatures mFeatures = {};
    vk::PhysicalDeviceProperties mProperties = {};
    vk::PhysicalDeviceMemoryProperties mMemoryProperties = {};

    VKQueueFamilyIndices mQueueFamilyIndices;
    vector<vk::DeviceQueueCreateInfo> mQueueCreateInformation;
    vector<vk::QueueFamilyProperties> mQueueFamilyProperties;
};

class VKDevice {
public:
    // Default
    VKDevice(const Reference<VKPhysicalDevice> &physicalDevice);
    ~VKDevice() = default;
    void Destroy();

    // Accessors
    const vk::Device &Call() const;
    const vk::Queue &GetQueue(VKQueueTypes type = VKQueueTypes::Graphics) const;
    const vk::CommandPool &GetCommandPool() const;
    const Reference<VKPhysicalDevice> &GetPhysicalDevice() const;

    // Conversions
    operator const vk::Device &() const;

    // Commands
    const vk::CommandBuffer &GetCommandBuffer(bool start = false, vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary) const;
    void FlushCommandBuffer(vk::CommandBuffer &buffer);
    void FlushCommandBuffer(vk::CommandBuffer &buffer, vk::SubmitInfo &submit);

private:
    Reference<VKPhysicalDevice> mPhysicalDevice = nullptr;

    vk::Device mDevice = nullptr;
    vk::Queue mGraphicsQueue = {};
    vk::Queue mPresentQueue = {};
    vk::CommandPool mCommandPool = {};
    vector<vk::CommandBuffer> mCommandBuffer = {};
};

}

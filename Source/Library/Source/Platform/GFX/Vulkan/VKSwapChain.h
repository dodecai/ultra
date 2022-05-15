#pragma once

#include "Omnia/Omnia.pch"
#include "Omnia/Core.h"

#include "Vulkan.h"

#include "VKAllocator.h"
#include "VKDevice.h"

namespace Omnia {

struct VKSwapChainBuffer {
    vk::Framebuffer FrameBuffer;
    vk::Image Image;
    vk::ImageView View;
};

struct VKDepthStencilBuffer {
    vk::Image Image;
    vk::DeviceMemory Memory;
    vk::ImageView View;
};

struct VKSurfaceProperties {
    vk::ClearValue ClearValues[2];
    bool SynchronizedDraw = false;

    vk::Extent2D Size;

    vk::Viewport Viewport;
    vk::Rect2D RenderArea;

    vk::Format ColorFormat;
    vk::ColorSpaceKHR ColorSpace;
    vk::Format DepthFormat;
};

struct VKSynchronization {
    const uint64_t DefaultTimeout = 5000000;
    size_t Timeout = 5000;

    vector<vk::Semaphore> PresentComplete;
    vector<vk::Semaphore> RenderComplete;

    vector<vk::Fence> WaitFences;
};


class VKSwapChain {
public:
    // Default
    VKSwapChain(const Reference<VKDevice> &device, const vk::SurfaceKHR &surface);
    ~VKSwapChain();
    void Create(uint32_t width, uint32_t height, bool synchronizedDraw = false);
    void Resize(uint32_t width, uint32_t height);
    void Destroy();

    void Prepare();
    void Submit(vk::CommandBuffer buffer);
    void Finish();
    void Present();

    // Accessors
    VKSurfaceProperties &GetSurfaceProperties() { return mSurfaceProperties; }
    const vk::CommandBuffer &GetCurrentDrawCommandBuffer() const;
    const vk::Framebuffer &GetCurrentFramebuffer() const;
    const uint32_t GetImageCount() const;
    const vk::Rect2D &GetRenderArea() const;
    const vk::RenderPass &GetRenderPass() const;

    // Mutators
    void SetSyncronizedDraw(bool enable);
private:
    // Helpers
    void CreateImageViews();
    void CreateRenderPass();
    void CreateFrameBuffer();
    void CreateCommandBuffers();

    // Internal
    void ChooseCapabilities(const vk::SurfaceCapabilitiesKHR &capabilities, uint32_t width, uint32_t height);
    void ChooseSurfaceFormat(const vector<vk::SurfaceFormatKHR> &surfaceFormats);
    void ChoosePresentModes(const vector<vk::PresentModeKHR> &presentModes, bool sync);
    vk::Result QueuePresent(uint32_t imageIndex, vk::Semaphore renderComplete = nullptr);

private:
    // Properties
    VKAllocator mAllocator;
    Reference<VKDevice> mDevice = nullptr;
    vk::SurfaceKHR mSurface = nullptr;
    VKSurfaceProperties mSurfaceProperties;

    uint32_t CurrentFrame = 0;
    uint32_t CurrentBufferIndex = 0;
    uint32_t mImageCount = 0;
    uint32_t mMaxImageCount = 3;
    uint32_t ComputeQueueIndex = 0;
    uint32_t GraphicsQueueIndex = 0;
    bool mRebuildRequested = false;

    // SwapChain
    vk::SwapchainKHR mSwapchain = nullptr;
    vk::RenderPass mRenderPass;
    vk::PresentModeKHR mPresentMode;
    vector<VKSwapChainBuffer> mSwapchainBuffers;
    VKDepthStencilBuffer mDepthStencilBuffer;
    VKSynchronization mSynchronization;

    // CommandPool\Buffers 
    vk::CommandPool mDrawCommandPool;
    vector<vk::CommandBuffer> mDrawCommandBuffers;
};

}

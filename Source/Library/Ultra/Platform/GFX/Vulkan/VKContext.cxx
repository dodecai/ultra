module;

// Hack: The included header unit in the Core module doesn't work here.
#include "Ultra/Core/Core.h"

#if defined(APP_PLATFORM_WINDOWS)
    #define VK_USE_PLATFORM_WIN32_KHR
#endif

#include <vulkan/vulkan.hpp>

module Ultra.Platform.GFX.VKContext;

import Ultra.Logger;
import Ultra.Engine.Utility.ShaderCompiler;
import Ultra.System.FileSystem;
import Ultra.UI.Window;

// Sources: https://alain.xyz/blog/raw-vulkan | https://gist.github.com/graphitemaster/e162a24e57379af840d4

namespace Ultra {

VKContext::VKContext(void *window) {
    mWindowHandle = reinterpret_cast<HWND>(window);
    mInstance = CreateReference<VKInstance>();
    mPhysicalDevice = CreateReference<VKPhysicalDevice>(mInstance);
    mDevice = CreateReference<VKDevice>(mPhysicalDevice);

    // Surface
    vk::Win32SurfaceCreateInfoKHR surfaceCreateinfo = {};
    surfaceCreateinfo.hinstance = GetModuleHandle(NULL);
    surfaceCreateinfo.hwnd = mWindowHandle;
    mSurface = mInstance->Call().createWin32SurfaceKHR(surfaceCreateinfo);

    // SwapChain
    mSwapChain = CreateReference<VKSwapChainLegacy>(mDevice, mSurface);
    mSwapChain->Create(1, 1); // ToDo: Resize triggered after start, so this should not be needed

    // PipelineCache
    mPipelineCache = mDevice->Call().createPipelineCache(vk::PipelineCacheCreateInfo());
}

VKContext::~VKContext() {
    mDevice->Call().waitIdle();
    mDevice->Call().destroyPipelineCache(mPipelineCache, nullptr);

    // The SwapChain should be destroyed before destroying the surface
    mSwapChain.reset();
    //vkDestroySurfaceKHR(mSurface);
    mInstance->Call().destroySurfaceKHR(mSurface, nullptr);
}

bool VKContext::CreateSurface(const void *window, vk::SurfaceKHR *surface) {
    vk::Win32SurfaceCreateInfoKHR createInfo = {};
    createInfo.hinstance = GetModuleHandle(NULL);
    createInfo.hwnd = (HWND)window;
    try {
        *surface = mInstance->Call().createWin32SurfaceKHR(createInfo);
    } catch (...) {
        return false;
    }
    return true;
}


void VKContext::Load() {
}

void VKContext::Attach() {
    mDevice->Call().waitIdle();
    mSwapChain->Prepare();
}

void VKContext::SwapBuffers() {
    mDevice->Call().waitIdle();
    mSwapChain->Finish();
}

void VKContext::Detach() {
    mDevice->Call().waitIdle();
    mSwapChain->Present();
}

void *VKContext::GetNativeContext() {
    static VkContextData *data = new VkContextData();
    return (void *)data;
}

bool const VKContext::IsCurrentContext() {
    return true;
}

void VKContext::SetViewport(uint32_t width, uint32_t height, [[maybe_unused]] int32_t x, [[maybe_unused]] int32_t y) {
    mSwapChain->Resize(width, height);
}

void VKContext::SetVSync(bool activate) {
    mDevice->Call().waitIdle();
    mSwapChain->SetSyncronizedDraw(activate);
}

}

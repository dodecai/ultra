#include "VKContext.h"

#include "Omnia/Log.h"
#include "Omnia/UI/Window.h"

// Sources: https://alain.xyz/blog/raw-vulkan | https://gist.github.com/graphitemaster/e162a24e57379af840d4

namespace Omnia {

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
    // Swapchain
    mSwapChain = CreateReference<VKSwapChain>(mDevice, mSurface);
    mSwapChain->Create(12, 12); // ToDo: Resize triggered after start, so this should not be needed

    // PipelineCache
    mPipelineCache = mDevice->Call().createPipelineCache(vk::PipelineCacheCreateInfo());
}

VKContext::~VKContext() {
    mDevice->Call().waitIdle();
    mDevice->Call().destroyPipelineCache(mPipelineCache, nullptr);

    // The SwapChain should be destroyed bevore destroying the surface
    mSwapChain.reset();
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
    return (void*)data;
}

bool const VKContext::IsCurrentContext() {
    return true;
}

void VKContext::SetViewport(uint32_t width, uint32_t height, int32_t x, int32_t y) {
    mSwapChain->Resize(width, height);
}

void VKContext::SetVSync(bool activate) {
    mDevice->Call().waitIdle();
    mSwapChain->SetSyncronizedDraw(activate);
}

}

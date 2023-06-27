module Ultra.Platform.Renderer.DXSwapchain;

#pragma warning(push)
#pragma warning(disable: 4100)

namespace Ultra {

DXSwapchain::DXSwapchain(void *windowHandle, uint32_t width, uint32_t height): Swapchain(windowHandle, width, height) {}

DXSwapchain::~DXSwapchain() {}


void DXSwapchain::Present() {}

void DXSwapchain::Resize(uint32_t width, uint32_t height) {}


Reference<Texture> DXSwapchain::GetCurrentTexture() {
    return nullptr;
}

uint32_t DXSwapchain::GetCurrentImageIndex() {
    return 0;
}

}

#pragma warning(pop)

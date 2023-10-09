module Ultra.Platform.Renderer.SWSwapchain;

#pragma warning(push)
#pragma warning(disable: 4100)

namespace Ultra {

SWSwapchain::SWSwapchain(void *windowHandle, uint32_t width, uint32_t height): Swapchain(windowHandle, width, height) {}

SWSwapchain::~SWSwapchain() {}


void SWSwapchain::Present() {}

void SWSwapchain::Resize(uint32_t width, uint32_t height) {}


Reference<Texture> SWSwapchain::GetCurrentTexture() {
    return nullptr;
}

uint32_t SWSwapchain::GetCurrentImageIndex() {
    return 0;
}

}

#pragma warning(pop)

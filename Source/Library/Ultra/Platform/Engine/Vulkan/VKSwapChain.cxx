module Ultra.Platform.Renderer.VKSwapchain;

namespace Ultra {

VKSwapchain::VKSwapchain(void *windowHandle, uint32_t width, uint32_t height): Swapchain(windowHandle, width, height) {}

VKSwapchain::~VKSwapchain() {}


void VKSwapchain::Present() {}

void VKSwapchain::Resize(uint32_t width, uint32_t height) {}


Reference<Texture> VKSwapchain::GetCurrentTexture() {
    return nullptr;
}

uint32_t VKSwapchain::GetCurrentImageIndex() {
    return 0;
}

}

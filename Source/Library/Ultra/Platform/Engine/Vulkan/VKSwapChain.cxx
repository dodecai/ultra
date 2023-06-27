module Ultra.Platform.Renderer.VKSwapchain;

#pragma warning(push)
#pragma warning(disable: 4100)

namespace Ultra {

VKSwapchain::VKSwapchain(void *windowHandle, uint32_t width, uint32_t height) {}

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

#pragma warning(pop)

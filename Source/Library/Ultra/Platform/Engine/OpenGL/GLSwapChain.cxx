module Ultra.Platform.Renderer.GLSwapchain;

namespace Ultra {

GLSwapchain::GLSwapchain(void *windowHandle, uint32_t width, uint32_t height): Swapchain(windowHandle, width, height) {}

GLSwapchain::~GLSwapchain() {}


void GLSwapchain::Present() {}

void GLSwapchain::Resize(uint32_t width, uint32_t height) {}


Reference<Texture> GLSwapchain::GetCurrentTexture() {
    return nullptr;
}

uint32_t GLSwapchain::GetCurrentImageIndex() {
    return 0;
}

}

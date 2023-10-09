module;

#include <glad/gl.h>

module Ultra.Platform.Renderer.GLSwapchain;

#pragma warning(push)
#pragma warning(disable: 4100)

namespace Ultra {

GLSwapchain::GLSwapchain(void *windowHandle, uint32_t width, uint32_t height): Swapchain(windowHandle, width, height) {}

GLSwapchain::~GLSwapchain() {
}


void GLSwapchain::Present() {
    //Context::SwapBuffers(mWindowHandle);
}

void GLSwapchain::Resize(uint32_t width, uint32_t height) {}


Reference<Texture> GLSwapchain::GetCurrentTexture() {
    // OpenGL does not have a swapchain texture concept like Vulkan/DirectX
    return nullptr;
}

uint32_t GLSwapchain::GetCurrentImageIndex() {
    // OpenGL does not have a swapchain image index concept like Vulkan/DirectX
    return 0;
}

}

#pragma warning(pop)

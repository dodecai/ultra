export module Ultra.Platform.Renderer.GLSwapchain;

import Ultra.Renderer.Swapchain;

export namespace Ultra {

class GLSwapchain: public Swapchain {
public:
    GLSwapchain(void *windowHandle, uint32_t width, uint32_t height);
    virtual ~GLSwapchain();

    virtual void Present() override;
    virtual void Resize(uint32_t width, uint32_t height) override;
    virtual Reference<Texture> GetCurrentTexture() override;
    virtual uint32_t GetCurrentImageIndex() override;
};

}

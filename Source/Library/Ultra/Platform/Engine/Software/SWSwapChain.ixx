export module Ultra.Platform.Renderer.SWSwapchain;

import Ultra.Renderer.Swapchain;

export namespace Ultra {

class SWSwapchain: public Swapchain {
public:
    SWSwapchain(void *windowHandle, uint32_t width, uint32_t height);
    virtual ~SWSwapchain();

    virtual void Present() override;
    virtual void Resize(uint32_t width, uint32_t height) override;
    virtual Reference<Texture> GetCurrentTexture() override;
    virtual uint32_t GetCurrentImageIndex() override;
};

}

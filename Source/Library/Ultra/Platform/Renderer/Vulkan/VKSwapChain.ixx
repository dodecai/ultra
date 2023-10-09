export module Ultra.Platform.Renderer.VKSwapchain;

import Ultra.Renderer.Swapchain;

export namespace Ultra {

class VKSwapchain: public Swapchain {
public:
    VKSwapchain(void *windowHandle, uint32_t width, uint32_t height);
    virtual ~VKSwapchain();

    virtual void Present() override;
    virtual void Resize(uint32_t width, uint32_t height) override;
    virtual Reference<Texture> GetCurrentTexture() override;
    virtual uint32_t GetCurrentImageIndex() override;
};

}

export module Ultra.Platform.Renderer.DXSwapchain;

import Ultra.Engine.Renderer.Swapchain;

export namespace Ultra {

class DXSwapchain: public Swapchain {
public:
    DXSwapchain(void *windowHandle, uint32_t width, uint32_t height);
    virtual ~DXSwapchain();

    virtual void Present() override;
    virtual void Resize(uint32_t width, uint32_t height) override;
    virtual Reference<Texture> GetCurrentTexture() override;
    virtual uint32_t GetCurrentImageIndex() override;
};

}

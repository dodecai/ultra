export module Ultra.Engine.Renderer.Swapchain;

export import Ultra.Core;
export import Ultra.Logger;

export import Ultra.Engine.Renderer.Texture;

export namespace Ultra {

/// 
/// @brief Agnostic Swapchain
/// 
/// @example: How-To
/// auto swapchain = Swapchain::Create(window, 1024, 768);
/// 
///
class Swapchain {
protected:
    Swapchain(void *windowHandle, uint32_t width, uint32_t height): mWindowHandle(windowHandle), mWidth(width), mHeight(height) {}

public:
    Swapchain() = default;
    virtual ~Swapchain() = default;

    static Scope<Swapchain> Create(void *windowHandle, uint32_t width, uint32_t height);

    virtual void Present() = 0;
    virtual void Resize(uint32_t width, uint32_t height) = 0;
    virtual Reference<Texture> GetCurrentTexture() = 0;
    virtual uint32_t GetCurrentImageIndex() = 0;

protected:
    void *mWindowHandle;
    int mWidth;
    int mHeight;
};

}

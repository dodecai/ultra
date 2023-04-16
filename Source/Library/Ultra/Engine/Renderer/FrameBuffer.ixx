export module Ultra.Renderer.Framebuffer;

export import Ultra.Core;
export import Ultra.Logger;

export import Ultra.Renderer.Texture;

export namespace Ultra {

/// 
/// @brief Agnostic Framebuffer
///
/// @example: How-To
/// auto framebuffer = Framebuffer::Create({ 1024, 768, TextureFormat::RGBA });
/// 
class Framebuffer {
protected:
    Framebuffer(uint32_t width, uint32_t height, TextureFormat format): mWidth(width), mHeight(height), mFormat(format) {}

public:
    virtual ~Framebuffer() = default;

    static Scope<Framebuffer> Create(uint32_t width, uint32_t height, TextureFormat format);

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual Reference<Texture> GetColorAttachment() const = 0;
    virtual Reference<Texture> GetDepthAttachment() const = 0;

protected:
    RendererID mFramebufferID;
    RendererID mColorTextureID;
    TextureFormat mFormat;
    uint32_t mHeight;
    uint32_t mWidth;
};

}

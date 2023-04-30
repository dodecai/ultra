module;

#include <glm/glm.hpp>

export module Ultra.Engine.Renderer.Framebuffer;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.Engine.Renderer.Texture;

export namespace Ultra {

struct FramebufferProperties {
    string Name = {};

    uint32_t Height = {};
    uint32_t Width = {};
    float Scale = 1.0f;

    glm::vec4 ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };

    vector<TextureFormat> Attachments;

    uint32_t Samples = 1;
};


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
    RendererID mDepthTextureID;
    TextureFormat mFormat;
    uint32_t mHeight;
    uint32_t mWidth;
};

}

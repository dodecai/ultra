export module Ultra.Renderer.Texture;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.Engine.RendererData;

export namespace Ultra {

enum class TextureFormat {
    RGB,
    RGBA,
    Depth,
    Stencil
};

/// 
/// @brief Agnostic Texture
///
/// @example: How-To
/// auto texture = Texture::Create({ data, 512, 512, TextureFormat::RGBA });
/// 
class Texture {
protected:
    Texture(const void *data, uint32_t width, uint32_t height, TextureFormat format): mWidth(width), mHeight(height), mFormat(format), mData(data) {}

public:
    Texture() = default;
    virtual ~Texture() = default;

    static Scope<Texture> Create(const void *data, uint32_t width, uint32_t height, TextureFormat format);

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

protected:
    RendererID mTextureID;
    const void *mData;
    TextureFormat mFormat;
    uint32_t mHeight;
    uint32_t mWidth;
};

}

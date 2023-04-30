export module Ultra.Renderer.Texture;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.Engine.RendererData;

export namespace Ultra {

///
/// @brief Texture Data
///
enum class TextureDimension {
    Texture2D,
    Texture3D,
};
 
enum class TextureFilter {
    Linear,
    Nearest,
    Cubic,
};

enum class TextureFormat {
    Blue,
    Green,
    Red,
    Alpha,

    BGR,
    BGRA,

    RGB,
    RGBA,

    RGBA16F,
    RGBA32F,

    Depth,
    Stencil,
};

enum class TextureType {
    Diffuse,
    Normal,
    Specular,
};

enum class TextureWrap {
    Clamp,
    Repeat,
};

struct TextureProperties {
    string Name;

    TextureDimension Dimension = TextureDimension::Texture2D;
    TextureFormat Format = TextureFormat::RGBA;
    uint32_t Width = 1;
    uint32_t Height = 1;

    TextureFilter SamplerFilter = TextureFilter::Linear;
    TextureWrap SamplerWrap = TextureWrap::Repeat;

    bool GenerateMips = true;
};



/// 
/// @brief Agnostic Texture
///
/// @example: How-To
/// auto texture = Texture::Create({ data, 512, 512, TextureFormat::RGBA });
/// 
class Texture {
protected:
    Texture(const TextureProperties &properties, const void *data, size_t size): mProperties(properties), mData(data), mSize(size) {}

public:
    Texture() = default;
    virtual ~Texture() = default;

    static Scope<Texture> Create(const TextureProperties &properties, const void *data, size_t size);
    static Scope<Texture> Create(const TextureProperties &properties, string &path);

    virtual void Bind(uint32_t slot = 0) const = 0;
    virtual void Unbind(uint32_t slot = 0) const = 0;

    // Accessors
    const TextureProperties &GetProperties() const { return mProperties; }

    // Operators
    bool operator==(const Texture &other) const { return mTextureID == other.mTextureID; }

protected:
    RendererID mTextureID;
    const void *mData;
    size_t mSize;
    TextureProperties mProperties;
};

using Texture2D = Texture;
using Texture3D = Texture;

}

module;

#include <glm/glm.hpp>

export module Ultra.Renderer.Texture;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.Renderer.Data;

export namespace Ultra {

///
/// @brief Texture Data
///
enum class TextureDimension {
    Texture2D,
    Texture3D,
    TextureCube = Texture3D,
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

    DEPTH32F,
    DEPTH24STENCIL8,

    Depth = DEPTH24STENCIL8,
    Stencil,
};

enum class TextureType {
    Diffuse,
    Normal,
    Specular,
};

enum class TextureUsage {
    Attachment,
    Texture,
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

    uint32_t Mips = 1;
    uint32_t Layers = 1;

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


namespace Helpers {

inline uint32_t GetTextureFormatBPP(TextureFormat format) {
    switch (format) {
        case TextureFormat::RGB:     return 3;
        case TextureFormat::RGBA:    return 4;
        case TextureFormat::RGBA16F: return 2 * 4;
        case TextureFormat::RGBA32F: return 4 * 4;
    }
    return 0;
}

inline uint32_t CalculateMipCount(uint32_t width, uint32_t height) {
    return (uint32_t)std::floor(std::log2(glm::min(width, height))) + 1;
}

inline uint32_t GetImageMemorySize(TextureFormat format, uint32_t width, uint32_t height) {
    return width * height * GetTextureFormatBPP(format);
}

inline bool IsDepthFormat(TextureFormat format) {
    if (format == TextureFormat::DEPTH24STENCIL8 || format == TextureFormat::DEPTH32F)
        return true;

    return false;
}

}


}

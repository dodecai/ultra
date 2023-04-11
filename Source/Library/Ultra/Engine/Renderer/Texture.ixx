module;

export module Ultra.Engine.Texture;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.GFX.Context;
export import Ultra.Engine.RendererData;
export import Ultra.Engine.RendererAPI;

export namespace Ultra {

enum class TextureFilter {
    Null    = 0,
    Linear  = 1,
    Nearest = 2,
    Cubic   = 3,
};

enum class TextureFormat {
	Null	= 0x00,

	Red		= 0x01,
	Green	= 0x02,
	Blue	= 0x03,
	Alpha	= 0x04,
    
	BGR		= 0x11,
	BGRA	= 0x12,

	RGB		= 0x20,
	RGBA	= 0x21,
    RGBA16F = 0x22,
    RGBA32F = 0x23,
};

enum class TextureWrap {
	Null	= 0,
	Clamp	= 1,
	Repeat	= 2,
};

struct TextureProperties {
    string Name;
    
    TextureFormat Format = TextureFormat::RGBA;
    uint32_t Width = 1;
    uint32_t Height = 1;

    TextureWrap SamplerWrap = TextureWrap::Repeat;
    TextureFilter SamplerFilter = TextureFilter::Linear;

    bool GenerateMips = true;
};

class Texture {
public:
	// Default
	Texture() = default;
	virtual ~Texture() = default;
	
	virtual void Bind(uint32_t slot = 0) const = 0;
	virtual void Unbind(uint32_t slot = 0) const = 0;

	// Accessors
    virtual RendererID GetRendererID() const = 0;
    virtual const TextureProperties &GetProperties() const = 0;

	// Operators
	virtual bool operator==(const Texture &other) const = 0;
};

class Texture2D: public Texture {
public:
	// Default
	Texture2D() = default;
	virtual ~Texture2D() = default;

    static Reference<Texture2D> Create(const TextureProperties &properties);
    static Reference<Texture2D> Create(const TextureProperties &properties, const void *data, size_t size);
	static Reference<Texture2D> Create(const TextureProperties &properties, const string &path);
};

class Texture3D: public Texture {
public:
	// Default
	Texture3D() = default;
	virtual ~Texture3D() = default;

    static Reference<Texture3D> Create(const TextureProperties &properties);
    static Reference<Texture3D> Create(const TextureProperties &properties, const void *data, size_t size);
	static Reference<Texture3D> Create(const TextureProperties &properties, const string &path);
};

}

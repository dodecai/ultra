export module Ultra.Platform.Engine.GLTexture;

import Ultra.Engine.Texture;

export namespace Ultra {

class GLTexture2D: public Texture2D {
public:
	// Default
	GLTexture2D() = default;
	GLTexture2D(const TextureProperties &properties);
    GLTexture2D(const TextureProperties &properties, const void *data, size_t size);
    GLTexture2D(const TextureProperties &properties, const string &path);
	virtual ~GLTexture2D();

	virtual void Bind(uint32_t slot) const override;
	virtual void Unbind(uint32_t slot) const override;

	// Accessors
    virtual uint32_t GetRendererID() const override { return mRendererID; };
	virtual const TextureProperties &GetProperties() const override { return mProperties; }

	// Operators
	virtual bool operator==(const Texture &other) const override { return mRendererID == ((GLTexture2D &)other).mRendererID; }

private:
    RendererID mRendererID;
    TextureProperties mProperties;
    uint32_t DataFormat;
    uint32_t RenderFormat;
};

class GLTexture3D: public Texture3D {
public:
	// Default
    GLTexture3D() = default;
    GLTexture3D(const TextureProperties &properties);
    GLTexture3D(const TextureProperties &properties, const void *data, size_t size);
    GLTexture3D(const TextureProperties &properties, const string &path);
	virtual ~GLTexture3D();

	virtual void Bind(uint32_t slot) const override;
	virtual void Unbind(uint32_t slot) const override;

	// Accessors
    virtual uint32_t GetRendererID() const override { return mRendererID; };
    virtual const TextureProperties &GetProperties() const { return mProperties; }

	// Operators
	virtual bool operator==(const Texture &other) const override { return mRendererID == ((GLTexture3D &)other).mRendererID; }

private:
    RendererID mRendererID;
    TextureProperties mProperties;
    uint32_t RenderFormat;
};

}

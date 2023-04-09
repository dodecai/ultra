export module Ultra.Platform.Engine.GLTexture;

import Ultra.Engine.Texture;

export namespace Ultra {

class GLTexture2D: public Texture2D {
public:
	// Default
	GLTexture2D(const string &path);
	GLTexture2D(const uint32_t width, const uint32_t height, TextureFormat format, TextureWrap wrap);
	virtual ~GLTexture2D();

	virtual void Bind(uint32_t slot) const override;
	virtual void UnBind(uint32_t slot) const override;

	// Accessors
	virtual TextureFormat GetFormat() const { return TextureFormat(); }
	virtual uint32_t GetHeight() const override { return Height; }
	virtual uint32_t GetWidth() const override { return Width; }
	virtual uint32_t GetRendererID() const override { return mRendererID; };

	// Mutators
	virtual void SetData(void *data, uint32_t size) const override;

	// Operators
	virtual bool operator==(const Texture &other) const override { return mRendererID == ((GLTexture2D &)other).mRendererID; }
    virtual operator RendererID() const override { return mRendererID; }

private:
    RendererID mRendererID;
    uint32_t Height;
    uint32_t Width;
    string Patch;
    uint32_t DataFormat;
    uint32_t RenderFormat;
};

class GLTexture3D: public Texture3D {
public:
	// Default
	GLTexture3D(const string &path);
	GLTexture3D(const uint32_t width, const uint32_t height, TextureFormat format, TextureWrap wrap);
	virtual ~GLTexture3D();

	virtual void Bind(uint32_t slot) const override;
	virtual void UnBind(uint32_t slot) const override;

	// Accessors
	virtual TextureFormat GetFormat() const { return TextureFormat(); }
	virtual uint32_t GetHeight() const override { return Height; }
	virtual uint32_t GetWidth() const override { return Width; }
	virtual uint32_t GetRendererID() const override { return mRendererID; };

	// Mutators
	virtual void SetData(void *data, uint32_t size) const override;

	// Operators
	virtual bool operator==(const Texture &other) const override { return mRendererID == ((GLTexture3D &)other).mRendererID; }
	virtual operator RendererID() const override { return mRendererID; }

private:
    RendererID mRendererID;
    uint32_t Height;
    uint32_t Width;
    string Patch;
    uint32_t DataFormat;
    uint32_t RenderFormat;
};

}

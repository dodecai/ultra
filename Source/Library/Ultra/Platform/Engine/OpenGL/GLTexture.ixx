export module Ultra.Platform.Renderer.GLTexture;

import Ultra.Renderer.Texture;

export namespace Ultra {

class GLTexture: public Texture {
public:
    GLTexture(const void *data, uint32_t width, uint32_t height, TextureFormat format);
    virtual ~GLTexture();

    virtual void Bind() const override;
    virtual void Unbind() const override;

private:
};

}

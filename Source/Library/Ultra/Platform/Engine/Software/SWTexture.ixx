export module Ultra.Platform.Renderer.SWTexture;

import Ultra.Renderer.Texture;

export namespace Ultra {

class SWTexture: public Texture {
public:
    SWTexture(const void *data, uint32_t width, uint32_t height, TextureFormat format);
    virtual ~SWTexture();

    virtual void Bind() const override;
    virtual void Unbind() const override;

private:
};

}

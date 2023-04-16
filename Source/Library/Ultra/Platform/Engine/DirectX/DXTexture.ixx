export module Ultra.Platform.Renderer.DXTexture;

import Ultra.Renderer.Texture;

export namespace Ultra {

class DXTexture: public Texture {
public:
    DXTexture(const void *data, uint32_t width, uint32_t height, TextureFormat format);
    virtual ~DXTexture();

    virtual void Bind() const override;
    virtual void Unbind() const override;

private:
};

}

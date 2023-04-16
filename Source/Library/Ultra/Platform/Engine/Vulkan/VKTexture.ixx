export module Ultra.Platform.Renderer.VKTexture;

import Ultra.Renderer.Texture;

export namespace Ultra {

class VKTexture: public Texture {
public:
    VKTexture(const void *data, uint32_t width, uint32_t height, TextureFormat format);
    virtual ~VKTexture();

    virtual void Bind() const override;
    virtual void Unbind() const override;

private:
};

}

export module Ultra.Platform.Renderer.DXTexture;

import Ultra.Renderer.Texture;

export namespace Ultra {

class DXTexture: public Texture {
public:
    DXTexture(const TextureProperties &properties, const void *data, size_t size);
    DXTexture(const TextureProperties &properties, const string &path);
    virtual ~DXTexture();

    virtual void Bind(uint32_t slot) const override;
    virtual void Unbind(uint32_t slot) const override;

private:
};

}

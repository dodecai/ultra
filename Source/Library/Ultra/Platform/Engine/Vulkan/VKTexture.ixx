export module Ultra.Platform.Renderer.VKTexture;

import Ultra.Renderer.Texture;

export namespace Ultra {

class VKTexture: public Texture {
public:
    VKTexture(const TextureProperties &properties, const void *data, size_t size);
    VKTexture(const TextureProperties &properties, string &path);
    virtual ~VKTexture();

    virtual void Bind(uint32_t slot) const override;
    virtual void Unbind(uint32_t slot) const override;

private:
};

}

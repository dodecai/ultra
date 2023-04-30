export module Ultra.Platform.Renderer.SWTexture;

import Ultra.Engine.Renderer.Texture;

export namespace Ultra {

class SWTexture: public Texture {
public:
    SWTexture(const TextureProperties &properties, const void *data, size_t size);
    SWTexture(const TextureProperties &properties, const string &path);
    virtual ~SWTexture();

    virtual void Bind(uint32_t slot) const override;
    virtual void Unbind(uint32_t slot) const override;

private:
};

}

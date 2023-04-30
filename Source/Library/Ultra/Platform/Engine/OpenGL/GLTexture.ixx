export module Ultra.Platform.Renderer.GLTexture;

import Ultra.Renderer.Texture;

export namespace Ultra {

class GLTexture: public Texture {
public:
    GLTexture(const TextureProperties &properties, const void *data, size_t size);
    GLTexture(const TextureProperties &properties, string &path);
    virtual ~GLTexture();

    virtual void Bind(uint32_t slot) const override;
    virtual void Unbind(uint32_t slot) const override;

private:
};

}

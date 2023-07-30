export module Ultra.Platform.Renderer.GLTexture;

import Ultra.Engine.Renderer.Texture;

export namespace Ultra {

class GLTexture: public Texture {
public:
    GLTexture(const TextureProperties &properties, const void *data, size_t size);
    GLTexture(const TextureProperties &properties, const string &path);
    virtual ~GLTexture();

    virtual void Bind(uint32_t slot) const override;
    void Draw(int index, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1) override;
    virtual void Unbind(uint32_t slot) const override;

private:
};

}

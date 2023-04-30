export module Ultra.Platform.Renderer.GLFramebuffer;

import Ultra.Engine.Renderer.Framebuffer;

export namespace Ultra {

class GLFramebuffer: public Framebuffer {
public:
    GLFramebuffer(uint32_t width, uint32_t height, TextureFormat format);
    virtual ~GLFramebuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual Reference<Texture> GetColorAttachment() const override;
    virtual Reference<Texture> GetDepthAttachment() const override;

private:
};

}

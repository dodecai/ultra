export module Ultra.Platform.Renderer.SWFramebuffer;

import Ultra.Renderer.Framebuffer;

export namespace Ultra {

class SWFramebuffer: public Framebuffer {
public:
    SWFramebuffer(uint32_t width, uint32_t height, TextureFormat format);
    virtual ~SWFramebuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual Reference<Texture> GetColorAttachment() const override;
    virtual Reference<Texture> GetDepthAttachment() const override;

private:
};

}

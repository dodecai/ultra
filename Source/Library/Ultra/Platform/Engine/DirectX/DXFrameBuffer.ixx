export module Ultra.Platform.Renderer.DXFramebuffer;

import Ultra.Renderer.Framebuffer;

export namespace Ultra {

class DXFramebuffer: public Framebuffer {
public:
    DXFramebuffer(uint32_t width, uint32_t height, TextureFormat format);
    virtual ~DXFramebuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual Reference<Texture> GetColorAttachment() const override;
    virtual Reference<Texture> GetDepthAttachment() const override;

private:
};

}

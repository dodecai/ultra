export module Ultra.Platform.Renderer.VKFramebuffer;

import Ultra.Engine.Renderer.Framebuffer;

export namespace Ultra {

class VKFramebuffer: public Framebuffer {
public:
    VKFramebuffer(uint32_t width, uint32_t height, TextureFormat format);
    virtual ~VKFramebuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual Reference<Texture> GetColorAttachment() const override;
    virtual Reference<Texture> GetDepthAttachment() const override;

private:
};

}

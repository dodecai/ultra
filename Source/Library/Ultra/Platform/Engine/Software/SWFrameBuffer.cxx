module Ultra.Platform.Renderer.SWFramebuffer;

namespace Ultra {

SWFramebuffer::SWFramebuffer(uint32_t width, uint32_t height, TextureFormat format): Framebuffer(width, height,format) {}

SWFramebuffer::~SWFramebuffer() {}


void SWFramebuffer::Bind() const {}

void SWFramebuffer::Unbind() const {}


Reference<Texture> SWFramebuffer::GetColorAttachment() const {
    return nullptr;
}

Reference<Texture> SWFramebuffer::GetDepthAttachment() const {
    return nullptr;
}

}

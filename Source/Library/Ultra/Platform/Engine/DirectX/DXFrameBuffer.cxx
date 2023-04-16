module Ultra.Platform.Renderer.DXFramebuffer;

namespace Ultra {

DXFramebuffer::DXFramebuffer(uint32_t width, uint32_t height, TextureFormat format): Framebuffer(width, height,format) {}

DXFramebuffer::~DXFramebuffer() {}


void DXFramebuffer::Bind() const {}

void DXFramebuffer::Unbind() const {}


Reference<Texture> DXFramebuffer::GetColorAttachment() const {
    return nullptr;
}

Reference<Texture> DXFramebuffer::GetDepthAttachment() const {
    return nullptr;
}

}

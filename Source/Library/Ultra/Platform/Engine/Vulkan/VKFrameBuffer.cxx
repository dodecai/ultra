module Ultra.Platform.Renderer.VKFramebuffer;

namespace Ultra {

VKFramebuffer::VKFramebuffer(uint32_t width, uint32_t height, TextureFormat format): Framebuffer(width, height, format) {}

VKFramebuffer::~VKFramebuffer() {}


void VKFramebuffer::Bind() const {}

void VKFramebuffer::Unbind() const {}


Reference<Texture> VKFramebuffer::GetColorAttachment() const {
    return nullptr;
}

Reference<Texture> VKFramebuffer::GetDepthAttachment() const {
    return nullptr;
}

}

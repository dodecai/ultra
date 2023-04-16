module Ultra.Platform.Renderer.GLFramebuffer;

namespace Ultra {

GLFramebuffer::GLFramebuffer(uint32_t width, uint32_t height, TextureFormat format): Framebuffer(width, height, format) {}

GLFramebuffer::~GLFramebuffer() {}


void GLFramebuffer::Bind() const {}

void GLFramebuffer::Unbind() const {}


Reference<Texture> GLFramebuffer::GetColorAttachment() const {
    return nullptr;
}

Reference<Texture> GLFramebuffer::GetDepthAttachment() const {
    return nullptr;
}

}

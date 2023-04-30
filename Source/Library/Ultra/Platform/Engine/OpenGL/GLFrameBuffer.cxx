module;

#include <glad/gl.h>

module Ultra.Platform.Renderer.GLFramebuffer;

namespace Ultra {

GLFramebuffer::GLFramebuffer(uint32_t width, uint32_t height, TextureFormat format): Framebuffer(width, height, format) {
    glCreateFramebuffers(1, &mFramebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);

    // Attach a texture, renderbuffer, or other attachment as needed
    // Example: attach a color texture
    glGenTextures(1, &mColorTextureID);
    glBindTexture(GL_TEXTURE_2D, mColorTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorTextureID, 0);

    // Check if the framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Failed to create framebuffer");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLFramebuffer::~GLFramebuffer() {
    glDeleteTextures(1, &mColorTextureID);
    glDeleteFramebuffers(1, &mFramebufferID);
}


void GLFramebuffer::Bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);
    //glViewport(0, 0, Properties.Width, Properties.Height);
}

void GLFramebuffer::Unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


Reference<Texture> GLFramebuffer::GetColorAttachment() const {
    return nullptr;
}

Reference<Texture> GLFramebuffer::GetDepthAttachment() const {
    return nullptr;
}

}

#include "GLFramebuffer.h"

#include <glad/glad.h>

namespace Ultra {

GLFramebuffer::GLFramebuffer(const FramebufferProperties &properties):
    Properties{properties} {
    Invalidate();
}

GLFramebuffer::~GLFramebuffer() {
    glDeleteFramebuffers(1, &RendererID);
}

void GLFramebuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, RendererID);
}

void GLFramebuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFramebuffer::Resize(uint32_t width, uint32_t height)
{
    Properties.Width = width;
    Properties.Height = height;

    Invalidate();
}

uint32_t GLFramebuffer::GetColorAttachmentRendererID() const {
    return ColorAttachment;
}

void GLFramebuffer::Invalidate() {
    if (RendererID) {
        glDeleteFramebuffers(1, &RendererID);
        glDeleteTextures(1, &ColorAttachment);
        glDeleteTextures(1, &DepthAttachment);
    }

    glCreateFramebuffers(1, &RendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, RendererID);

    // Color Buffer
    glCreateTextures(GL_TEXTURE_2D, 1, &ColorAttachment);
    glBindTexture(GL_TEXTURE_2D, ColorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Properties.Width, Properties.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorAttachment, 0);

    // Depth Buffer
    glCreateTextures(GL_TEXTURE_2D, 1, &DepthAttachment);
    glBindTexture(GL_TEXTURE_2D, DepthAttachment);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, Properties.Width, Properties.Height);
    //glTexImage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, Properties.Width, Properties.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthAttachment, 0);

    // ToDo: Replace with assert
    if (!glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
        APP_LOG_ERROR("Failed to create the framebuffer");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const FramebufferProperties &GLFramebuffer::GetProperties() const {
    return Properties;
}

}

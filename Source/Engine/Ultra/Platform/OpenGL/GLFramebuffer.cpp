#include "GLFramebuffer.h"

#include "Ultra/Core.h"

#include <glad/gl.h>


namespace Ultra {

GLFramebuffer::GLFramebuffer(const FramebufferProperties &properties):
    Properties { properties } {
    Invalidate();
}

GLFramebuffer::~GLFramebuffer() {
    glDeleteFramebuffers(1, &RendererID);
}

void GLFramebuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, RendererID);
    glViewport(0, 0, Properties.Width, Properties.Height);
}

void GLFramebuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


const FramebufferProperties &GLFramebuffer::GetProperties() const {
    return Properties;
}

uint32_t GLFramebuffer::GetColorAttachmentRendererID() const {
    return ColorAttachment;
}

uint32_t GLFramebuffer::GetDepthAttachmentRendererID() const {
    return uint32_t();
}

uint32_t GLFramebuffer::GetRendererID() const {
    return RendererID;
}


void GLFramebuffer::Resize(uint32_t width, uint32_t height, bool reload) {
    if (!reload || (Properties.Width != width || Properties.Height != height))
        return;

    Properties.Width = width;
    Properties.Height = height;

    Invalidate();
}


void GLFramebuffer::Invalidate() {
    if (RendererID) {
        glDeleteFramebuffers(1, &RendererID);
        glDeleteTextures(1, &ColorAttachment);
        glDeleteTextures(1, &DepthAttachment);
    }

    glCreateFramebuffers(1, &RendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, RendererID);

    if (Properties.Samples > 1) {
        // Color Buffer
        glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &ColorAttachment);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, ColorAttachment);

        switch (Properties.Format) {
            case FramebufferFormat::RGBA16F: {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Properties.Samples, GL_RGBA16F, Properties.Width, Properties.Height, GL_FALSE);
                break;
            }

            case FramebufferFormat::RGBA32F: {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Properties.Samples, GL_RGBA32F, Properties.Width, Properties.Height, GL_FALSE);
                break;
            }

            default:
            case FramebufferFormat::RGBA8: {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Properties.Samples, GL_RGBA8, Properties.Width, Properties.Height, GL_FALSE);
                break;
            }
        }
        //glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, ColorAttachment, 0);

        // Depth Buffer
        glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &DepthAttachment);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, DepthAttachment);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Properties.Samples, GL_DEPTH24_STENCIL8, Properties.Width, Properties.Height, GL_FALSE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, DepthAttachment, 0);
    } else {
        // Color Buffer
        glCreateTextures(GL_TEXTURE_2D, 1, &ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, ColorAttachment);

        switch (Properties.Format) {
            case FramebufferFormat::RGBA16F: {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Properties.Width, Properties.Height, GL_FALSE, GL_RGBA, GL_FLOAT, nullptr);
                break;
            }

            case FramebufferFormat::RGBA32F: {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, Properties.Width, Properties.Height, GL_FALSE, GL_RGBA, GL_FLOAT, nullptr);
                break;
            }

            default:
            case FramebufferFormat::RGBA8: {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Properties.Width, Properties.Height, GL_FALSE, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
                break;
            }
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorAttachment, 0);

        // Depth Buffer
        glCreateTextures(GL_TEXTURE_2D, 1, &DepthAttachment);
        glBindTexture(GL_TEXTURE_2D, DepthAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, Properties.Width, Properties.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthAttachment, 0);
    }

    // ToDo: Replace with assert
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        //APP_ASSERT(nullptr, "Failed to create the framebuffer");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}

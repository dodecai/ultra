module Ultra.Platform.Renderer.GLFramebuffer;

import <glad/gl.h>;

namespace Ultra {

// Note: glDrawBuffers(count, buffers) -> Draw multiple attachments at once

GLFramebuffer::GLFramebuffer(uint32_t width, uint32_t height, TextureFormat format): Framebuffer(width, height, format) {
    glCreateFramebuffers(1, &mFramebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);

    // Attach a texture, renderbuffer, or other attachment as needed
    // Example: attach a color texture
    glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &mColorTextureID);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mColorTextureID);


    //switch (format) {
    //    case TextureFormat::RGBA: {
    //        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, mProperties.Samples, GL_RGBA8, mProperties.Width, mProperties.Height, GL_FALSE);
    //        break;
    //    }

    //    case TextureFormat::RGBA16F: {
    //        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, mProperties.Samples, GL_RGBA16F, mProperties.Width, mProperties.Height, GL_FALSE);
    //        break;
    //    }

    //    case TextureFormat::RGBA32F: {
    //        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, mProperties.Samples, GL_RGBA32F, mProperties.Width, mProperties.Height, GL_FALSE);
    //        break;
    //    }

    //    default:
    //}

    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, mColorTextureID, 0);


    glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &mDepthTextureID);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mDepthTextureID);
    //glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Properties.Samples, GL_DEPTH24_STENCIL8, Properties.Width, Properties.Height, GL_FALSE);
    
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, mDepthTextureID, 0);



            // Color Buffer
    //glCreateTextures(GL_TEXTURE_2D, 1, &ColorAttachment);
    //glBindTexture(GL_TEXTURE_2D, ColorAttachment);

    //switch (Properties.Format) {
    //    case FramebufferFormat::RGBA16F: {
    //        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Properties.Width, Properties.Height, GL_FALSE, GL_RGBA, GL_FLOAT, nullptr);
    //        break;
    //    }

    //    case FramebufferFormat::RGBA32F: {
    //        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, Properties.Width, Properties.Height, GL_FALSE, GL_RGBA, GL_FLOAT, nullptr);
    //        break;
    //    }

    //    default:
    //    case FramebufferFormat::RGBA8: {
    //        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Properties.Width, Properties.Height, GL_FALSE, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    //        break;
    //    }
    //}

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glBindTexture(GL_TEXTURE_2D, 0);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorAttachment, 0);

    //// Depth Buffer
    //glCreateTextures(GL_TEXTURE_2D, 1, &DepthAttachment);
    //glBindTexture(GL_TEXTURE_2D, DepthAttachment);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, Properties.Width, Properties.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    //glBindTexture(GL_TEXTURE_2D, 0);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthAttachment, 0);


    // Check if the framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Failed to create framebuffer");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLFramebuffer::~GLFramebuffer() {
    glDeleteTextures(1, &mColorTextureID);
    glDeleteTextures(1, &mDepthTextureID);
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

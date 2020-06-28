#pragma once

#include "Engine/Renderer/Framebuffer.h"

namespace Ultra {

class GLFramebuffer: public Framebuffer {
    uint32_t RendererID;
    uint32_t ColorAttachment;
    uint32_t DepthAttachment;
    FramebufferProperties Properties;
public:
    GLFramebuffer(const FramebufferProperties &properties);
    virtual ~GLFramebuffer();
    
    virtual void Bind() override;
    virtual void Unbind() override;

    virtual uint32_t GetColorAttachmentRendererID() const override;

    void Invalidate();
    virtual const FramebufferProperties &GetProperties() const override;
};

}

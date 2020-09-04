#pragma once

#include "Ultra/Renderer/Framebuffer.h"

namespace Ultra {

class GLFramebuffer: public Framebuffer {
    uint32_t RendererID = 0;
    uint32_t ColorAttachment = 0;
    uint32_t DepthAttachment = 0;
    FramebufferProperties Properties;

public:
    GLFramebuffer(const FramebufferProperties &properties);
    virtual ~GLFramebuffer();
    
    // Methods
    virtual void Bind() override;
    virtual void Unbind() override;

    // Accessors
    virtual const FramebufferProperties &GetProperties() const override;
    virtual uint32_t GetColorAttachmentRendererID() const override;
    virtual uint32_t GetDepthAttachmentRendererID() const override;
    virtual uint32_t GetRendererID() const override;

    // Mutators
    virtual void Resize(uint32_t width, uint32_t height, bool reload) override;

private:
    // Internal Methods
    void Invalidate();
};

}

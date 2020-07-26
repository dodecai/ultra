#pragma once

#include "Ultra.pch"
#include "Core.h"

namespace Ultra {

enum class FramebufferFormat {
    Null    = 0,
    RGBA8   = 1,
    RGBA16F = 2,
    RGBA32F = 3,
};

struct FramebufferProperties {
    uint32_t Width = 800;
    uint32_t Height = 600;
    uint32_t Samples = 1;

    FramebufferFormat Format = FramebufferFormat::RGBA16F;

    bool SwapChainTarget = false;
};

class Framebuffer {
public:
    virtual ~Framebuffer() = default;

    // Methods
    static Reference<Framebuffer> Create(const FramebufferProperties &properties);
    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    // Accessors
    virtual const FramebufferProperties &GetProperties() const = 0;
    virtual uint32_t GetColorAttachmentRendererID() const = 0;
    virtual uint32_t GetDepthAttachmentRendererID() const = 0;
    virtual uint32_t GetRendererID() const = 0;

    // Mutators
    virtual void Resize(uint32_t width, uint32_t height, bool reload = false) = 0;
};

}

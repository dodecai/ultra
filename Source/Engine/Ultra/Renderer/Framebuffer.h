#pragma once

#include "Ultra.pch"
#include "Ultra/Core.h"

#include <glm/glm.hpp>

namespace Ultra {

enum class FramebufferFormat {
    Null    = 0,
    RGBA8   = 1,
    RGBA16F = 2,
    RGBA32F = 3,
};

struct FramebufferProperties {
    glm::vec4 ClearColor = {};
    uint32_t Width = 1280;
    uint32_t Height = 1024;
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
    virtual void BindTexture(uint32_t slot = 0) const = 0;
    virtual void Unbind() = 0;

    // Accessors
    virtual const FramebufferProperties &GetProperties() const = 0;
    virtual uint32_t GetColorAttachmentRendererID() const = 0;
    virtual uint32_t GetDepthAttachmentRendererID() const = 0;
    virtual uint32_t GetRendererID() const = 0;

    // Mutators
    virtual void Resize(uint32_t width, uint32_t height, bool reload = false) = 0;
};

class FramebufferPool final {
public:
    FramebufferPool(uint32_t maxBuffers = 32);
    ~FramebufferPool();

    void Add(const Reference<Framebuffer> &framebuffer);
    weak_ptr<Framebuffer> Allocate();

    static FramebufferPool *GetGlobal() { return Instance; }
    const vector<Reference<Framebuffer>> &GetPool() const { return Pool; }


private:
    vector<Reference<Framebuffer>> Pool;
    static inline FramebufferPool *Instance = nullptr;
};

}

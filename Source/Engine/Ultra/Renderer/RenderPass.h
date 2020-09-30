#pragma once

#include "Framebuffer.h"

namespace Ultra {

struct RenderPassProperties {
    Reference<Framebuffer> Framebuffer;
};

class RenderPass {
public:
    RenderPass() = default;
    virtual ~RenderPass() = default;

    static Reference<RenderPass> Create(const RenderPassProperties &properties);

    virtual RenderPassProperties &GetProperties() = 0;
    virtual const RenderPassProperties &GetProperties() const = 0;

};

}

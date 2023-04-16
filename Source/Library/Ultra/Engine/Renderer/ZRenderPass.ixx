module;

#include <glm/glm.hpp>

export module Ultra.Engine.RenderPass;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.GFX.Context;
export import Ultra.Engine.RendererData;
export import Ultra.Engine.Framebuffer;

export namespace Ultra {

struct RenderPassProperties {
    Reference<Framebuffer> Framebuffer;
    string Name;
    glm::vec4 MarkerColor;
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

#pragma once

#include "Ultra/Renderer/RenderPass.h"

namespace Ultra {

class GLRenderPass: public RenderPass {
public:
    GLRenderPass(const RenderPassProperties &properties);
    virtual ~GLRenderPass();

    virtual RenderPassProperties &GetProperties() override;
    virtual const RenderPassProperties &GetProperties() const override;

private:
    RenderPassProperties Properties;
};

}

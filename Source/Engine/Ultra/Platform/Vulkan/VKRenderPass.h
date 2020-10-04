#pragma once

#include "Ultra/Renderer/RenderPass.h"

#include "VKContext.h"

namespace Ultra {

class VKRenderPass: public RenderPass {
public:
    VKRenderPass(const RenderPassProperties &properties);
    virtual ~VKRenderPass();

    virtual RenderPassProperties &GetProperties() override;
    virtual const RenderPassProperties &GetProperties() const override;

private:
    RenderPassProperties Properties;
    vk::RenderPass mRenderPass;
};

}

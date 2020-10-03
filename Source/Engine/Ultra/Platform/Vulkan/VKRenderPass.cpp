#include "VKRenderpass.h"

namespace Ultra {
VKRenderPass::VKRenderPass(const RenderPassProperties &properties) {}

VKRenderPass::~VKRenderPass() {}

RenderPassProperties &VKRenderPass::GetProperties() {
    return Properties;
}

const RenderPassProperties &VKRenderPass::GetProperties() const {
    return Properties;
}
}

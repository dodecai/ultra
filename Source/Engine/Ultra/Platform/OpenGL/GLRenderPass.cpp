#include "GLRenderPass.h"

namespace Ultra {
GLRenderPass::GLRenderPass(const RenderPassProperties &properties):
    Properties { properties } {}

GLRenderPass::~GLRenderPass() {}

RenderPassProperties &GLRenderPass::GetProperties() {
    return Properties;
}

const RenderPassProperties &GLRenderPass::GetProperties() const {
    return Properties;
}

}

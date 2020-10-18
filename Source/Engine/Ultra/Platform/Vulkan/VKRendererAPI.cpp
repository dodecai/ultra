#include "VKRendererAPI.h"

#include <vulkan/vulkan.hpp>
#include <Omnia/System/FileSystem.h>

namespace Ultra {

void VKRendererAPI::Load() {
    sContext = reinterpret_cast<VKContext *>(&Application::GetContext());
    mDevice = sContext->GetDevice();

    SetViewport(0, 0, 0, 0);
}

void VKRendererAPI::Unload() {
}

void VKRendererAPI::Clear() {
}

void VKRendererAPI::DrawIndexed(uint32_t count, PrimitiveType type, bool depthTest) {
    auto &drawCommandBuffer = sContext->GetSwapChain()->GetCurrentDrawCommandBuffer();

    vk::RenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.renderPass = sContext->GetSwapChain()->GetRenderPass();
    renderPassInfo.framebuffer = sContext->GetSwapChain()->GetCurrentFramebuffer();
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = sContext->GetSwapChain()->GetSurfaceProperties().RenderArea.extent;
    renderPassInfo.clearValueCount = 2;
    renderPassInfo.pClearValues = sContext->GetSwapChain()->GetSurfaceProperties().ClearValues;

    drawCommandBuffer.begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eSimultaneousUse));

    drawCommandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
    drawCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mPipelineA->GetNativePipeline());
    drawCommandBuffer.draw(3, 1, 0, 0);
    drawCommandBuffer.endRenderPass();

    drawCommandBuffer.end();
}

void VKRendererAPI::SetClearColor(const glm::vec4 &color) {
}

void VKRendererAPI::SetLineThickness(float value) {
}

void VKRendererAPI::SetPolygonMode(PolygonMode mode) {
}

void VKRendererAPI::SetViewport(const int32_t x, const int32_t y, const uint32_t width, const uint32_t height) {
    PipelineProperties properties;
    mPipelineA = CreateReference<VKPipeline>(properties);
}

}

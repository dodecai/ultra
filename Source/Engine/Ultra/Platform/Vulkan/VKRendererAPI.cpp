#include "VKRendererAPI.h"

#include <vulkan/vulkan.hpp>
#include <Omnia/System/FileSystem.h>

namespace Ultra {

void VKRendererAPI::Load() {
    sContext = reinterpret_cast<VKContext *>(&Application::GetContext());
    mDevice = sContext->GetDevice();

    PipelineProperties properties;
    mPipelineA = CreateReference<VKPipeline>(properties);
}

void VKRendererAPI::Unload() {
}

void VKRendererAPI::Clear() {
}

void VKRendererAPI::DrawIndexed(uint32_t count, PrimitiveType type, bool depthTest) {
    auto &drawCommandBuffer = sContext->GetSwapChain()->GetCurrentDrawCommandBuffer();
    drawCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mPipelineA->GetNativePipeline());
    mPipelineA->Bind(drawCommandBuffer);
    //drawCommandBuffer.draw(3, 1, 0, 0);
    //drawCommandBuffer.drawIndexed(count, 1, 0, 0, 0);
}

void VKRendererAPI::SetClearColor(const glm::vec4 &color) {
    //sContext->GetSwapChain()->GetSurfaceProperties().ClearValues[0].color = array<float, 4>{ color.r, color.g, color.b, color.a };
}

void VKRendererAPI::SetLineThickness(float value) {
}

void VKRendererAPI::SetPolygonMode(PolygonMode mode) {
}

void VKRendererAPI::SetViewport(const int32_t x, const int32_t y, const uint32_t width, const uint32_t height) {
}

}

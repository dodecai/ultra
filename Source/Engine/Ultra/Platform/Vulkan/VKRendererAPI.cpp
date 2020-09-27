#include "VKRendererAPI.h"

#include "VKTest.h"

namespace Ultra {

VKTest *VulkanTest;

void VKRendererAPI::Load() {
    VulkanTest = new VKTest();
}

void VKRendererAPI::Unload() {
}

void VKRendererAPI::Clear() {
    VulkanTest->Update();
}

void VKRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray, uint32_t indexCount) {
}
void VKRendererAPI::DrawIndexed(uint32_t count, Type type, bool depthTest) {}

void VKRendererAPI::SetClearColor(const glm::vec4 &color) {}

void VKRendererAPI::SetLineThickness(float value) {}

void VKRendererAPI::SetPolygonMode(PolygonMode mode) {}

void VKRendererAPI::SetViewport(const int32_t x, const int32_t y, const uint32_t width, const uint32_t height) {}

}

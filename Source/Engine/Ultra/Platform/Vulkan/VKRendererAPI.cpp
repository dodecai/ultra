#include "VKRendererAPI.h"

namespace Ultra {
void VKRendererAPI::Load() {}
void VKRendererAPI::Unload() {}
void VKRendererAPI::Clear() {}

void VKRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray, uint32_t indexCount) {}
void VKRendererAPI::DrawIndexed(uint32_t count, Type type, bool depthTest) {}

void VKRendererAPI::SetClearColor(const glm::vec4 &color) {}
void VKRendererAPI::SetLineThickness(float value) {}
void VKRendererAPI::SetPolygonMode(PolygonMode mode) {}
void VKRendererAPI::SetViewport(const int32_t x, const int32_t y, const uint32_t width, const uint32_t height) {}

}

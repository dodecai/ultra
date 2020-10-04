#include "VKRendererAPI.h"

#include "vulkan/vulkan.hpp"

namespace Ultra {

void VKRendererAPI::Load() {
    sContext = reinterpret_cast<VKContext *>(&Application::GetContext());
    sDevice = sContext->GetDevice();
}

void VKRendererAPI::Unload() {
}

void VKRendererAPI::Clear() {
}

void VKRendererAPI::DrawIndexed(uint32_t count, PrimitiveType type, bool depthTest) {
}

void VKRendererAPI::SetClearColor(const glm::vec4 &color) {
}

void VKRendererAPI::SetLineThickness(float value) {
}

void VKRendererAPI::SetPolygonMode(PolygonMode mode) {
}

void VKRendererAPI::SetViewport(const int32_t x, const int32_t y, const uint32_t width, const uint32_t height) {
}

}

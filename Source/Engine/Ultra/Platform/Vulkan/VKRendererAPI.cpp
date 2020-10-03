#include "VKRendererAPI.h"

#include "vulkan/vulkan.hpp"

namespace Ultra {

struct Vertex {
    float Color[3];
    float Position[3];
};

struct VertexBuffer {
    vk::DeviceMemory Memory;
    vk::Buffer Buffer;
} Vertices;

struct IndexBuffer {
    vk::DeviceMemory Memory;
    vk::Buffer Buffer;
    uint32_t Count;
} Indices;

struct UniformBlockLayout {
    glm::mat4 projectionMatrix;
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
} uboVS;

void VKRendererAPI::Load() {
    pContext = reinterpret_cast<VKContext *>(&Application::GetContext());
}

void VKRendererAPI::Unload() {
}

void VKRendererAPI::Clear() {
    pContext->GetSwapChain()->Test();
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

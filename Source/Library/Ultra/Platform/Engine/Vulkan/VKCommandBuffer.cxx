module Ultra.Platform.Renderer.VKCommandBuffer;

namespace Ultra {

VKCommandBuffer::VKCommandBuffer() {}

VKCommandBuffer::~VKCommandBuffer() {}


void VKCommandBuffer::Begin() {}

void VKCommandBuffer::End() {}


void VKCommandBuffer::Clear(float r, float g, float b, float a) {}

void VKCommandBuffer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {}


void VKCommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {}

void VKCommandBuffer::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {}

void VKCommandBuffer::DrawIndexed(size_t count, PrimitiveType type, bool depthTest) {}


void VKCommandBuffer::Execute() {}

}

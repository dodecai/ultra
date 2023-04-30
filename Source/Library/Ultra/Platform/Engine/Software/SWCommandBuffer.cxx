module Ultra.Platform.Renderer.SWCommandBuffer;

namespace Ultra {

SWCommandBuffer::SWCommandBuffer() {}

SWCommandBuffer::~SWCommandBuffer() {}


void SWCommandBuffer::Begin() {}

void SWCommandBuffer::End() {}


void SWCommandBuffer::Clear(float r, float g, float b, float a) {}

void SWCommandBuffer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {}


void SWCommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {}

void SWCommandBuffer::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {}

void SWCommandBuffer::DrawIndexed(size_t count, PrimitiveType type, bool depthTest) {}


void SWCommandBuffer::Execute() {}

}

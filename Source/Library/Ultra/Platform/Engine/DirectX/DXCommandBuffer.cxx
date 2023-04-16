module Ultra.Platform.Renderer.DXCommandBuffer;

namespace Ultra {

DXCommandBuffer::DXCommandBuffer() {}

DXCommandBuffer::~DXCommandBuffer() {}


void DXCommandBuffer::Begin() {}

void DXCommandBuffer::End() {}


void DXCommandBuffer::Clear(float r, float g, float b, float a) {}

void DXCommandBuffer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {}


void DXCommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {}

void DXCommandBuffer::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {}


void DXCommandBuffer::Execute() {}

}

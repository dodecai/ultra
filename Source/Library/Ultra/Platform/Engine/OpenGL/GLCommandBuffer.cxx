module;

#include <glad/gl.h>

module Ultra.Platform.Renderer.GLCommandBuffer;

namespace Ultra {

GLCommandBuffer::GLCommandBuffer() {}

GLCommandBuffer::~GLCommandBuffer() {}


void GLCommandBuffer::Begin() {
    // ToDo: Not needed, but it can be done with lambdas
}

void GLCommandBuffer::End() {
    // ToDo: Not needed, but it can be done with lambdas
}


void GLCommandBuffer::Clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLCommandBuffer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {}


void GLCommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {}

void GLCommandBuffer::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {}


void GLCommandBuffer::Execute() {}

}

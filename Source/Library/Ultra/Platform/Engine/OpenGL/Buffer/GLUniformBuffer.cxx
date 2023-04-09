module;

#include <glad/gl.h>

module Ultra.Platform.Engine.GLUniformBuffer;

namespace Ultra {

GLUniformBuffer::GLUniformBuffer(size_t size, uint32_t binding): mBuffer(size), mBinding(binding) {
    glCreateBuffers(1, &mRendererID);
    glNamedBufferData(mRendererID, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, mRendererID);
}

GLUniformBuffer::~GLUniformBuffer() {
    mBuffer.Release();
    glDeleteBuffers(1, &mRendererID);
}

void GLUniformBuffer::SetData(const void *data, size_t size, size_t offset) {
    mBuffer = BufferData::Copy(data, size);
    glNamedBufferSubData(mRendererID, offset, mBuffer.Size, mBuffer.Data);
}

}

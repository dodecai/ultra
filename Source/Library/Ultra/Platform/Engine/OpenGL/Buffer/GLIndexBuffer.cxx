module;

#include <glad/gl.h>

module Ultra.Platform.Engine.GLIndexBuffer;

namespace Ultra {

GLIndexBuffer::GLIndexBuffer(const void *data, size_t size) {
    mBuffer = BufferData::Copy(data, size);
    glCreateBuffers(1, &mRendererID);
    glNamedBufferData(mRendererID, mBuffer.Size, mBuffer.Data, GL_STATIC_DRAW);
}

GLIndexBuffer::GLIndexBuffer(size_t size): mBuffer(size) {
    glCreateBuffers(1, &mRendererID);
    glNamedBufferData(mRendererID, mBuffer.Size, nullptr, GL_DYNAMIC_DRAW);
}

GLIndexBuffer::~GLIndexBuffer() {
    glDeleteBuffers(1, &mRendererID);
}

void GLIndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
}

void GLIndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
}

void GLIndexBuffer::SetData(const void *data, size_t size, size_t offset) {
    mBuffer = BufferData::Copy(data, size);
    glNamedBufferSubData(mRendererID, offset, mBuffer.Size, mBuffer.Data);
}

}

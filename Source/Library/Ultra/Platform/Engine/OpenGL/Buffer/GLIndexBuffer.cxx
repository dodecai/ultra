module;

#include <glad/gl.h>

module Ultra.Platform.Engine.GLIndexBuffer;

namespace Ultra {

GLIndexBuffer::GLIndexBuffer(uint32_t size): mSize(size) {
    glCreateBuffers(1, &mRendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID); // ToDo: Remove
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW); // ToDo: Remove
    glNamedBufferData(mRendererID, size, nullptr, GL_DYNAMIC_DRAW);
}

GLIndexBuffer::GLIndexBuffer(void *data, uint32_t size): mSize(size) {
    mBuffer = BufferData::Copy(data, size);

    glCreateBuffers(1, &mRendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID); // ToDo: Remove
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(uint32_t), (uint32_t *)data, GL_STATIC_DRAW); // ToDo: Remove
    glNamedBufferData(mRendererID, size, mBuffer.Data, GL_STATIC_DRAW);
}

GLIndexBuffer::~GLIndexBuffer() {
    glDeleteBuffers(1, &mRendererID);
}

void GLIndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
}

void GLIndexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLIndexBuffer::SetData(void *data, uint32_t size, uint32_t offset) {
    mBuffer = BufferData::Copy(data, size);
    mSize = size;
    glNamedBufferSubData(mRendererID, offset, mSize, mBuffer.Data);
}

}

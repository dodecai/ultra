module;

#include <glad/gl.h>

module Ultra.Platform.Engine.GLVertexBuffer;

namespace Ultra {

static GLenum GetGLDrawType(VertexBufferType type) {
    switch (type) {
        case VertexBufferType::Dynamic: return GL_DYNAMIC_DRAW;
        case VertexBufferType::Static: return GL_STATIC_DRAW;
    }
    LogFatal("The specified buffer draw type is unknown!");
    return 0;
}

// Vertex Buffer
GLVertexBuffer::GLVertexBuffer(uint32_t size, VertexBufferType type): mSize(size), mType(type) {
    glCreateBuffers(1, &mRendererID);
    glBindBuffer(GL_ARRAY_BUFFER, mRendererID); // ToDo: Remove
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GetGLDrawType(mType)); // ToDo: Remove
    glNamedBufferData(mRendererID, size, nullptr, GetGLDrawType(mType));
}

GLVertexBuffer::GLVertexBuffer(void *data, uint32_t size, VertexBufferType type): mSize(size), mType(type) {
    mBuffer = BufferData::Copy(data, size);

    glCreateBuffers(1, &mRendererID);
    glBindBuffer(GL_ARRAY_BUFFER, mRendererID); // ToDo: Remove
    glBufferData(GL_ARRAY_BUFFER, size, data, GetGLDrawType(mType)); // ToDo: Remove
    glNamedBufferData(mRendererID, size, mBuffer.Data, GetGLDrawType(mType));
}

GLVertexBuffer::~GLVertexBuffer() {
    glDeleteBuffers(1, &mRendererID);
}

const VertexBufferLayout &GLVertexBuffer::GetLayout() const {
    return mLayout;
}

void GLVertexBuffer::SetLayout(const VertexBufferLayout &layout) {
    mLayout = layout;
}

void GLVertexBuffer::SetData(void *data, uint32_t size, uint32_t offset) {
    mBuffer = BufferData::Copy(data, size);
    mSize = size;
    glNamedBufferSubData(mRendererID, offset, mSize, mBuffer.Data);
}

void GLVertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
}

void GLVertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}

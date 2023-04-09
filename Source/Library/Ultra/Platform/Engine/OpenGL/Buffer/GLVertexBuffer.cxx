module;

#include <glad/gl.h>

module Ultra.Platform.Engine.GLVertexBuffer;

namespace Ultra {

static GLenum GetGLDrawType(VertexBufferType type) {
    switch (type) {
        case VertexBufferType::Static:  return GL_STATIC_DRAW;
        case VertexBufferType::Dynamic: return GL_DYNAMIC_DRAW;
    }
    LogFatal("The specified buffer draw type is unknown!");
    return 0;
}

GLVertexBuffer::GLVertexBuffer(const void *data, size_t size, VertexBufferType type): mType(type) {
    mBuffer = BufferData::Copy(data, size);
    glCreateBuffers(1, &mRendererID);
    glNamedBufferData(mRendererID, size, mBuffer.Data, GetGLDrawType(mType));
}

GLVertexBuffer::GLVertexBuffer(size_t size, VertexBufferType type): mType(type) {
    mBuffer = BufferData(size);
    glCreateBuffers(1, &mRendererID);
    glNamedBufferData(mRendererID, size, nullptr, GetGLDrawType(mType));
}

GLVertexBuffer::~GLVertexBuffer() {
    glDeleteBuffers(1, &mRendererID);
}

void GLVertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
}

void GLVertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLVertexBuffer::SetData(void *data, size_t size, size_t offset) {
    mBuffer = BufferData::Copy(data, size);
    glNamedBufferSubData(mRendererID, offset, mBuffer.Size, mBuffer.Data);
}

}

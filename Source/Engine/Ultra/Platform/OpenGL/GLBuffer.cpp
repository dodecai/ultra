#include "GLBuffer.h"

#include <glad/gl.h>

namespace Ultra {

static GLenum GetGLDrawType(VertexBufferType type) {
    switch (type) {
        case VertexBufferType::Dynamic: return GL_DYNAMIC_DRAW;
        case VertexBufferType::Static: return GL_STATIC_DRAW;
    }
    AppLogCritical("The specified buffer draw type is unknown!");
    return 0;
}

// Vertex Buffer
GLVertexBuffer::GLVertexBuffer(uint32_t size, VertexBufferType type): mSize(size), mType(type) {
	glCreateBuffers(1, &RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, RendererID); // ToDo: Remove
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GetGLDrawType(mType)); // ToDo: Remove
    glNamedBufferData(RendererID, size, nullptr, GetGLDrawType(mType));
}

GLVertexBuffer::GLVertexBuffer(void *data, uint32_t size, VertexBufferType type): mSize(size), mType(type) {
    mBuffer = DataBuffer::Copy(data, size);

	glCreateBuffers(1, &RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, RendererID); // ToDo: Remove
    glBufferData(GL_ARRAY_BUFFER, size, data, GetGLDrawType(mType)); // ToDo: Remove
    glNamedBufferData(RendererID, size, mBuffer.Data, GetGLDrawType(mType));
}

GLVertexBuffer::~GLVertexBuffer() {
	glDeleteBuffers(1, &RendererID);
}

const BufferLayout &GLVertexBuffer::GetLayout() const {
	return mLayout;
}

void GLVertexBuffer::SetLayout(const BufferLayout &layout) {
	mLayout = layout;
}

void GLVertexBuffer::SetData(void *data, uint32_t size, uint32_t offset) {
    mBuffer = DataBuffer::Copy(data, size);
    mSize = size;
    glNamedBufferSubData(RendererID, offset, mSize, mBuffer.Data);
}

void GLVertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, RendererID);
}

void GLVertexBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}



// Index Buffer
GLIndexBuffer::GLIndexBuffer(uint32_t size): mSize(size) {
    glCreateBuffers(1, &RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID); // ToDo: Remove
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW); // ToDo: Remove
    glNamedBufferData(RendererID, size, nullptr, GL_DYNAMIC_DRAW);
}

GLIndexBuffer::GLIndexBuffer(void *data, uint32_t size): mSize(size) {
    mBuffer = DataBuffer::Copy(data, size);

	glCreateBuffers(1, &RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID); // ToDo: Remove
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(uint32_t), (uint32_t *)data, GL_STATIC_DRAW); // ToDo: Remove
    glNamedBufferData(RendererID, size, mBuffer.Data, GL_STATIC_DRAW);
}

GLIndexBuffer::~GLIndexBuffer() {
	glDeleteBuffers(1, &RendererID);
}

void GLIndexBuffer::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID);
}

void GLIndexBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLIndexBuffer::SetData(void *data, uint32_t size, uint32_t offset) {
    mBuffer = DataBuffer::Copy(data, size);
    mSize = size;
    glNamedBufferSubData(RendererID, offset, mSize, mBuffer.Data);
}

}

#include "GLBuffer.h"

#include <glad/gl.h>

namespace Ultra {

// Vertex Buffer
GLVertexBuffer::GLVertexBuffer(uint32_t size) {
	glCreateBuffers(1, &RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, RendererID);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

GLVertexBuffer::GLVertexBuffer(float *vertices, uint32_t size) {
	glCreateBuffers(1, &RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, RendererID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
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

void GLVertexBuffer::SetData(const void *data, uint32_t size) {
	glBindBuffer(GL_ARRAY_BUFFER, RendererID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}


void GLVertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, RendererID);
}

void GLVertexBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


// Index Buffer
GLIndexBuffer::GLIndexBuffer(uint32_t *indices, uint32_t count):
	mCount{ count } {
	glCreateBuffers(1, &RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

GLIndexBuffer::~GLIndexBuffer() {
	glDeleteBuffers(1, &RendererID);
}

void GLIndexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, RendererID);
}

void GLIndexBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}

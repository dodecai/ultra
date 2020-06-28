#include "GLVertexArray.h"

#include <glad/glad.h>

#include "Engine/Renderer/Buffer.h"

namespace Ultra {

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
	switch (type) {
		case ShaderDataType::Bool:			return GL_BOOL;
		case ShaderDataType::Float:			return GL_FLOAT;
		case ShaderDataType::Float2:		return GL_FLOAT;
		case ShaderDataType::Float3:		return GL_FLOAT;
		case ShaderDataType::Float4:		return GL_FLOAT;
		case ShaderDataType::Int:			return GL_INT;
		case ShaderDataType::Int2:			return GL_INT;
		case ShaderDataType::Int3:			return GL_INT;
		case ShaderDataType::Int4:			return GL_INT;
		case ShaderDataType::Mat3:			return GL_FLOAT;
		case ShaderDataType::Mat4:			return GL_FLOAT;
		default:							return 0;
	}
}


GLVertexArray::GLVertexArray() {
	glCreateVertexArrays(1, &mRendererID);
}

GLVertexArray::~GLVertexArray() {
	glDeleteVertexArrays(1, &mRendererID);
}

void GLVertexArray::Bind() const {
	glBindVertexArray(mRendererID);
}

void GLVertexArray::Unbind() const {
	glBindVertexArray(0);
}

void  GLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) {
	glBindVertexArray(mRendererID);
	vertexBuffer->Bind();

	// if (vertexBuffer->GetLayout().GetElements().size() > 0) --> Error no layout

	uint32_t index = 0;
	const auto &layout = vertexBuffer->GetLayout();
	for (const auto &element : layout) {
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index,
							  element.GetComponentCount(),
							  ShaderDataTypeToOpenGLBaseType(element.Type),
							  element.Normalized ? GL_TRUE : GL_FALSE,
							  layout.GetStride(),
							  (const void *)(uint64_t)element.Offset
		);
		index++;
	}

	mVertexBuffers.push_back(vertexBuffer);
}

void GLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) {
	glBindVertexArray(mRendererID);
	indexBuffer->Bind();

	mIndexBuffer = indexBuffer;
}

const std::vector<std::shared_ptr<VertexBuffer>> &GLVertexArray::GetVertexBuffers() const {
	return mVertexBuffers;
}

const std::shared_ptr<IndexBuffer> &GLVertexArray::GetIndexBuffer() const {
	return mIndexBuffer;
}

}

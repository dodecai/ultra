#include "GLVertexArray.h"

#include <glad/gl.h>

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


// Default
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


// Accessors
RendererID GLVertexArray::GetRendererID() const {
	return mRendererID;
}

const vector<Reference<VertexBuffer>> &GLVertexArray::GetVertexBuffers() const {
	return mVertexBuffers;
}

const Reference<IndexBuffer> &GLVertexArray::GetIndexBuffer() const {
	return mIndexBuffer;
}


// Mutators
void  GLVertexArray::AddVertexBuffer(const Reference<VertexBuffer> &vertexBuffer) {
	APP_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Error their is no layout specified for the vertex buffer!");
	
	Bind();
	vertexBuffer->Bind();

	const auto &layout = vertexBuffer->GetLayout();
	for (const auto &element : layout) {
		glEnableVertexAttribArray(mVertexBufferIndex);

		auto baseType = ShaderDataTypeToOpenGLBaseType(element.Type);
		switch (baseType) {
			case GL_INT: {
				glVertexAttribIPointer(
					mVertexBufferIndex,
					element.GetComponentCount(),
					baseType,
					layout.GetStride(),
					(const void *)(uint64_t)element.Offset
				);
			}

			default: {
				glVertexAttribPointer(
					mVertexBufferIndex,
					element.GetComponentCount(),
					baseType,
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void *)(uint64_t)element.Offset
				);
			}
		}
		
		mVertexBufferIndex++;
	}

	mVertexBuffers.push_back(vertexBuffer);
}

void GLVertexArray::SetIndexBuffer(const Reference<IndexBuffer> &indexBuffer) {
	Bind();
	indexBuffer->Bind();

	mIndexBuffer = indexBuffer;
}

}

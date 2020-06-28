#pragma once

#include "Ultra.pch"
#include "Core.h"

#include "Buffer.h"

namespace Ultra {

class VertexArray {
public:
	virtual ~VertexArray() {}

	static Reference<VertexArray> Create();

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) = 0;
	virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) = 0;

	virtual const std::vector<std::shared_ptr<VertexBuffer>> &GetVertexBuffers() const = 0;
	virtual const std::shared_ptr<IndexBuffer> &GetIndexBuffer() const = 0;
};

}

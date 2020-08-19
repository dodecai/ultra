#pragma once

#include "Buffer.h"

namespace Ultra {

class VertexArray {
public:
	// Default
	VertexArray() = default;
	virtual ~VertexArray() = default;

	static Reference<VertexArray> Create();

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	// Accessors
	virtual RendererID GetRendererID() const = 0;
	virtual const vector<Reference<VertexBuffer>> &GetVertexBuffers() const = 0;
	virtual const Reference<IndexBuffer> &GetIndexBuffer() const = 0;

	// Mutators
	virtual void AddVertexBuffer(const Reference<VertexBuffer> &vertexBuffer) = 0;
	virtual void SetIndexBuffer(const Reference<IndexBuffer> &indexBuffer) = 0;
};

}

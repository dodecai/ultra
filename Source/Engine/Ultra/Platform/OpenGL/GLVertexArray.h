#pragma once

#include "Ultra/Renderer/VertexArray.h"

namespace Ultra {

class GLVertexArray: public VertexArray {
	// Properties
	RendererID mRendererID;
	uint32_t mVertexBufferIndex = 0;
	vector<std::shared_ptr<VertexBuffer>> mVertexBuffers;
	shared_ptr<IndexBuffer> mIndexBuffer;

public:
	// Default
	GLVertexArray();
	virtual ~GLVertexArray();

	virtual void Bind() const override;
	virtual void Unbind() const override;

	// Accessors
	virtual RendererID GetRendererID() const override;
	virtual const vector<Reference<VertexBuffer>> &GetVertexBuffers() const override;
	virtual const Reference<IndexBuffer> &GetIndexBuffer() const override;

	// Mutators
	virtual void AddVertexBuffer(const Reference<VertexBuffer> &vertexBuffer) override;
	virtual void SetIndexBuffer(const Reference<IndexBuffer> &indexBuffer) override;
};

}

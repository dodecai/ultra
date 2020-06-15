#pragma once

#include "Renderer/Buffer.h"

namespace Ultra {

class GLVertexBuffer: public VertexBuffer {
	uint32_t RendererID;
	BufferLayout mLayout;

public:
	GLVertexBuffer(float *vertices, uint32_t size);
	virtual ~GLVertexBuffer();

	virtual const BufferLayout &GetLayout() const override;
	virtual void SetLayout(const BufferLayout &layout) override;

	virtual void Bind() const override;
	virtual void Unbind() const override;
};

class GLIndexBuffer: public IndexBuffer {
	uint32_t RendererID;
	uint32_t mCount;

public:
	GLIndexBuffer(uint32_t *indices, uint32_t size);
	virtual ~GLIndexBuffer();

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual uint32_t GetCount() const { return mCount; };
};

}

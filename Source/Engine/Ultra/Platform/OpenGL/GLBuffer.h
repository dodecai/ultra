#pragma once

#include "Ultra/Renderer/Buffer.h"

namespace Ultra {

class GLVertexBuffer: public VertexBuffer {
public:
	GLVertexBuffer(uint32_t size);
	GLVertexBuffer(float *vertices, uint32_t size);
	virtual ~GLVertexBuffer();

	virtual const BufferLayout &GetLayout() const override;

	virtual void SetLayout(const BufferLayout &layout) override;
	virtual void SetData(const void *data, uint32_t size) override;

	virtual void Bind() const override;
	virtual void Unbind() const override;

private:
    uint32_t RendererID;
    BufferLayout mLayout;
};

class GLIndexBuffer: public IndexBuffer {
public:
	GLIndexBuffer(uint32_t *indices, uint32_t size);
	virtual ~GLIndexBuffer();

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual uint32_t GetCount() const { return mCount; };
    virtual uint32_t GetSize() const { return mSize; };

private:
    uint32_t RendererID;
    uint32_t mCount;
    uint32_t mSize;
};

}

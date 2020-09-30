#pragma once

#include "Ultra/Renderer/Buffer.h"

namespace Ultra {

class GLVertexBuffer: public VertexBuffer {
public:
	GLVertexBuffer(uint32_t size, VertexBufferType type = VertexBufferType::Dynamic);
	GLVertexBuffer(void *data, uint32_t size, VertexBufferType type = VertexBufferType::Static);
	virtual ~GLVertexBuffer();

    virtual uint32_t GetRendererID() const override { return RendererID; }
	virtual const BufferLayout &GetLayout() const override;
    virtual uint32_t GetSize() const override { return mSize; };

	virtual void SetLayout(const BufferLayout &layout) override;
	virtual void SetData(void *data, uint32_t size, uint32_t offset = 0) override;

	virtual void Bind() const override;
	virtual void Unbind() const override;

private:
    uint32_t RendererID = 0;
    BufferLayout mLayout;
    uint32_t mSize;
    VertexBufferType mType;

    DataBuffer mBuffer;
};

class GLIndexBuffer: public IndexBuffer {
public:
    GLIndexBuffer(uint32_t size);
	GLIndexBuffer(void *data, uint32_t size);
	virtual ~GLIndexBuffer();

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual uint32_t GetCount() const override { return mSize / sizeof(uint32_t); };
    virtual uint32_t GetSize() const override { return mSize; };
    virtual uint32_t GetRendererID() const override { return RendererID; }

    virtual void SetData(void *data, uint32_t size, uint32_t offset = 0);

private:
    uint32_t RendererID = 0;
    uint32_t mSize;

    DataBuffer mBuffer;
};

}

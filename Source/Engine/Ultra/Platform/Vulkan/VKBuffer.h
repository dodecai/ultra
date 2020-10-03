#pragma once

#include <vulkan/vulkan.hpp>
#include "Ultra/Renderer/Buffer.h"

namespace Ultra {

class VKTest;

struct StagingBuffer {
    vk::Buffer Buffer;
    vk::DeviceMemory Memory;
};

class VKVertexBuffer: public VertexBuffer {
public:
    VKVertexBuffer(uint32_t size, VertexBufferType type = VertexBufferType::Dynamic);
    VKVertexBuffer(void *data, uint32_t size, VertexBufferType type = VertexBufferType::Static);
    virtual ~VKVertexBuffer();

    virtual uint32_t GetRendererID() const override;
    virtual const BufferLayout &GetLayout() const override;
    virtual uint32_t GetSize() const override;

    virtual void SetLayout(const BufferLayout &layout) override;
    virtual void SetData(void *data, uint32_t size, uint32_t offset = 0) override;

    virtual void Bind() const override;
    virtual void Unbind() const override;

    vk::Buffer GetNativeBuffer() { return Vertices.Buffer; }
    vk::Buffer GetNativeBufferT() { return VerticesT.Buffer; }

private:
    uint32_t RendererID = 0;
    BufferLayout mLayout;
    uint32_t mSize;
    VertexBufferType mType;

    DataBuffer mBuffer;
    StagingBuffer Vertices;
    StagingBuffer VerticesT;
};

class VKIndexBuffer: public IndexBuffer {
public:
    VKIndexBuffer(uint32_t size);
    VKIndexBuffer(void *data, uint32_t size);
    virtual ~VKIndexBuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual uint32_t GetCount() const override { return mSize / sizeof(uint32_t); };
    virtual uint32_t GetSize() const override { return mSize; };
    virtual uint32_t GetRendererID() const override { return RendererID; }

    virtual void SetData(void *data, uint32_t size, uint32_t offset = 0);

    vk::Buffer GetNativeBuffer() { return Indices.Buffer; }
    vk::Buffer GetNativeBufferT() { return IndicesT.Buffer; }

private:
    uint32_t RendererID = 0;
    uint32_t mSize;

    DataBuffer mBuffer;
    StagingBuffer Indices;
    StagingBuffer IndicesT;
};

}

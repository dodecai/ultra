﻿export module Ultra.Renderer.Buffer;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.Engine.RendererData;

export namespace Ultra {

enum class BufferType {
    Vertex,
    Index,
    Staging,
    Uniform,
    Storage,
};

enum class BufferUsage {
    Dynamic,
    Static,
    Stream,
};

/// 
/// @brief Agnostic Buffer
///
/// @subsection  auto vertexBuffer = Buffer::Create({ BufferType::Vertex, sizeof(vertices), vertices });
/// @subsection  auto indexBuffer = Buffer::Create({ BufferType::Index, sizeof(indices), indices });
///
class Buffer {
protected:
    Buffer(BufferType type, const void *data, size_t size, BufferUsage usage): mType(type), mData(data), mSize(size), mUsage(usage) {}

public:
    Buffer() = default;
    virtual ~Buffer() = default;

    static Scope<Buffer> Create(BufferType type, const void *data, size_t size, BufferUsage usage = BufferUsage::Static);

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void UpdateData(const void *data, size_t size) = 0;

protected:
    RendererID mBufferID;
    const void *mData;
    size_t mSize;
    BufferType mType;
    BufferUsage mUsage;
};

}

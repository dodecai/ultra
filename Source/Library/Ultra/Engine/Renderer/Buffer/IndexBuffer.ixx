export module Ultra.Engine.IndexBuffer;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.Engine.BufferData;
export import Ultra.Engine.RendererData;

export namespace Ultra {

class IndexBuffer {
public:
    IndexBuffer() = default;
    virtual ~IndexBuffer() = default;

    static Reference<IndexBuffer> Create(const void *data, size_t size);
    static Reference<IndexBuffer> Create(size_t size);

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual const RendererID GetRendererID() const = 0;
    virtual const size_t GetCount() const = 0;
    virtual const size_t GetSize() const = 0;

    virtual void SetData(const void *data, size_t size, size_t offset = 0) = 0;
};

}

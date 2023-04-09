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

    static Reference<IndexBuffer> Create(uint32_t size);
    static Reference<IndexBuffer> Create(void *data, uint32_t size);

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual const RendererID GetRendererID() const = 0;
    virtual const uint32_t GetCount() const = 0;
    virtual const uint32_t GetSize() const = 0;

    virtual void SetData(void *data, uint32_t size, uint32_t offset = 0) = 0;
};

}

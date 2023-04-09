export module Ultra.Engine.UniformBuffer;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.Engine.BufferData;
export import Ultra.Engine.RendererData;

export namespace Ultra {

class UniformBuffer {
public:
    UniformBuffer() = default;
    ~UniformBuffer() = default;

    static Reference<UniformBuffer> Create(size_t size, uint32_t binding);

    virtual const RendererID GetRendererID() const = 0;
    virtual const uint32_t GetBinding() const = 0;

    virtual void SetData(const void *data, size_t size, size_t offset = 0) = 0;
};

}

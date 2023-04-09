export module Ultra.Engine.VertexBuffer;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.Engine.BufferData;
export import Ultra.Engine.RendererData;
export import Ultra.Engine.ShaderData;

export namespace Ultra {

struct VertexBufferElement {
    VertexBufferElement() = default;
    VertexBufferElement(ShaderDataType type, const string &name, bool normalized = false):
        Name(name),
        Type(type),
        Size(ShaderDataTypeSize(type)),
        Offset(0),
        Normalized(normalized) {
    }

    uint32_t GetComponentCount() const {
        switch (Type) {
            case ShaderDataType::Bool:	    return 1;
            case ShaderDataType::Bool2:	    return 2;
            case ShaderDataType::Bool3:	    return 3;
            case ShaderDataType::Bool4:	    return 4;
            case ShaderDataType::Float:	    return 1;
            case ShaderDataType::Float2:    return 2;
            case ShaderDataType::Float3:    return 3;
            case ShaderDataType::Float4:    return 4;
            case ShaderDataType::Int:	    return 1;
            case ShaderDataType::Int2:	    return 2;
            case ShaderDataType::Int3:	    return 3;
            case ShaderDataType::Int4:	    return 4;
            case ShaderDataType::Mat2:	    return 3;
            case ShaderDataType::Mat3:	    return 3;
            case ShaderDataType::Mat4:	    return 4;
            default:					    return 0;
        }
    }

    string Name;
    ShaderDataType Type;
    uint32_t Size;
    uint32_t Offset;
    bool Normalized;
};

class VertexBufferLayout {
public:
    VertexBufferLayout() {}
    VertexBufferLayout(const std::initializer_list<VertexBufferElement> &elements): mElements { elements } {
        CalculateOffsetAndStride();
    }

    inline const std::vector<VertexBufferElement> &GetElements() const { return mElements; }
    inline const uint32_t GetStride() const { return mStride; }

    [[nodiscard]] auto begin() { return mElements.begin(); }
    [[nodiscard]] auto end() { return mElements.end(); }
    [[nodiscard]] auto begin() const { return mElements.begin(); }
    [[nodiscard]] auto end() const { return mElements.end(); }

private:
    void CalculateOffsetAndStride() {
        mStride = 0;
        uint32_t offset = 0;
        for (auto &element : mElements) {
            element.Offset = offset;
            offset += element.Size;
            mStride += element.Size;
        }
    }

    vector<VertexBufferElement> mElements;
    uint32_t mStride = 0;
};

enum class VertexBufferType {
	Null	= 0x0,
	Static	= 0x1,
	Dynamic = 0x2,
};

class VertexBuffer {
public:
    VertexBuffer() = default;
    virtual ~VertexBuffer() = default;

    static Reference<VertexBuffer> Create(const void *data, size_t size, VertexBufferType type = VertexBufferType::Static);
    static Reference<VertexBuffer> Create(size_t size, VertexBufferType type = VertexBufferType::Dynamic);

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual const RendererID GetRendererID() const = 0;
    virtual const size_t GetSize() const = 0;

    virtual void SetData(void *data, size_t size, size_t offset = 0) = 0;
};

}

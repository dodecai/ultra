export module Ultra.Engine.IndexBuffer;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.Engine.BufferData;
export import Ultra.Engine.RendererData;

export namespace Ultra {

template<typename T>
concept index_t = std::same_as<T, uint8_t> || std::same_as<T, uint16_t> || std::same_as<T, uint32_t>;

enum class IndexType: uint8_t {
    Null    = 0,
    UINT8   = sizeof(uint8_t),
    UINT16  = sizeof(uint16_t),
    UINT32  = sizeof(uint32_t)
};

struct IndexProperties {
    uint32_t Count = {};
    IndexType Type = IndexType::UINT32;
};

///
/// @brief IndexBuffer
///
class IndexBuffer {
public:
    IndexBuffer(): mRendererID(), mProperties(), mData() {}
    virtual ~IndexBuffer() { mData.Release(); }

    static Reference<IndexBuffer> Create(size_t size);
    static Reference<IndexBuffer> Create(const uint32_t *data, size_t size);

    // Controls
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    const RendererID GetRendererID() const { return mRendererID; }
    const IndexProperties &GetProperties() const { return mProperties; }

    virtual void SetData(const uint32_t *data, size_t size, intptr_t offset = 0) = 0;

protected:
    template <index_t T>
    T *CompressIndices(const uint32_t *data, size_t &size);
    void DetectCompressionLevel(const uint32_t *data, size_t size);

protected:
    RendererID mRendererID;
    IndexProperties mProperties;
    BufferData mData;
};

template <index_t T>
T *IndexBuffer::CompressIndices(const uint32_t *data, size_t &size) {
    vector<uint32_t> indices {data, data + size};
    T *result = new T[indices.size()];
    for (size_t i = 0; i < indices.size(); i++) {
        result[i] = static_cast<T>(indices[i]);
    }
    size = sizeof(result);
    return result;
}

}

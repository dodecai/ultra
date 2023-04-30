module Ultra.Engine.Renderer.Buffer;

import Ultra.GFX.Context;
import Ultra.Platform.Renderer.DXBuffer;
import Ultra.Platform.Renderer.GLBuffer;
import Ultra.Platform.Renderer.VKBuffer;

namespace Ultra {

Scope<Buffer> Buffer::Create(BufferType type, const void *data, size_t size, BufferUsage usage) {
    switch (Context::API) {
        case GraphicsAPI::DirectX:  { return CreateScope<DXBuffer>(type, data, size, usage); }
        case GraphicsAPI::OpenGL:   { return CreateScope<GLBuffer>(type, data, size, usage); }
        case GraphicsAPI::Vulkan:   { return CreateScope<VKBuffer>(type, data, size, usage); }

        default: {
        #if APP_MODE_DEBUG
            throw std::runtime_error("Renderer::Buffer: RenderAPI not supported!");
        #else
            LogFatal("Renderer::Buffer: RenderAPI not supported!");
        #endif
            return nullptr;
        }
    }
}

#if UNDEFINED

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

void IndexBuffer::DetectCompressionLevel(const uint32_t *indices, size_t size) {
    auto maxValue = *std::max_element(indices, indices + size / sizeof(uint32_t));

    if (maxValue <= std::numeric_limits<uint8_t>::max()) {
        mProperties.Type = IndexType::UINT8;
    } else if (maxValue <= std::numeric_limits<uint16_t>::max()) {
        mProperties.Type = IndexType::UINT16;
    } else {
        mProperties.Type = IndexType::UINT32;
    }
}

struct BufferData {
    BufferData(): Data(nullptr), Size(0) {}
    BufferData(size_t size) { Allocate(size); }
    BufferData(byte *data, size_t size): Data((byte *)data), Size(size) {}

    static BufferData Copy(const BufferData &other) {
        BufferData buffer;
        buffer.Allocate(other.Size);
        memcpy(buffer.Data, other.Data, other.Size);
        return buffer;
    }
    static BufferData Copy(const void *data, size_t size) {
        BufferData buffer;
        buffer.Allocate(size);
        memcpy(buffer.Data, data, size);
        bool result = memcmp(data, buffer.Data, sizeof(data)) == 0 ? true : false;
        return buffer;
    }

    void Allocate(size_t size) {
        delete[] Data;
        Data = nullptr;
        if (size == 0) return;

        Data = new byte[size];
        Size = size;
    }
    void Initialize() {
        if (Data) memset(Data, 0, Size);
    }
    void Release() {
        delete[] Data;
        Data = nullptr;
        Size = 0;
    }

    template<typename T>
    T *As() const { return (T *)(Data); }
    template<typename T>
    T &Read(size_t offset = 0) {
        return *(T *)(Data + offset);
    }
    void Write(const void *data, size_t size, size_t offset = 0) {
        AppAssert(offset + size <= Size, "Renderer::Buffer: Buffer overflow detected!");
        memcpy(Data + offset, data, size);
    }

    size_t GetSize() const { return Size; }

    operator bool() const {
        return Data;
    }
    byte &operator[](int index) {
        return Data[index];
    }
    byte operator[](int index) const {
        return Data[index];
    }

    byte *Data;
    size_t Size;
};

#endif

}

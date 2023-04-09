export module Ultra.Engine.BufferData;

import Ultra.Core;

export namespace Ultra {

class BufferData {
public:
    BufferData(): Data { nullptr }, Size { 0 } {}
    BufferData(std::byte *data, size_t size): Data { data }, Size { size } {}

    static BufferData Copy(void *data, size_t size) {
        BufferData buffer;
        buffer.Allocate(size);
        memcpy(buffer.Data, data, size);
        return buffer;
    }

    void Allocate(size_t size) {
        delete[] Data;
        Data = nullptr;
        if (size == 0) return;

        Data = new std::byte[size];
        Size = size;
    }
    void Initialize() {
        if (Data) memset(Data, 0, Size);
    }

    template<typename T>
    T *As() { return dynamic_cast<T*>(Data); }
    template<typename T>
    T &Read(uint32_t offset = 0) {
        return *(T *)(Data + offset);
    }
    void Write(void *data, size_t size, size_t offset = 0) {
        memcpy(Data + offset, data, size);
    }

    size_t GetSize() const { return Size; }

    operator bool() const {
        return Data;
    }
    std::byte &operator[](int index) {
        return Data[index];
    }
    std::byte operator[](int index) const {
        return Data[index];
    }

    std::byte *Data;
    size_t Size;
};

}

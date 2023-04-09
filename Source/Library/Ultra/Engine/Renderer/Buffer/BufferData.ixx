export module Ultra.Engine.BufferData;

import Ultra.Core;

export namespace Ultra {

class BufferData {
public:
    BufferData(): Data(nullptr), Size(0) {}
    BufferData(size_t size) { Allocate(size); }
    BufferData(std::byte *data, size_t size): Data { data }, Size { size } {}
    ~BufferData() { Release(); }
    

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
    T *As() const { return (T*)(Data); }
    template<typename T>
    T &Read(uint32_t offset = 0) {
        return *(T *)(Data + offset);
    }
    void Write(const void *data, size_t size, size_t offset = 0) {
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

}

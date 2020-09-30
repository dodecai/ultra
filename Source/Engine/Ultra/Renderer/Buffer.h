#pragma once

#include "Ultra.pch"
#include "Ultra/Core.h"

#include "RendererData.h"

//#define __cpp_lib_byte
//#include <cstddef>
enum class byte : unsigned char {};

namespace Ultra {

enum class ShaderDataType: uint8_t {
	Null		= 0x00,

	Bool		= 0x01,

	Float		= 0x10,
	Float2		= 0x12,
	Float3		= 0x13,
	Float4		= 0x14,

	Int			= 0x20,
	Int2		= 0x22,
	Int3		= 0x23,
	Int4		= 0x24,

	Mat3		= 0x33,
	Mat4		= 0x34,
};

static uint32_t ShaderDatatTypeSize(ShaderDataType type) {
	switch (type) {
		case ShaderDataType::Bool:			return 1;
		case ShaderDataType::Float:			return 4;
		case ShaderDataType::Float2:		return 4 * 2;
		case ShaderDataType::Float3:		return 4 * 3;
		case ShaderDataType::Float4:		return 4 * 4;
		case ShaderDataType::Int:			return 4;
		case ShaderDataType::Int2:			return 4 * 2;
		case ShaderDataType::Int3:			return 4 * 3;
		case ShaderDataType::Int4:			return 4 * 4;
		case ShaderDataType::Mat3:			return 4 * 3 * 3;
		case ShaderDataType::Mat4:			return 4 * 4 * 4;
		default:							return 0;
	}
}

struct BufferElement {
	std::string Name;
	ShaderDataType Type;
	uint32_t Offset;
	uint32_t Size;
	bool Normalized;

	BufferElement() = default;

	BufferElement(ShaderDataType type, const std::string &name, bool normalized = false):
		Name { name },
		Type { type },
		Size { ShaderDatatTypeSize(type) },
		Offset { 0 },
		Normalized { normalized } {
	}

	uint32_t GetComponentCount() const {
		switch (Type) {
			case ShaderDataType::Bool:			return 1;
			case ShaderDataType::Float:			return 1;
			case ShaderDataType::Float2:		return 2;
			case ShaderDataType::Float3:		return 3;
			case ShaderDataType::Float4:		return 4;
			case ShaderDataType::Int:			return 1;
			case ShaderDataType::Int2:			return 2;
			case ShaderDataType::Int3:			return 3;
			case ShaderDataType::Int4:			return 4;
			case ShaderDataType::Mat3:			return 3;
			case ShaderDataType::Mat4:			return 4;
			default:							return 0;
		}
	}
};

class BufferLayout {
	std::vector<BufferElement> mElements;
	uint32_t mStride = 0;

public:
	BufferLayout() {}
	BufferLayout(const std::initializer_list<BufferElement> &elements):
		mElements{ elements } {
		CalculateOffsetAndStride();
	}

	inline const std::vector<BufferElement> &GetElements() const { return mElements; }
	inline const uint32_t GetStride() const { return mStride; }

	std::vector<BufferElement>::iterator begin() { return mElements.begin(); }
	std::vector<BufferElement>::iterator end() { return mElements.end(); }
	std::vector<BufferElement>::const_iterator begin() const { return mElements.begin(); }
	std::vector<BufferElement>::const_iterator end() const { return mElements.end(); }
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
};

enum class VertexBufferType {
	Null	= 0,
	Static	= 1,
	Dynamic = 2,
};


struct DataBuffer {
    byte *Data;
    uint32_t Size;

    DataBuffer(): Data { nullptr }, Size { 0 } {}
    DataBuffer(byte *data, uint32_t size): Data { data }, Size { size } {}

    static DataBuffer Copy(void *data, uint32_t size) {
        DataBuffer buffer;
        buffer.Allocate(size);
        memcpy(buffer.Data, data, size);
        return buffer;
    }

    void Allocate(uint32_t size) {
        delete[] Data;
        Data = nullptr;
        if (size == 0) return;

        Data = new byte[size];
        Size = size;
    }
    void Initialize() {
        if (Data) memset(Data, 0, Size);
    }

    template<typename T>
    T *As() {
        return (T *)Data;
    }
    template<typename T>
    T &Read(uint32_t offset = 0) {
        return *(T *)(Data + offset);
    }
    void Write(void *data, uint32_t size, uint32_t offset = 0) {
        memcpy(Data + offset, data, size);
    }
    
    uint32_t GetSize() const { return Size; }

    operator bool() const {
        return Data;
    }
    byte &operator[](int index) {
        return Data[index];
    }
    byte operator[](int index) const {
        return Data[index];
    }
};


class VertexBuffer {
public:
	virtual ~VertexBuffer() {}

	static Reference<VertexBuffer> Create(uint32_t size, VertexBufferType type = VertexBufferType::Dynamic);
	static Reference<VertexBuffer> Create(void *data, uint32_t size, VertexBufferType type = VertexBufferType::Static);

    virtual uint32_t GetRendererID() const = 0;
	virtual const BufferLayout &GetLayout() const = 0;
    virtual uint32_t GetSize() const = 0;

    virtual void SetLayout(const BufferLayout &layout) = 0;
    virtual void SetData(void *data, uint32_t size, uint32_t offset = 0) = 0;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;
};


class IndexBuffer {
public:
	virtual ~IndexBuffer() {}

    static Reference<IndexBuffer> Create(uint32_t size);
	static Reference<IndexBuffer> Create(void *data, uint32_t size);

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual uint32_t GetCount() const = 0;
    virtual uint32_t GetSize() const = 0;
    virtual uint32_t GetRendererID() const = 0;

    virtual void SetData(void *data, uint32_t size, uint32_t offset = 0) = 0;
};

}

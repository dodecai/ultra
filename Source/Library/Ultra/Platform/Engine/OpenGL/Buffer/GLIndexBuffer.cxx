module;

#include <glad/gl.h>

module Ultra.Platform.Engine.GLIndexBuffer;

namespace Ultra {

GLIndexBuffer::GLIndexBuffer(size_t size) {
    mData.Allocate(size);
    glCreateBuffers(1, &mRendererID);
    glNamedBufferData(mRendererID, size, nullptr, GL_DYNAMIC_DRAW);
}

GLIndexBuffer::GLIndexBuffer(const uint32_t *data, size_t size) {
    DetectCompressionLevel(data, size);
    mProperties.Count = size / (size_t)mProperties.Type;

    void *optimizedData = nullptr;
    if (mProperties.Type == IndexType::UINT8) {
        optimizedData = reinterpret_cast<void *>(CompressIndices<uint8_t>(data, size));
        mData = BufferData::Copy(optimizedData, size);
    } else if (mProperties.Type == IndexType::UINT16) {
        optimizedData = reinterpret_cast<void *>(CompressIndices<uint16_t>(data, size));
        mData = BufferData::Copy(optimizedData, size);
    } else {
        optimizedData = (void *)data;
        mData = BufferData::Copy(optimizedData, size);
    }
    glCreateBuffers(1, &mRendererID);
    glNamedBufferData(mRendererID, mProperties.Count, optimizedData, GL_STATIC_DRAW);
}

GLIndexBuffer::~GLIndexBuffer() {
    glDeleteBuffers(1, &mRendererID);
}

void GLIndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
}

void GLIndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLIndexBuffer::SetData(const uint32_t *data, size_t size, intptr_t offset) {
    mData = BufferData::Copy(data, size);
    glNamedBufferSubData(mRendererID, offset, size, data);
}

}

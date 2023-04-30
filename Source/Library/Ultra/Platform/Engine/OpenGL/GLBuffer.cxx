module;

#include <glad/gl.h>

module Ultra.Platform.Renderer.GLBuffer;

namespace Ultra {

GLenum GetGLBufferType(BufferType type) {
    switch (type) {
        case BufferType::Vertex:    { return GL_ARRAY_BUFFER; }
        case BufferType::Index:     { return GL_ELEMENT_ARRAY_BUFFER; }
        case BufferType::Uniform:   { return GL_UNIFORM_BUFFER; }
        case BufferType::Staging:   { return GL_COPY_READ_BUFFER; }
        case BufferType::Storage:   { return GL_SHADER_STORAGE_BUFFER; }
    }
}

GLenum GetGLBufferUsage(BufferUsage usage) {
    switch (usage) {
        case BufferUsage::Dynamic:  { return GL_DYNAMIC_DRAW; }
        case BufferUsage::Static:   { return GL_STATIC_DRAW; }
        case BufferUsage::Stream:   { return GL_STREAM_DRAW; }
    }
}

GLBuffer::GLBuffer(BufferType type, const void *data, size_t size, BufferUsage usage): Buffer(type, data, size, usage) {
    auto bufferUsage = GetGLBufferUsage(usage);
    mNativeType = GetGLBufferType(type);

    glCreateBuffers(1, &mBufferID);
    glNamedBufferData(mBufferID, size, data, bufferUsage);
}

GLBuffer::~GLBuffer() {
    glDeleteBuffers(1, &mBufferID);
}


void GLBuffer::Bind() const {
    glBindBuffer(mNativeType, mBufferID);
}

void GLBuffer::Unbind() const {
    glBindBuffer(mNativeType, 0);
}

void GLBuffer::UpdateData(const void *data, size_t size) {
    glNamedBufferSubData(mBufferID, 0, size, data);
}

#if UNDEFINED

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
#endif


}

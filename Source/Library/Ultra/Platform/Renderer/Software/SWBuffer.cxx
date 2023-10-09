module Ultra.Platform.Renderer.SWBuffer;

#pragma warning(push)
#pragma warning(disable: 4100)

namespace Ultra {

SWBuffer::SWBuffer(BufferType type, const void *data, size_t size, BufferUsage usage): Buffer(type, data, size, usage) {}

SWBuffer::~SWBuffer() {}


void SWBuffer::Bind() const {}

void SWBuffer::Unbind() const {}

void SWBuffer::UpdateData(const void *data, size_t size) {}

}

#pragma warning(pop)

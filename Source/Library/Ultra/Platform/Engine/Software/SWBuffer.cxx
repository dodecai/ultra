module Ultra.Platform.Renderer.SWBuffer;

namespace Ultra {

SWBuffer::SWBuffer(BufferType type, const void *data, size_t size, BufferUsage usage): Buffer(type, data, size, usage) {}

SWBuffer::~SWBuffer() {}


void SWBuffer::Bind() const {}

void SWBuffer::Unbind() const {}

void SWBuffer::UpdateData(const void *data, size_t size) {}

}

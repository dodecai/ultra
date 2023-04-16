module Ultra.Platform.Renderer.VKBuffer;

namespace Ultra {

VKBuffer::VKBuffer(BufferType type, const void *data, size_t size, BufferUsage usage): Buffer(type, data, size, usage) {}

VKBuffer::~VKBuffer() {}


void VKBuffer::Bind() const {}

void VKBuffer::Unbind() const {}

void VKBuffer::UpdateData(const void *data, size_t size) {}

}

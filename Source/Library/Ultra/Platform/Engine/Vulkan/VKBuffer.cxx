module Ultra.Platform.Renderer.VKBuffer;

namespace Ultra {

VKBuffer::VKBuffer(BufferType type, const void *data, size_t size): Buffer(type, data, size) {}

VKBuffer::~VKBuffer() {}


void VKBuffer::Bind() const {}

void VKBuffer::Unbind() const {}

void VKBuffer::UpdateData(const void *data, size_t size) {}

}

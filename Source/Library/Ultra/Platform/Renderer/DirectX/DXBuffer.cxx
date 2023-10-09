module Ultra.Platform.Renderer.DXBuffer;

#pragma warning(push)
#pragma warning(disable: 4100)

namespace Ultra {

DXBuffer::DXBuffer(BufferType type, const void *data, size_t size, BufferUsage usage): Buffer(type, data, size, usage) {}

DXBuffer::~DXBuffer() {}


void DXBuffer::Bind() const {}

void DXBuffer::Unbind() const {}

void DXBuffer::UpdateData(const void *data, size_t size) {}

}

#pragma warning(pop)

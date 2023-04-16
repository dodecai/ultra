module Ultra.Platform.Renderer.DXBuffer;

namespace Ultra {

DXBuffer::DXBuffer(BufferType type, const void *data, size_t size): Buffer(type, data, size) {}

DXBuffer::~DXBuffer() {}


void DXBuffer::Bind() const {}

void DXBuffer::Unbind() const {}

void DXBuffer::UpdateData(const void *data, size_t size) {}

}

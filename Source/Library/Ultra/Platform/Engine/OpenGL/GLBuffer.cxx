module Ultra.Platform.Renderer.GLBuffer;

namespace Ultra {

GLBuffer::GLBuffer(BufferType type, const void *data, size_t size): Buffer(type, data, size) {}

GLBuffer::~GLBuffer() {}


void GLBuffer::Bind() const {}

void GLBuffer::Unbind() const {}

void GLBuffer::UpdateData(const void *data, size_t size) {}

}

module;

#include <glad/gl.h>

export module Ultra.Platform.Renderer.GLBuffer;

import Ultra.Renderer.Buffer;

export namespace Ultra {

class GLBuffer: public Buffer {
public:
    GLBuffer(BufferType type, const void *data, size_t size, BufferUsage usage);
    virtual ~GLBuffer() override;

    virtual void Bind() const override;
    virtual void Bind(uint32_t binding) const override;
    virtual void Unbind() const override;
    virtual void UpdateData(const void *data, size_t size) override;

private:
    GLenum mNativeType;
};

}

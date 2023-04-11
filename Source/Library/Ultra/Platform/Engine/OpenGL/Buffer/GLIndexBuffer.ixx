export module Ultra.Platform.Engine.GLIndexBuffer;

import Ultra.Platform.GFX.OpenGL;
import Ultra.Engine.IndexBuffer;

export namespace Ultra {

class GLIndexBuffer: public IndexBuffer {
public:
    GLIndexBuffer() = default;
    GLIndexBuffer(size_t size);
    GLIndexBuffer(const uint32_t *data, size_t size);
    virtual ~GLIndexBuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void SetData(const uint32_t *data, size_t size, intptr_t offset = 0);
};

}

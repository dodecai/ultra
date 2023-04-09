module;

#include <glad/gl.h>

export module Ultra.Platform.Engine.GLVertexBuffer;

import Ultra.Engine.VertexBuffer;

export namespace Ultra {

class GLVertexBuffer: public VertexBuffer {
public:
    GLVertexBuffer(uint32_t size, VertexBufferType type = VertexBufferType::Dynamic);
    GLVertexBuffer(void *data, uint32_t size, VertexBufferType type = VertexBufferType::Static);
    virtual ~GLVertexBuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual const RendererID GetRendererID() const override { return static_cast<RendererID>(mRendererID); }
    virtual const VertexBufferLayout &GetLayout() const override;
    virtual const uint32_t GetSize() const override { return static_cast<uint32_t>(mSize); };

    virtual void SetLayout(const VertexBufferLayout &layout) override;
    virtual void SetData(void *data, uint32_t size, uint32_t offset = 0) override;

private:
    GLuint mRendererID = 0;
    VertexBufferType mType;
    VertexBufferLayout mLayout;
    BufferData mBuffer;
    GLsizei mSize;
};

}

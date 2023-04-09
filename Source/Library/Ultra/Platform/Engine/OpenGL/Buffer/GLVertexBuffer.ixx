module;

#include <glad/gl.h>

export module Ultra.Platform.Engine.GLVertexBuffer;

import Ultra.Engine.VertexBuffer;

export namespace Ultra {

class GLVertexBuffer: public VertexBuffer {
public:
    GLVertexBuffer() = default;
    GLVertexBuffer(const void *data, size_t size, VertexBufferType type = VertexBufferType::Static);
    GLVertexBuffer(size_t size, VertexBufferType type = VertexBufferType::Dynamic);
    virtual ~GLVertexBuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual const RendererID GetRendererID() const override { return mRendererID; }
    virtual const VertexBufferLayout &GetLayout() const override { return mLayout; }
    virtual const size_t GetSize() const override { return mBuffer.Size; };

    virtual void SetData(void *data, size_t size, size_t offset = 0) override;
    virtual void SetLayout(const VertexBufferLayout &layout) override { mLayout = layout; }

private:
    RendererID mRendererID = 0;
    VertexBufferType mType;
    VertexBufferLayout mLayout;
    BufferData mBuffer;
};

}

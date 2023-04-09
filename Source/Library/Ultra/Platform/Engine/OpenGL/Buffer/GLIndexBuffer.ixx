export module Ultra.Platform.Engine.GLIndexBuffer;

import Ultra.Platform.GFX.OpenGL;
import Ultra.Engine.IndexBuffer;

export namespace Ultra {

class GLIndexBuffer: public IndexBuffer {
public:
    GLIndexBuffer() = default;
    GLIndexBuffer(const void *data, size_t size);
    GLIndexBuffer(size_t size);
    virtual ~GLIndexBuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual const RendererID GetRendererID() const override { return mRendererID; }
    virtual const size_t GetCount() const override { return mBuffer.Size / sizeof(size_t); };
    virtual const size_t GetSize() const override { return mBuffer.Size; };

    virtual void SetData(const void *data, size_t size, size_t offset = 0);

private:
    RendererID mRendererID = 0;
    BufferData mBuffer;
};

}

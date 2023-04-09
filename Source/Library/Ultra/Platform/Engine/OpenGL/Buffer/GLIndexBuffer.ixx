export module Ultra.Platform.Engine.GLIndexBuffer;

import Ultra.Platform.GFX.OpenGL;
import Ultra.Engine.IndexBuffer;

export namespace Ultra {

class GLIndexBuffer: public IndexBuffer {
public:
    GLIndexBuffer(uint32_t size);
    GLIndexBuffer(void *data, uint32_t size);
    virtual ~GLIndexBuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual const RendererID GetRendererID() const override { return static_cast<RendererID>(mRendererID); }
    virtual const uint32_t GetCount() const override { return static_cast<uint32_t>(mSize) / sizeof(uint32_t); };
    virtual const uint32_t GetSize() const override { return static_cast<uint32_t>(mSize); };

    virtual void SetData(void *data, uint32_t size, uint32_t offset = 0);

private:
    GLuint mRendererID = 0;
    BufferData mBuffer;
    GLsizei mSize;
};

}

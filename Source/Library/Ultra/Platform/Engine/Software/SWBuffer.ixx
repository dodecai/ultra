export module Ultra.Platform.Renderer.SWBuffer;

import Ultra.Engine.Renderer.Buffer;

export namespace Ultra {

class SWBuffer: public Buffer {
public:
    SWBuffer(BufferType type, const void *data, size_t size, BufferUsage usage);
    virtual ~SWBuffer() override;

    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void UpdateData(const void *data, size_t size) override;

private:
};

}

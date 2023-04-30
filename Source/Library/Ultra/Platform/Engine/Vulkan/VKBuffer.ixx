export module Ultra.Platform.Renderer.VKBuffer;

import Ultra.Engine.Renderer.Buffer;

export namespace Ultra {

class VKBuffer: public Buffer {
public:
    VKBuffer(BufferType type, const void *data, size_t size, BufferUsage usage);
    virtual ~VKBuffer() override;

    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void UpdateData(const void *data, size_t size) override;

private:
};

}

export module Ultra.Platform.Renderer.DXBuffer;

import Ultra.Renderer.Buffer;

export namespace Ultra {

class DXBuffer: public Buffer {
public:
    DXBuffer(BufferType type, const void *data, size_t size, BufferUsage usage);
    virtual ~DXBuffer() override;

    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void UpdateData(const void *data, size_t size) override;

private:
};

}

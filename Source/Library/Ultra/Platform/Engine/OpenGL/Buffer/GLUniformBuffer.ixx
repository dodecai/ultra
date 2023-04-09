export module Ultra.Platform.Engine.GLUniformBuffer;

import Ultra.Platform.GFX.OpenGL;
import Ultra.Engine.UniformBuffer;

export namespace Ultra {

class GLUniformBuffer: public UniformBuffer {
public:
    GLUniformBuffer() = default;
    GLUniformBuffer(size_t size, uint32_t binding);
    virtual ~GLUniformBuffer();

    virtual const RendererID GetRendererID() const override { return mRendererID; }
    virtual const uint32_t GetBinding() const override { return mBinding; }
    virtual void SetData(const void *data, size_t size, size_t offset = 0) override;

private:
    RendererID mRendererID = 0;
    uint32_t mBinding = 0;
    BufferData mBuffer;
};

}

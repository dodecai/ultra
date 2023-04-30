export module Ultra.Platform.Renderer.VKCommandBuffer;

import Ultra.Engine.Renderer.CommandBuffer;

export namespace Ultra {

class VKCommandBuffer: public CommandBuffer {
public:
    VKCommandBuffer();
    virtual ~VKCommandBuffer();

    virtual void Begin() override;
    virtual void End() override;

    virtual void Clear(float r, float g, float b, float a) override;
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

    virtual void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) override;
    virtual void DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) override;
    virtual void DrawIndexed(size_t count, PrimitiveType type, bool depthTest = true) override;

    virtual void Execute() override;
};

}

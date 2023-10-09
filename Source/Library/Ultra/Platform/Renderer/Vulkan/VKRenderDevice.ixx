export module Ultra.Platform.Renderer.VKRenderDevice;

export import Ultra.Renderer.RenderDevice;

export namespace Ultra {

class VKRenderDevice: public RenderDevice {
public:
    VKRenderDevice();
    virtual ~VKRenderDevice();

    virtual void Load() override;
    virtual void BeginFrame() override;
    virtual void EndFrame() override;
    virtual void Dispose() override;

    virtual void SetLineThickness(float value) override;
    virtual void SetPolygonMode(PolygonMode mode) override;
};

}

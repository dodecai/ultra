export module Ultra.Platform.Renderer.DXRenderDevice;

export import Ultra.Engine.Renderer.RenderDevice;

export namespace Ultra {

class DXRenderDevice: public RenderDevice {
public:
    DXRenderDevice();
    virtual ~DXRenderDevice();

    virtual void Load() override;
    virtual void BeginFrame() override;
    virtual void EndFrame() override;
    virtual void Dispose() override;

    virtual void SetLineThickness(float value) override;
    virtual void SetPolygonMode(PolygonMode mode) override;
};

}

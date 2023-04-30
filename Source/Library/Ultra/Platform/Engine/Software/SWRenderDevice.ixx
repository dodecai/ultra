export module Ultra.Platform.Renderer.SWRenderDevice;

export import Ultra.Engine.Renderer.RenderDevice;

export namespace Ultra {

class SWRenderDevice: public RenderDevice {
public:
    SWRenderDevice();
    virtual ~SWRenderDevice();

    virtual void Load() override;
    virtual void BeginFrame() override;
    virtual void EndFrame() override;
    virtual void Dispose() override;

    virtual void SetLineThickness(float value) override;
    virtual void SetPolygonMode(PolygonMode mode) override;
};

}

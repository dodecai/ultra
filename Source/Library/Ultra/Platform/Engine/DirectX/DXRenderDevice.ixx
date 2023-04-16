export module Ultra.Platform.Renderer.DXRenderDevice;

export import Ultra.Renderer.RenderDevice;

export namespace Ultra {

class DXRenderDevice: public RenderDevice {
public:
    DXRenderDevice();
    virtual ~DXRenderDevice();

    virtual void Load() override;
    virtual void BeginFrame() override;
    virtual void EndFrame() override;
    virtual void Dispose() override;
};

}

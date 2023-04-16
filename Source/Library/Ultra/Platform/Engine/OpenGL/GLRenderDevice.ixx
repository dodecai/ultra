export module Ultra.Platform.Renderer.GLRenderDevice;

export import Ultra.Renderer.RenderDevice;

export namespace Ultra {

class GLRenderDevice: public RenderDevice {
public:
    GLRenderDevice();
    virtual ~GLRenderDevice();

    virtual void Load() override;
    virtual void BeginFrame() override;
    virtual void EndFrame() override;
    virtual void Dispose() override;
};

}

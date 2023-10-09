export module Ultra.Platform.Renderer.GLRenderDevice;

import Ultra.Renderer.RenderDevice;

export namespace Ultra {

class GLRenderDevice: public RenderDevice {
public:
    GLRenderDevice();
    virtual ~GLRenderDevice();

    virtual void Load() override;
    virtual void BeginFrame() override;
    virtual void EndFrame() override;
    virtual void Dispose() override;

    virtual void SetLineThickness(float value) override;
    virtual void SetPolygonMode(PolygonMode mode) override;
};

}

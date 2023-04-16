export module Ultra.Platform.Renderer.VKRenderDevice;

export import Ultra.Renderer.RenderDevice;

export namespace Ultra {

class VKRenderDevice: public RenderDevice {
public:
    VKRenderDevice();
    virtual ~VKRenderDevice();

    virtual Scope<Shader> CreateShader(ShaderType type, const string &source, const string &entryPoint) override;

    virtual void Load() override;
    virtual void BeginFrame() override;
    virtual void EndFrame() override;
    virtual void Dispose() override;
};

}

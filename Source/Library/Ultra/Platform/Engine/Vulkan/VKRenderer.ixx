export module Ultra.Platform.VKRenderer;

import Ultra.Renderer;

export namespace Ultra {

class VKRenderer: public Renderer {
public:
    VKRenderer();
    virtual ~VKRenderer();

    virtual void Load() override;
    virtual void LoadShader(ShaderType type, const string &source, const string &entryPoint) override;
    virtual void RenderFrame() override;
    virtual void Dispose() override;
};

}

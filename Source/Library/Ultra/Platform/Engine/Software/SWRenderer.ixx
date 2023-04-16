export module Ultra.Platform.SWRenderer;

import Ultra.Renderer;

export namespace Ultra {

class SWRenderer: public Renderer {
public:
    SWRenderer();
    virtual ~SWRenderer();

    virtual void Load() override;
    virtual void LoadShader(ShaderType type, const string &source, const string &entryPoint) override;
    virtual void RenderFrame() override;
    virtual void Dispose() override;
};

}

export module Ultra.Platform.DXRenderer;

import Ultra.Renderer;

export namespace Ultra {

class DXRenderer: public Renderer {
public:
    DXRenderer();
    virtual ~DXRenderer();

    virtual void Load() override;
    virtual void LoadShader(ShaderType type, const string &source, const string &entryPoint) override;
    virtual void RenderFrame() override;
    virtual void Dispose() override;
};

}

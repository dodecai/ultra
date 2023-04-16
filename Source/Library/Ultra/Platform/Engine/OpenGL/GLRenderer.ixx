export module Ultra.Platform.GLRenderer;

import Ultra.Renderer;

export namespace Ultra {

class GLRenderer: public Renderer {
public:
    GLRenderer();
    virtual ~GLRenderer();

    virtual void Load() override;
    virtual void LoadShader(ShaderType type, const string &source, const string &entryPoint) override;
    virtual void RenderFrame() override;
    virtual void Dispose() override;
};

}

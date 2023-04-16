export module Ultra.Platform.Renderer.DXShader;

import Ultra.Renderer.Shader;

export namespace Ultra {

class DXShader: public Shader {
public:
    DXShader(ShaderType type, const string &source, const string &entryPoint);
    virtual ~DXShader();

    virtual void Compile() override;
    virtual void Bind() const override;
    virtual void Unbind() const override;
private:
};

}

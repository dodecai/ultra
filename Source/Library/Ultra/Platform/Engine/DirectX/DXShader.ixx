export module Ultra.Platform.Renderer.DXShader;

import Ultra.Renderer.Shader;

export namespace Ultra {

class DXShader: public Shader {
public:
    DXShader(const string &source, const string &entryPoint, const ShaderType type);
    virtual ~DXShader();

    virtual void Compile(ShaderList shaders) override;
    virtual void Bind() const override;
    virtual void Unbind() const override;
private:
};

}

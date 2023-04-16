export module Ultra.Platform.Renderer.SWShader;

import Ultra.Renderer.Shader;

export namespace Ultra {

class SWShader: public Shader {
public:
    SWShader(const string &source, const string &entryPoint, const ShaderType type);
    virtual ~SWShader();

    virtual void Compile(ShaderList shaders) override;
    virtual void Bind() const override;
    virtual void Unbind() const override;
private:
};

}

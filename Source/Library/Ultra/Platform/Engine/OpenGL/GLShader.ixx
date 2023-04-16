export module Ultra.Platform.Renderer.GLShader;

import Ultra.Renderer.Shader;

export namespace Ultra {

class GLShader: public Shader {
public:
    GLShader(ShaderType type, const string &source, const string &entryPoint);
    virtual ~GLShader();

    virtual void Compile() override;
    virtual void Bind() const override;
    virtual void Unbind() const override;
private:
};

}

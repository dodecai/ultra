export module Ultra.Platform.Renderer.GLShader;

import Ultra.Renderer.Shader;

export namespace Ultra {

class GLShader: public Shader {
public:
    GLShader(const string &source, const string &entryPoint, const ShaderType type);
    virtual ~GLShader();

    virtual void Compile(ShaderList shaders) override;
    virtual void Bind() const override;
    virtual void Unbind() const override;
private:
};

}

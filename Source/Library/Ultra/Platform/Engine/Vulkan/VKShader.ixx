export module Ultra.Platform.Renderer.VKShader;

import Ultra.Renderer.Shader;

export namespace Ultra {

class VKShader: public Shader {
public:
    VKShader(ShaderType type, const string &source, const string &entryPoint);
    virtual ~VKShader();

    virtual void Compile() override;
    virtual void Bind() const override;
    virtual void Unbind() const override;
private:
};

}

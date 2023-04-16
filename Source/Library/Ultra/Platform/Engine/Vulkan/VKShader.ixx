export module Ultra.Platform.Renderer.VKShader;

import Ultra.Renderer.Shader;

export namespace Ultra {

class VKShader: public Shader {
public:
    VKShader(const string &source, const string &entryPoint, const ShaderType type);
    virtual ~VKShader();

    virtual void Compile(ShaderList shaders) override;
    virtual void Bind() const override;
    virtual void Unbind() const override;
private:
};

}

module Ultra.Platform.Renderer.SWShader;

namespace Ultra {

SWShader::SWShader(const string &source, const string &entryPoint, const ShaderType type): Shader(source, entryPoint, type) {}

SWShader::~SWShader() {}


void SWShader::Compile(ShaderList shaders) {}

void SWShader::Bind() const {}

void SWShader::Unbind() const {}

}

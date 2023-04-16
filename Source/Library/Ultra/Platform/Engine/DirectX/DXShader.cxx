module Ultra.Platform.Renderer.DXShader;

namespace Ultra {

DXShader::DXShader(const string &source, const string &entryPoint, const ShaderType type): Shader(source, entryPoint, type) {}

DXShader::~DXShader() {}


void DXShader::Compile(ShaderList shaders) {}

void DXShader::Bind() const {}

void DXShader::Unbind() const {}

}

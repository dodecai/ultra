module Ultra.Platform.Renderer.DXShader;

namespace Ultra {

DXShader::DXShader(ShaderType type, const string &source, const string &entryPoint): Shader(type, source, entryPoint) {}

DXShader::~DXShader() {}


void DXShader::Compile() {}

void DXShader::Bind() const {}

void DXShader::Unbind() const {}

}

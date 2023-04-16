module Ultra.Platform.Renderer.VKShader;

namespace Ultra {

VKShader::VKShader(ShaderType type, const string &source, const string &entryPoint): Shader(type, source, entryPoint) {}

VKShader::~VKShader() {}


void VKShader::Compile() {}

void VKShader::Bind() const {}

void VKShader::Unbind() const {}

}

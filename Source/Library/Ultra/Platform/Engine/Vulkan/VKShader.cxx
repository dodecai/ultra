module Ultra.Platform.Renderer.VKShader;

namespace Ultra {

VKShader::VKShader(const string &source, const string &entryPoint, const ShaderType type): Shader(source, entryPoint, type) {}

VKShader::~VKShader() {}


void VKShader::Compile(ShaderList shaders) {}

void VKShader::Bind() const {}

void VKShader::Unbind() const {}

}

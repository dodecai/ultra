module Ultra.Platform.Renderer.GLShader;

namespace Ultra {

GLShader::GLShader(ShaderType type, const string &source, const string &entryPoint): Shader(type, source, entryPoint) {}

GLShader::~GLShader() {}


void GLShader::Compile() {}

void GLShader::Bind() const {}

void GLShader::Unbind() const {}

}

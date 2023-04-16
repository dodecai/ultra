module;

#include <glad/gl.h>

module Ultra.Platform.Renderer.GLShader;

namespace Ultra {

/// Helpers
GLenum GetGLShaderType(ShaderType type) {
    switch (type) {
        case ShaderType::Linked:            { return GL_SHADER; }
        case ShaderType::Compute:           { return GL_COMPUTE_SHADER; }
        case ShaderType::Fragment:          { return GL_FRAGMENT_SHADER; }
        case ShaderType::Geometry:          { return GL_GEOMETRY_SHADER; }
        case ShaderType::TessControl:       { return GL_TESS_CONTROL_SHADER; }
        case ShaderType::TessEvaluation:    { return GL_TESS_EVALUATION_SHADER; }
        case ShaderType::Vertex:            { return GL_VERTEX_SHADER; }
    }
}


GLShader::GLShader(const string &source, const string &entryPoint, const ShaderType type): Shader(source, entryPoint, type) {
    auto shaderCode = source;
    auto shaders = SplitSources(shaderCode);
    Compile(shaders);
}

GLShader::~GLShader() {
    glDeleteProgram(mShaderID);
}


void GLShader::Compile(ShaderList shaders) {
    // Create the Program
    auto program = glCreateProgram();

    vector<GLenum> ids;
    ids.reserve(shaders.size());
    for (auto &&[key, code] : shaders) {
        // Create Shader
        auto type = GetGLShaderType((ShaderType)key);
        auto shader = glCreateShader(type);

        const GLchar *source = code.c_str();
        glShaderSource(shader, 1, &source, 0);
        
        // Compile Shader
        GLint compiled = 0;
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (compiled == GL_FALSE) {
            GLint length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

            vector<GLchar> message(length);
            glGetShaderInfoLog(shader, length, &length, message.data());

            glDeleteShader(shader);

            LogError("Couldn't compile shader:", string(message.begin(), message.end()));
            break;
        }

        // Attach Shader
        glAttachShader(program, shader);
        ids.push_back(shader);
    }

    // Link them to the Program
    GLint linked = 0;
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE) {
        GLint length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

        vector<GLchar> message(length);
        glGetProgramInfoLog(program, length, &length, message.data());
        LogError("Couldn't link shaders:", string(message.begin(), message.end()));

        // CleanUp
        glDeleteProgram(program);
        for (auto id : ids) {
            glDetachShader(program, id);
        }
        return;
    }

    // Detach all Shaders
    mShaderID = program;
    for (auto id : ids) {
        glDetachShader(program, id);
    }
}

void GLShader::Bind() const {
    glUseProgram(mShaderID);
}

void GLShader::Unbind() const {
    glUseProgram(0);
}

}

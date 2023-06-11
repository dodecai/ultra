module;

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    auto shaders = Convert(shaderCode);
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


int32_t GLShader::FindUniformLocation(const string &name) const {
    return glGetUniformLocation(mShaderID, name.c_str());
}


void GLShader::UpdateUniformBuffer(const string &name, const void *data, size_t size) {
    auto location = FindUniformLocation(name);
    glUniform1iv(location, static_cast<GLsizei>(size), reinterpret_cast<const GLint *>(data));
}

void GLShader::UpdateUniform(const string &name, const Bool &data) {
    auto location = FindUniformLocation(name);
    glUniform1i(location, data);
}

void GLShader::UpdateUniform(const string &name, const Bool2 &data) {
    auto location = FindUniformLocation(name);
    glUniform2iv(location, 1, reinterpret_cast<const GLint *>(glm::value_ptr(data)));
}

void GLShader::UpdateUniform(const string &name, const Bool3 &data) {
    auto location = FindUniformLocation(name);
    glUniform3iv(location, 1, reinterpret_cast<const GLint *>(glm::value_ptr(data)));
}

void GLShader::UpdateUniform(const string &name, const Bool4 &data) {
    auto location = FindUniformLocation(name);
    glUniform4iv(location, 1, reinterpret_cast<const GLint *>(glm::value_ptr(data)));
}

void GLShader::UpdateUniform(const string &name, const Double &data) {
    auto location = FindUniformLocation(name);
    glUniform1dv(location, 1, &data);
}

void GLShader::UpdateUniform(const string &name, const Double2 &data) {
    auto location = FindUniformLocation(name);
    glUniform2dv(location, 1, glm::value_ptr(data));
}

void GLShader::UpdateUniform(const string &name, const Double3 &data) {
    auto location = FindUniformLocation(name);
    glUniform3dv(location, 1, glm::value_ptr(data));
}

void GLShader::UpdateUniform(const string &name, const Double4 &data) {
    auto location = FindUniformLocation(name);
    glUniform4dv(location, 1, glm::value_ptr(data));
}

void GLShader::UpdateUniform(const string &name, const Float &data) {
    auto location = FindUniformLocation(name);
    glUniform1fv(location, 1, &data);
}

void GLShader::UpdateUniform(const string &name, const Float2 &data) {
    auto location = FindUniformLocation(name);
    glUniform2fv(location, 1, glm::value_ptr(data));
}

void GLShader::UpdateUniform(const string &name, const Float3 &data) {
    auto location = FindUniformLocation(name);
    glUniform3fv(location, 1, glm::value_ptr(data));
}

void GLShader::UpdateUniform(const string &name, const Float4 &data) {
    auto location = FindUniformLocation(name);
    glUniform4fv(location, 1, glm::value_ptr(data));
}

void GLShader::UpdateUniform(const string &name, const Int &data) {
    auto location = FindUniformLocation(name);
    glUniform1iv(location, 1, &data);
}

void GLShader::UpdateUniform(const string &name, const Int2 &data) {
    auto location = FindUniformLocation(name);
    glUniform2iv(location, 1, glm::value_ptr(data));
}

void GLShader::UpdateUniform(const string &name, const Int3 &data) {
    auto location = FindUniformLocation(name);
    glUniform3iv(location, 1, glm::value_ptr(data));
}

void GLShader::UpdateUniform(const string &name, const Int4 &data) {
    auto location = FindUniformLocation(name);
    glUniform4iv(location, 1, glm::value_ptr(data));
}

void GLShader::UpdateUniform(const string &name, const UInt &data) {
    auto location = FindUniformLocation(name);
    glUniform1uiv(location, 1, &data);
}

void GLShader::UpdateUniform(const string &name, const UInt2 &data) {
    auto location = FindUniformLocation(name);
    glUniform2uiv(location, 1, glm::value_ptr(data));
}

void GLShader::UpdateUniform(const string &name, const UInt3 &data) {
    auto location = FindUniformLocation(name);
    glUniform3uiv(location, 1, glm::value_ptr(data));
}

void GLShader::UpdateUniform(const string &name, const UInt4 &data) {
    auto location = FindUniformLocation(name);
    glUniform4uiv(location, 1, glm::value_ptr(data));
}

void GLShader::UpdateUniform(const string &name, const Matrix2 &data) {
    auto location = FindUniformLocation(name);
    glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(data));
}

void GLShader::UpdateUniform(const string &name, const Matrix3 &data) {
    auto location = FindUniformLocation(name);
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(data));
}

void GLShader::UpdateUniform(const string &name, const Matrix4 &data) {
    auto location = FindUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
}

}

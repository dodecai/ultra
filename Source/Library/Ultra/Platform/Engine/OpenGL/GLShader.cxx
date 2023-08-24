module;

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

module Ultra.Platform.Renderer.GLShader;

import Ultra.Engine.Utility.ShaderCompiler;
import Ultra.System.FileSystem;

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
        default:                            { return GL_SHADER; } // ToDo: Fixes compiler warning, but we don't need a default path.
    }
}

string GetGLShaderDataTypeName(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::Bool:          { return "bool"; break; }
        case ShaderDataType::Bool2:         { return "bvec2"; break; }
        case ShaderDataType::Bool3:         { return "bvec3"; break; }
        case ShaderDataType::Bool4:         { return "bvec4"; break; }
        case ShaderDataType::Float:         { return "float"; break; }
        case ShaderDataType::Float2:        { return "vec2"; break; }
        case ShaderDataType::Float3:        { return "vec3"; break; }
        case ShaderDataType::Float4:        { return "vec4"; break; }
        case ShaderDataType::Int:           { return "int"; break; }
        case ShaderDataType::Int2:          { return "ivec2"; break; }
        case ShaderDataType::Int3:          { return "ivec3"; break; }
        case ShaderDataType::Int4:          { return "ivec4"; break; }
        case ShaderDataType::Mat2:          { return "mat2"; break; }
        case ShaderDataType::Mat3:          { return "mat3"; break; }
        case ShaderDataType::Mat4:          { return "mat4"; break; }
        case ShaderDataType::Texture1D:     { return "sampler1D"; break; }
        case ShaderDataType::Texture2D:     { return "sampler2D"; break; }
        case ShaderDataType::Texture3D:     { return "sampler3D"; break; }
        case ShaderDataType::TextureCube:   { return "samplerCube"; break; }
        default: {
            AppAssert(true, "Not implemented!");
            return {};
        }
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
        
        // Option A: Compiled shader
        //if (FileSystemObjectExists(cache)) {

        //}
        auto spirv = ShaderCompiler::Compile(mShaderName, (ShaderType)key, code);
        glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), static_cast<GLsizei>(spirv.size() * sizeof(uint32_t)));
        string entrypoint = "main"; // Get VS entry point name
        glSpecializeShader(shader, entrypoint.c_str(), 0, nullptr, nullptr);
        
        // Opiton B: Compile Shader
        //const GLchar *source = code.c_str();
        //glShaderSource(shader, 1, &source, 0);
        //glCompileShader(shader);

        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (compiled == GL_FALSE) {
            GLint length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

            vector<GLchar> message(length);
            glGetShaderInfoLog(shader, length, &length, message.data());

            glDeleteShader(shader);

            LogError("Couldn't compile shader: {}", string(message.begin(), message.end()));
            break;
        }

        // Attach Shader
        glAttachShader(program, shader);
        ids.push_back(shader);

        // Cache Shader
        //if (!spirv.empty()) {
        //    auto cache = "./Data/Cache/Shaders/" + mShaderName + "." + ShaderTypeToString((ShaderType)key) + ".spirv";
        //    WriteFile(cache, spirv);
        //}
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
        LogError("Couldn't link shaders: {}", string(message.begin(), message.end()));

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

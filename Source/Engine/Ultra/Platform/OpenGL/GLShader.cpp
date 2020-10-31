#include "GLShader.h"

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

#include <Omnia/System/FileSystem.h>


namespace Ultra {

GLShader::GLShader(const string &source) {
	string shaderSource = ReadFile(source);
	auto sources = Prepare(shaderSource);

	Compile(sources);

	ShaderName = GetFileName(source);

	//auto test1 = GetFileExtension(source);
	//auto test2 = GetFileName(source);
	//auto test3 = GetFileRoot(source);
	//auto test4 = GetFilePath(source);
}

GLShader::GLShader(const string &vertexSource, const string &fragmentSource) {
	std::unordered_map<uint32_t, std::string> sources;
	sources[GL_VERTEX_SHADER] = ReadFile(vertexSource);
	sources[GL_FRAGMENT_SHADER] = ReadFile(fragmentSource);

	Compile(sources);

	ShaderName = GetFileName(vertexSource);
}

GLShader::~GLShader() {
	glDeleteProgram(RendererID);
}

void GLShader::Bind() const {
	glUseProgram(RendererID);
}

void GLShader::Reload() const {
}

void GLShader::Unbind() const {
	glUseProgram(0);
}


const std::string GLShader::GetName() const {
    return ShaderName;
}

const unordered_map<string, ShaderBuffer> &GLShader::GetBuffers() const {
    static unordered_map<string, ShaderBuffer> result;
    return result;
}

const unordered_map<string, ShaderResourceDeclaration> &GLShader::GetResources() const {
    static unordered_map<string, ShaderResourceDeclaration> result;
    return result;
}


void GLShader::SetUniformBuffer(const string &name, const void *data, size_t size) {
    // ToDo: Check for better OpenGL Function
    auto location = GetUniformLocation(name);
    glUniform1iv(location, size, (int *)data);
}

void GLShader::SetUniform(const string &name, bool data) {
    auto location = GetUniformLocation(name);
    glUniform1ui(location, data);
}

void GLShader::SetUniform(const string &name, int data) {
    auto location = GetUniformLocation(name);
    glUniform1i(location, data);
}

void GLShader::SetUniform(const string &name, float data) {
    auto location = GetUniformLocation(name);
    glUniform1f(location, data);
}

void GLShader::SetUniform(const string &name, glm::vec2 &data) {
    auto location = GetUniformLocation(name);
    glUniform2f(location, data.x, data.y);
}

void GLShader::SetUniform(const string &name, glm::vec3 &data) {
    auto location = GetUniformLocation(name);
    glUniform3f(location, data.x, data.y, data.z);
}

void GLShader::SetUniform(const string &name, glm::vec4 &data) {
    auto location = GetUniformLocation(name);
    glUniform4f(location, data.x, data.y, data.z, data.w);
}

void GLShader::SetUniform(const string &name, glm::mat3 &data) {
    auto location = GetUniformLocation(name);
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(data));
}

void GLShader::SetUniform(const string &name, glm::mat4 &data) {
    auto location = GetUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
}


static void CheckCompileErrors(GLuint shader, std::string type) {
	GLint success;
	GLchar infoLog[1024];
	if(type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	} else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if(!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

static GLenum ShaderTypeFromString(const std::string &type) {
	if (type == "vertex") return GL_VERTEX_SHADER;
	if (type == "pixel" || type == "fragment") return GL_FRAGMENT_SHADER;

	// Unknown Shader
	return 0;
}


void GLShader::Compile(std::unordered_map<uint32_t, std::string> sources) {

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    GLuint program = glCreateProgram();
    // ToDo: Is a vector really that bad here?
    std::vector<GLenum> glShaderIDs;
    glShaderIDs.reserve(sources.size());

    for (auto &&[key, value]: sources) {
        GLenum shaderType = key;
        std::string &shaderSource = value;

        uint32_t shader = glCreateShader(shaderType);

        const GLchar *cshaderSource = shaderSource.c_str();
        glShaderSource(shader, 1, &cshaderSource, 0);

        glCompileShader(shader);

        GLint isCompiled = 0;

        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        //CheckCompileErrors(id, "VERTEX");
        if(isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(shader);

            // Use the infoLog as you see fit.
            APP_LOG_ERROR("Couldn't compile shader!");

            // In this simple program, we'll just leave
            break;
        }

        glAttachShader(program, shader);
        glShaderIDs.push_back(shader);
    }



    // Link our program
    glLinkProgram(program);
    GLint isLinked = 0; // Note the different functions here: glGetProgram* instead of glGetShader*.
    glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(program);

        // Don't leak shaders either.
        for (auto id : glShaderIDs) {
            glDeleteShader(id);
        }

        // Use the infoLog as you see fit.

        // In this simple program, we'll just leave
        return;
    }

    // Always detach shaders after a successful link.
    RendererID = program;
    for (auto id : glShaderIDs) {
        glDetachShader(program, id);
    }
}

std::unordered_map<uint32_t, std::string> GLShader::Prepare(std::string &source) {
	std::unordered_map<GLenum, std::string> shaderSources;

	const char *typeToken = "#type";
	size_t typeTokenLength = strlen(typeToken);
	size_t pos = source.find(typeToken, 0);
	while (pos != std::string::npos) {
		size_t eol = source.find_first_of("\r\n", pos);
		// Syntax Error: eol != std::string::npos
		size_t begin = pos + typeTokenLength + 1;
		std::string type = source.substr(begin, eol - begin);
		// Unknown Shader Type type == "vertex" || type == "fragment" || "pixel"

		size_t nextLinePos = source.find_first_not_of("\r\n", eol);
		// Syntax Error: nextLinePos != std::string::npos
		pos = source.find(typeToken, nextLinePos);

		shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
	}

	return shaderSources;
}

int32_t GLShader::GetUniformLocation(const string &name) const {
	auto location = glGetUniformLocation(RendererID, name.c_str());
	return location;

	// ToDo: Runs very slow that you can easily see it, the question is why?
	//if (UniformLocationCache.find(name) != UniformLocationCache.end()) return UniformLocationCache[name];
	//auto location = glGetUniformLocation(RendererID, name.c_str());
	//if (location == -1) {
	//	// Uniform not found!
	//	return location;
	//}
	//UniformLocationCache[name] = location;
	//return location;
}

}

#include "GLShader.h"

#include <glad/glad.h>
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
	gladLoadGL();

	std::unordered_map<GLenum, std::string> sources;
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

void GLShader::Unbind() const {
	glUseProgram(0);
}

const std::string GLShader::GetName() const {
	return ShaderName;
}

void GLShader::SetInt(const string &name, int value) { UploadaUniformInt(name, value); }
void GLShader::SetIntArray(const string &name, int *values, size_t count) { UploadUniformIntArray(name, values, count); }
void GLShader::SetFloat(const string &name, const float value) { UploadaUniformFloat(name, value); }
void GLShader::SetFloat3(const string &name, const glm::vec3 &values) { UploadaUniformFloat3(name, values); }
void GLShader::SetFloat4(const string &name, const glm::vec4 &values) { UploadaUniformFloat4(name, values); }
void GLShader::SetMat4(const string &name, const glm::mat4 &values) { UploadaUniformMat4(name, values); }


void GLShader::Compile(std::unordered_map<GLenum, std::string> sources) {

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

static GLenum ShaderTypeFromString(const std::string &type) {
	if (type == "vertex") return GL_VERTEX_SHADER;
	if (type == "pixel" || type == "fragment") return GL_FRAGMENT_SHADER;

	// Unknown Shader
	return 0;
}

std::unordered_map<GLenum, std::string> GLShader::Prepare(std::string &source) {
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

void GLShader::UploadaUniformInt(const std::string &name, int values) const {
	auto location = GetUniformLocation(name);
	glUniform1i(location, values);
}
void GLShader::UploadUniformIntArray(const string &name, int *values, size_t count) const {
	auto location = GetUniformLocation(name);
	glUniform1iv(location, count, values);
}
void GLShader::UploadaUniformFloat(const std::string &name, const float values) const {
	auto location = GetUniformLocation(name);
	glUniform1f(location, values);
}
void GLShader::UploadaUniformFloat2(const std::string &name, const glm::vec2 &values) const {
	auto location = GetUniformLocation(name);
	glUniform2f(location, values.x, values.y);
}
void GLShader::UploadaUniformFloat3(const std::string &name, const glm::vec3 &values) const {
	auto location = GetUniformLocation(name);
	glUniform3f(location, values.x, values.y, values.z);
}
void GLShader::UploadaUniformFloat4(const std::string &name, const glm::vec4 &values) const {
	auto location = GetUniformLocation(name);
	glUniform4f(location, values.x, values.y, values.z, values.w);
}
void GLShader::UploadaUniformMat3(const std::string &name, const glm::mat3 &matrix) const {
	auto location = GetUniformLocation(name);
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
void GLShader::UploadaUniformMat4(const std::string &name, const glm::mat4 &matrix) const {
	auto location = GetUniformLocation(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

}

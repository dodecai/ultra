#pragma once

#include <unordered_map>
#include "Engine/Renderer/Shader.h"

namespace Ultra {

class GLShader: public Shader {
	uint32_t RendererID;
	std::string ShaderName;

	mutable std::unordered_map<std::string, int32_t> UniformLocationCache;

public:
	GLShader(const string &source);
	GLShader(const string &vertexSource, const string &fragmentSource);
	virtual ~GLShader();

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual const std::string GetName() const override;

	virtual void SetInt(const string &name, int value) override;
	virtual void SetIntArray(const string &name, int *values, size_t count) override;
	virtual void SetFloat(const string &name, const float value) override;
	virtual void SetFloat3(const string &name, const glm::vec3 &values) override;
	virtual void SetFloat4(const string &name, const glm::vec4 &values) override;
	virtual void SetMat4(const string &name, const glm::mat4 &values) override;

private:
	void Compile(std::unordered_map<unsigned int, std::string> sources);
	std::unordered_map<unsigned int, std::string> Prepare(std::string &source);

	int32_t GetUniformLocation(const string &name) const;

	// Uniforms
	virtual void UploadaUniformInt(const std::string &name, int values) const;
	virtual void UploadUniformIntArray(const string &name, int *values, size_t count) const;

	virtual void UploadaUniformFloat(const string &name, const float value) const;
	virtual void UploadaUniformFloat2(const std::string &name, const glm::vec2 &values) const;
	virtual void UploadaUniformFloat3(const std::string &name, const glm::vec3 &values) const;
	virtual void UploadaUniformFloat4(const std::string &name, const glm::vec4 &values) const;

	virtual void UploadaUniformMat3(const std::string &name, const glm::mat3 &matrix) const;
	virtual void UploadaUniformMat4(const std::string &name, const glm::mat4 &matrix) const;
};

}

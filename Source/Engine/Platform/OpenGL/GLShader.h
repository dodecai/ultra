#pragma once

#include <unordered_map>
#include "Renderer/Shader.h"

namespace Ultra {

class GLShader: public Shader {
	uint32_t RendererID;
	std::string ShaderName;

public:
	GLShader(const string &source);
	GLShader(const string &vertexSource, const string &fragmentSource);
	virtual ~GLShader();

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual const std::string GetName() const override;

	virtual void UploadaUniformInt(const std::string &name, int values) const override;

	virtual void UploadaUniformFloat(const std::string &name, float values) const override;
	virtual void UploadaUniformFloat2(const std::string &name, const glm::vec2 &values) const override;
	virtual void UploadaUniformFloat3(const std::string &name, const glm::vec3 &values) const override;
	virtual void UploadaUniformFloat4(const std::string &name, const glm::vec4 &values) const override;

	virtual void UploadaUniformMat3(const std::string &name, const glm::mat3 &matrix) const override;
	virtual void UploadaUniformMat4(const std::string &name, const glm::mat4 &matrix) const override;

private:
	void Compile(std::unordered_map<unsigned int, std::string> sources);
	std::unordered_map<unsigned int, std::string> Prepare(std::string &source);
};

}

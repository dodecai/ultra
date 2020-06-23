#pragma once

#include <memory>
#include <unordered_map>
#include <string>
using std::string;

#include <Omnia/Core.h>

#include <glm/glm.hpp>

namespace Ultra {

class Shader {
	uint32_t RendererID;
public:
	virtual ~Shader() {}

	static Omnia::Reference<Shader> Create(const string &source);
	static Omnia::Reference<Shader> Create(const string &vertexSource, const string &fragmentSource);

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual const std::string GetName() const = 0;

	virtual void SetInteger(const string &name, int value) {}

	virtual void UploadaUniformInt(const std::string &name, int values) const = 0;

	virtual void UploadaUniformFloat(const std::string &name, float values) const = 0;
	virtual void UploadaUniformFloat2(const std::string &name, const glm::vec2 &values) const = 0;
	virtual void UploadaUniformFloat3(const std::string &name, const glm::vec3 &values) const = 0;
	virtual void UploadaUniformFloat4(const std::string &name, const glm::vec4 &values) const = 0;

	virtual void UploadaUniformMat3(const std::string &name, const glm::mat3 &matrix) const = 0;
	virtual void UploadaUniformMat4(const std::string &name, const glm::mat4 &matrix) const = 0;
};

class ShaderLibrary {
	std::unordered_map<std::string, std::shared_ptr<Shader>> Shaders;

public:
	void Add(const std::shared_ptr<Shader> &shader);
	std::shared_ptr<Shader> Load(const std::string &source);
	std::shared_ptr<Shader> Load(const std::string &vertex, const std::string &fragment);
	std::shared_ptr<Shader> Get(const std::string &name);

	bool Exist(const string &name);
};

}

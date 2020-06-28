#pragma once

#include <glm/glm.hpp>

#include "Ultra.pch"
#include "Core.h"

namespace Ultra {

class Shader {
	uint32_t RendererID;
public:
	virtual ~Shader() {}

	static Omnia::Reference<Shader> Create(const string &source);
	static Omnia::Reference<Shader> Create(const string &vertexSource, const string &fragmentSource);

	virtual void Bind() const = 0;	//Activate, Compile, Load
	virtual void Unbind() const = 0;

	virtual const std::string GetName() const = 0;

	virtual void SetInt(const string &name, int value) = 0;
	virtual void SetIntArray(const string &name, int *values, size_t count) = 0;
	virtual void SetFloat(const string &name, const float value) = 0;
	virtual void SetFloat3(const string &name, const glm::vec3 &values) = 0;
	virtual void SetFloat4(const string &name, const glm::vec4 &values) = 0; // Vec seams now more "common" ...
	virtual void SetMat4(const string &name, const glm::mat4 &values) = 0; // + 2 & 3

	// From Prototype
	virtual void SetBool(const std::string &name, bool value) const {}
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

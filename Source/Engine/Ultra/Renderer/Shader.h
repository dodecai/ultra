#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Ultra.pch"
#include "Ultra/Core.h"

#include "ShaderUniform.h"

namespace Ultra {

class ShaderUniform {
public:
    ShaderUniform() = default;
    ShaderUniform(const string &name, ShaderUniformType type, uint32_t size, uint32_t offset);

    const string &GetName() const { return mName; }
    const ShaderUniformType GetType() const { return mType; }
    const uint32_t GetSize() const { return mSize; }
    const uint32_t GetOffset() const { return mOffset; }

    static const string UniformTypeToString(ShaderUniformType type);

private:
    string mName;
    ShaderUniformType mType = ShaderUniformType::None;
    uint32_t mSize = 0;
    uint32_t mOffset = 0;
};

struct ShaderUniformBuffer {
    string Name;
    uint32_t BindingPoint;
    uint32_t Index;
    uint32_t Size;
    uint32_t RendererID;
    vector<ShaderUniform> Uniforms;
};

struct ShaderBuffer {
    string Name;
    uint32_t Size = 0;
    unordered_map<string, ShaderUniform> Uniforms;
};

class Shader {
public:
    // Default
	virtual ~Shader() = default;
	static Reference<Shader> Create(const string &source);
	static Reference<Shader> Create(const string &vertexSource, const string &fragmentSource);
    virtual void Reload() const = 0;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

    // Accessors
    virtual const std::string GetName() const = 0;
    virtual const unordered_map<string, ShaderBuffer> &GetBuffers() const = 0;
    virtual const unordered_map<string, ShaderResourceDeclaration> &GetResources() const = 0;

    // Mutators
    virtual void SetUniformBuffer(const string &name, const void *data, size_t size) = 0;
    virtual void SetUniform(const string &name, bool data) = 0;
    virtual void SetUniform(const string &name, int data) = 0;
    virtual void SetUniform(const string &name, float data) = 0;
    virtual void SetUniform(const string &name, glm::vec2 &data) = 0;
    virtual void SetUniform(const string &name, glm::vec3 &data) = 0;
    virtual void SetUniform(const string &name, glm::vec4 &data) = 0;
    virtual void SetUniform(const string &name, glm::mat3 &data) = 0;
    virtual void SetUniform(const string &name, glm::mat4 &data) = 0;

protected:
    static vector<Reference<Shader>> sShaders;
};

class ShaderLibrary {
public:
	void Add(const Reference<Shader> &shader);
	void Load(const string &source);
	void Load(const string &vertex, const string &fragment);
	Reference<Shader> Get(const string &name);

	bool Exist(const string &name);

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> Shaders;
};

}

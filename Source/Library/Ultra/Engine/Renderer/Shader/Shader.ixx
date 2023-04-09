module;

#include <glm/glm.hpp>

export module Ultra.Engine.Shader;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.GFX.Context;
export import Ultra.Engine.ShaderData;

export namespace Ultra {

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
    ShaderUniformType mType = ShaderUniformType::Null;
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
    Shader() = default;
    ~Shader() = default;

    static Reference<Shader> Create(const string &source) { return {}; }
    virtual bool Reload() const = 0;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    // Accessors
    virtual const string &GetName() const = 0;

    // Mutators
    virtual void SetUniformBuffer(const string &name, const void *data, size_t size) = 0;
    virtual void SetUniform(const string &name, bool data) = 0;
    virtual void SetUniform(const string &name, int data) = 0;
    virtual void SetUniform(const string &name, float data) = 0;
    virtual void SetUniform(const string &name, glm::vec2 &data) = 0;
    virtual void SetUniform(const string &name, glm::vec3 &data) = 0;
    virtual void SetUniform(const string &name, glm::vec4 &data) = 0;
    //virtual void SetUniform(const string &name, glm::mat2 &data) = 0;
    virtual void SetUniform(const string &name, glm::mat3 &data) = 0;
    virtual void SetUniform(const string &name, glm::mat4 &data) = 0;
};

class ShaderLibrary {
public:
    // Default
    ShaderLibrary() = default;
    ~ShaderLibrary() = default;

    void Load(const string &directory) {}

    // Accessors
    const Reference<Shader> &Get(const string &name) {
        return mShaders.at(name);
    }
    size_t GetSize() const {
        return mShaders.size();
    }

private:
    // Methods
    void Add(const Reference<Shader> &shader) {
        auto &name = shader->GetName();
        mShaders[name] = shader;
    }

    // Properties
    unordered_map<string, Reference<Shader>> mShaders;
};

}

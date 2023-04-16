module;

#include <glm/glm.hpp>

export module Ultra.Engine.ZShader;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.GFX.Context;
export import Ultra.Engine.ShaderData;

export namespace Ultra {

class ShaderUniform {
public:
    ShaderUniform() = default;
    ShaderUniform(const string &name, ShaderUniformType type, uint32_t size, uint32_t offset):
        mName(name),
        mType(type),
        mSize(size),
        mOffset(offset) {
    }

    const string &GetName() const { return mName; }
    const ShaderUniformType GetType() const { return mType; }
    const uint32_t GetSize() const { return mSize; }
    const uint32_t GetOffset() const { return mOffset; }

    static const string UniformTypeToString(ShaderUniformType type) {
        if (type == ShaderUniformType::Bool)        return "Boolean";
        else if (type == ShaderUniformType::Int)    return "Int";
        else if (type == ShaderUniformType::Float)  return "Float";
        return "Null";
    }

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

class ShaderResourceDeclaration {
public:
    ShaderResourceDeclaration() = default;
    ShaderResourceDeclaration(const string &name, uint32_t resourceRegister, uint32_t count):
        mName(name),
        mRegister(resourceRegister),
        mCount(count) {
    }

    virtual const string &GetName() const { return mName; }
    virtual const uint32_t GetRegister() const { return mRegister; }
    virtual const uint32_t GetCount() const { return mCount; }

private:
    string mName;
    uint32_t mRegister = 0;
    uint32_t mCount = 0;
};


class ZShader {
public:
    // Default
    ZShader() = default;
    ~ZShader() = default;

    static Reference<ZShader> Create(const string &source);
    virtual bool Reload() const = 0;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    // Accessors
    virtual const string &GetName() const = 0;
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
    //virtual void SetUniform(const string &name, glm::mat2 &data) = 0;
    virtual void SetUniform(const string &name, glm::mat3 &data) = 0;
    virtual void SetUniform(const string &name, glm::mat4 &data) = 0;
};

class ShaderLibrary {
public:
    // Default
    ShaderLibrary() = default;
    ~ShaderLibrary() = default;

    void Load(const string &source) {
        auto shader = ZShader::Create(source);
        Add(shader);
    }
    const Reference<ZShader> &Get(const string &name) {
        if (!Exist(name)) return nullptr;
        return mShaders[name];
    }
    bool Exist(const string &name) {
        if (mShaders.empty()) return false;
        return mShaders.find(name) != mShaders.end();
    }

private:
    // Methods
    void Add(const Reference<ZShader> &shader) {
        auto &name = shader->GetName();
        if (Exist(name)) { return; }
        mShaders[name] = shader;
    }

    // Properties
    unordered_map<string, Reference<ZShader>> mShaders;
};

}

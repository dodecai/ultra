module;

export module Ultra.Engine.Shader;

import Ultra.Core;
import Ultra.Logger;

export namespace Ultra {

class Shader {
public:
    // Default
    Shader() = default;
    ~Shader() = default;

    static Reference<Shader> Create(const string &source) { return {}; }
    virtual bool Reload() { return {}; };

    // Accessors
    const string &GetName() const { return mName; }

private:
    string mName = {};
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

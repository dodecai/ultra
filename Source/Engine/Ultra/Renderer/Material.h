#pragma once

#include "Ultra/Core.h"

#include "Shader.h"
#include "Texture.h"

namespace Ultra {

enum class MaterialFlag {
    None      = 0x01,
    DepthTest = 0x02,
    Blend     = 0x04,
};

class Material {
    friend class MaterialInstance;

public:
    Material(const Reference<Shader> &shader);
    virtual ~Material();

    const string &GetName();
    template <typename T>
    T &Get(const string &name);
    uint32_t GetFlags() const;
    template <typename T>
    T &GetResource(const string &name);
    Reference<Shader> &GetShader();

    template <typename T>
    void Set(const string &name, const T &value);
    void Set(const string &name, Reference<Texture> &texture);
    void Set(const string &name, Reference<Texture2D> &texture);
    void Set(const string &name, Reference<Texture3D> &texture);

private:
    void Allocate();

private:
    string mName;
    uint32_t mMaterialFlags;

    //Buffer mUniformStorageBuffer;
    vector<Reference<Texture>> mTextures;
};

class MaterialInstance {
    friend class Material;
    
public:
    MaterialInstance(const Reference<Material> &material, const string &name = "");
    virtual ~MaterialInstance();
    static Reference<MaterialInstance> Create(const Reference<Material> &material);

    const string &GetName();
    template <typename T>
    T &Get(const string &name);
    uint32_t GetFlags() const;
    template <typename T>
    T &GetResource(const string &name);
    Reference<Shader> &GetShader();

    template <typename T>
    void Set(const string &name, const T &value);
    void Set(const string &name, Reference<Texture> &texture);
    void Set(const string &name, Reference<Texture2D> &texture);
    void Set(const string &name, Reference<Texture3D> &texture);

private:
    void Allocate();

private:
    string mName;
    Reference<Material> mMaterial;

    //Buffer mUniformStorageBuffer;
    vector<Reference<Texture>> mTextures;
};

}

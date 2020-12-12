#include "Material.h"
#include "Material.h"
#include "Material.h"
#include "Material.h"
#include "Material.h"
#include "Material.h"
#include "Material.h"
#include "Material.h"
#include "Material.h"
#include "Material.h"
#include "Material.h"
#include "Material.h"
#include "Material.h"
#include "Material.h"

namespace Ultra {

Material::Material(const Reference<Shader>& shader) {
}

Material::~Material() {

}

const string &Material::GetName() {
    return mName;
}

uint32_t Material::GetFlags() const {
    return uint32_t();
}

Reference<Shader> &Material::GetShader() {
    return mShader;
}

void Material::Set(const string &name, Reference<Texture> &texture) {
}

void Material::Allocate() {
}


template<typename T>
T &Material::Get(const string &name) {
   return T();
}

template<typename T>
T &Material::GetResource(const string &name) {
    return T();
}

template<typename T>
void Material::Set(const string &name, const T &value) {
}



MaterialInstance::MaterialInstance(const Reference<Material> &material, const string &name) {
}

MaterialInstance::~MaterialInstance() {
}

Reference<MaterialInstance> MaterialInstance::Create(const Reference<Material> &material) {
    return Reference<MaterialInstance>();
}

template<typename T>
const string &MaterialInstance::GetName() {
    return mName;
}

uint32_t MaterialInstance::GetFlags() const {
    return uint32_t();
}

Reference<Shader> &MaterialInstance::GetShader() {
    return mShader;
}

void MaterialInstance::Set(const string &name, Reference<Texture> &texture) {
}

void MaterialInstance::Allocate() {
}

template<typename T>
T &MaterialInstance::Get(const string &name) {
    return T();
}

template<typename T>
T &MaterialInstance::GetResource(const string &name) {
    return T();
}

template<typename T>
void MaterialInstance::Set(const string &name, const T &value) {
}


}

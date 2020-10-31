#include "Shader.h"
#include "Ultra/Platform/OpenGL/GLShader.h"
#include "Ultra/Platform/Vulkan/VKShader.h"

#include "Renderer.h"

namespace Ultra {

Reference<Shader> Shader::Create(const string &source) {
    switch (Context::API) {
        case GraphicsAPI::Null:		{ return nullptr; }
        case GraphicsAPI::OpenGL:	{ return CreateReference<GLShader>(source); }
        case GraphicsAPI::Vulkan:	{ return CreateReference<VKShader>(source); }
        default:                    { break; }
	}
    AppLogCritical("[Engine::Renderer::Shader] ", "The current graphics API doesn't support Shaders!");
	return nullptr;
}

Reference<Shader> Shader::Create(const string &vertexSource, const string &fragmentSource) {
    switch (Context::API) {
        case GraphicsAPI::Null:		{ return nullptr; }
        case GraphicsAPI::OpenGL:	{ return CreateReference<GLShader>(vertexSource, fragmentSource); }
        case GraphicsAPI::Vulkan:	{ return CreateReference<VKShader>(vertexSource, fragmentSource); }
        default:                    { break; }
	}
    AppLogCritical("[Engine::Renderer::Shader] ", "The current graphics API doesn't support Shaders!");
	return nullptr;
}

ShaderUniform::ShaderUniform(const string &name, ShaderUniformType type, uint32_t size, uint32_t offset): 
    mName(name),
    mType(type),
    mSize(size),
    mOffset(offset) {
}

const string ShaderUniform::UniformTypeToString(ShaderUniformType type) {
    if (type == ShaderUniformType::Bool)        return "Boolean";
    else if (type == ShaderUniformType::Int)    return "Int";
    else if (type == ShaderUniformType::Float)  return "Float";
    return "None";
}


void ShaderLibrary::Add(const Reference<Shader> &shader) {
	auto &name = shader->GetName();
	if (Exist(name)) { return; }
	Shaders[name] = shader;
}

void ShaderLibrary::Load(const std::string &source) {
    auto shader = Shader::Create(source);
    Add(shader);
}

void ShaderLibrary::Load(const std::string &vertex, const std::string &fragment) {
	auto shader = Shader::Create(vertex, fragment);
	Add(shader);
}

Reference<Shader> ShaderLibrary::Get(const string &name) {

	if (!Exist(name)) {
		// Shader doesn't exist!
		return nullptr;
	}


	return Shaders[name];
}

bool ShaderLibrary::Exist(const string &name) {
	if (Shaders.empty()) return false;
	return Shaders.find(name) != Shaders.end();
}

}

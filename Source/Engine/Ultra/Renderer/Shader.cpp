#include "Shader.h"
#include "Ultra/Platform/OpenGL/GLShader.h"
#include "Ultra/Platform/Vulkan/VKShader.h"

#include "Renderer.h"

namespace Ultra {

Omnia::Reference<Shader> Shader::Create(const string &source) {
    switch (Context::API) {
        case GraphicsAPI::Null:		{ return nullptr; }
        case GraphicsAPI::OpenGL:	{ return Omnia::CreateReference<GLShader>(source); }
        case GraphicsAPI::Vulkan:	{ return Omnia::CreateReference<VKShader>(source); }
        default:                    { break; }
	}
    AppLogCritical("[Engine::Renderer::Shader] ", "The current graphics API doesn't support Shaders!");
	return nullptr;
}

Omnia::Reference<Shader> Shader::Create(const string &vertexSource, const string &fragmentSource) {
    switch (Context::API) {
        case GraphicsAPI::Null:		{ return nullptr; }
        case GraphicsAPI::OpenGL:	{ return Omnia::CreateReference<GLShader>(vertexSource, fragmentSource); }
        case GraphicsAPI::Vulkan:	{ return Omnia::CreateReference<VKShader>(vertexSource, fragmentSource); }
        default:                    { break; }
	}
    AppLogCritical("[Engine::Renderer::Shader] ", "The current graphics API doesn't support Shaders!");
	return nullptr;
}


void ShaderLibrary::Add(const std::shared_ptr<Shader> &shader) {
	auto &name = shader->GetName();
	if (Exist(name)) { return; }
	Shaders[name] = shader;
}

std::shared_ptr<Shader> ShaderLibrary::Load(const std::string &source) {
	auto shader = Shader::Create(source);
	Add(shader);
	return shader;
}

std::shared_ptr<Shader> ShaderLibrary::Load(const std::string &vertex, const std::string &fragment) {
	auto shader = Shader::Create(vertex, fragment);
	Add(shader);
	return shader;
}

std::shared_ptr<Shader> ShaderLibrary::Get(const std::string &name) {

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

#include "Shader.h"

#include "Renderer.h"

#include "Engine/Platform/OpenGL/GLShader.h"

namespace Ultra {

Omnia::Reference<Shader> Shader::Create(const string &source) {
	switch (Renderer::GetAPI()) {
		case RendererAPI::API::Null:		{ return nullptr; }
		case RendererAPI::API::OpenGL:		{ return Omnia::CreateReference<GLShader>(source); }
	}
	// Unknown RendererAPI
	return nullptr;
}

Omnia::Reference<Shader> Shader::Create(const string &vertexSource, const string &fragmentSource) {
	switch (Renderer::GetAPI()) {
		case RendererAPI::API::Null:		{ return nullptr; }
		case RendererAPI::API::OpenGL:		{ return Omnia::CreateReference<GLShader>(vertexSource, fragmentSource); }
	}
	// Unknown RendererAPI
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

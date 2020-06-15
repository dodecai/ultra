#include "Shader.h"

//#include "Renderer.h"

#include "Platform/OpenGL/GLShader.h"

namespace Ultra {

Shader *Shader::Create(const string &vertexSource, const string &fragmentSource) {
	//switch (Renderer::GetAPI()) {
	//	case RendererAPI::Null:		{ return nullptr; }
	//	case RendererAPI::OpenGL:	{ return new GLShader(vertexSource, fragmentSource); }
	//}
	return new GLShader(vertexSource, fragmentSource);
	// Unknown RendererAPI
	return nullptr;
}
}

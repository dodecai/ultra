#pragma once

#include "Renderer/Shader.h"

namespace Ultra {

class GLShader: public Shader {
	uint32_t RendererID;

public:
	GLShader(const string &vertexSource, const string &fragmentSource);
	virtual ~GLShader();

	virtual void Bind() const override;
	virtual void Unbind() const override;
};

}

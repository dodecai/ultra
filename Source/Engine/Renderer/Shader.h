#pragma once

#include <string>
using std::string;

namespace Ultra {

class Shader {
	uint32_t RendererID;
public:
	virtual ~Shader() {}

	static Shader *Create(const string &vertexSource, const string &fragmentSource);

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;
};

}

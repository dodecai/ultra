#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "VertexArray.h"

namespace Ultra {

class RendererAPI {
public:
	enum class API {
		Null		= 0x0,
		OpenGL		= 0x1,
	};

private:
	static API s_API;

public:
	static RendererAPI *Create();

	virtual void Load() = 0;

	virtual void Clear() = 0;
	virtual void DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray, uint32_t indexCount = 0) = 0;

	virtual void SetClearColor(const glm::vec4 &color) = 0;
	virtual void SetViewport(const int32_t x, const int32_t y, const uint32_t width, const uint32_t height) = 0;

	inline static API GetAPI() { return s_API; };
};

}

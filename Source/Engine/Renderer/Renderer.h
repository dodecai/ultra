#pragma once

namespace Ultra {

enum class RendererAPI {
	Null		= 0x0,
	OpenGL		= 0x1,
};

class Renderer {
	static RendererAPI sRendererAPI;

public:
	inline static RendererAPI GetAPI() { return sRendererAPI; }
};

}

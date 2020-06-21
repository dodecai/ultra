#pragma once

#include "Renderer/Texture.h"

namespace Ultra {

class GLTexture2D: public Texture2D {
	std::string Patch;
	uint32_t Height;
	uint32_t Width;
	uint32_t RendererID;

public:
	GLTexture2D(const std::string &path);
	virtual ~GLTexture2D();

	virtual void Bind(uint32_t slot) const override;

	virtual uint32_t GetHeight() const override { return Height; }
	virtual uint32_t GetWidth() const override { return Width; }
};

}

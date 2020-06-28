#pragma once

#include "Engine/Renderer/Texture.h"

namespace Ultra {

class GLTexture2D: public Texture2D {
	std::string Patch;
	uint32_t DataFormat; //GLenum
	uint32_t RenderFormat; //GLenum
	uint32_t Height;
	uint32_t Width;
	uint32_t RendererID;

public:
	GLTexture2D(const std::string &path);
	GLTexture2D(const uint32_t width, const uint32_t height);
	virtual ~GLTexture2D();

	virtual void Bind(uint32_t slot) const override;
	virtual void UnBind(uint32_t slot) const override;
	virtual void SetData(void *data, uint32_t size) const override;

	virtual uint32_t GetHeight() const override { return Height; }
	virtual uint32_t GetWidth() const override { return Width; }
	virtual uint32_t GetRendererID() const override { return RendererID; };

	virtual bool operator==(const Texture& other) const override {
		return RendererID == ((GLTexture2D&)other).RendererID;
	}
};

}

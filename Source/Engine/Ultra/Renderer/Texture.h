#pragma once

#include "Ultra.pch"
#include "Ultra/Core.h"

#include "RendererAPI.h"

namespace Ultra {

enum class TextureFormat {
	Null	= 0x00,

	Red		= 0x01,
	Green	= 0x02,
	Blue	= 0x03,
	Alpha	= 0x04,

	RGB		= 0x10,
	BGR		= 0x11,

	RGBA	= 0x20,
	BGRA	= 0x21,
};

enum class TextureWrap {
	Null	= 0,
	Clamp	= 1,
	Repeat	= 2,
};

class Texture {
public:
	// Default
	Texture() = default;
	virtual ~Texture() = default;
	
	virtual void Bind(uint32_t slot = 0) const = 0;
	virtual void UnBind(uint32_t slot = 0) const = 0;

	// Accessors
	virtual TextureFormat GetFormat() const = 0;
	virtual uint32_t GetHeight() const = 0;
	virtual uint32_t GetWidth() const = 0;
	virtual uint32_t GetRendererID() const = 0;

	// Mutators
	virtual void SetData(void *data, uint32_t size) const = 0;

	// Operators
	virtual bool operator==(const Texture &other) const = 0;
	virtual operator RendererID() const = 0;
};

class Texture2D: public Texture {
public:
	// Default
	Texture2D() = default;
	virtual ~Texture2D() = default;

	static Reference<Texture2D> Create(const std::string &path);
	static Reference<Texture2D> Create(const uint32_t width, const uint32_t height, TextureFormat format = TextureFormat::RGBA, TextureWrap wrap = TextureWrap::Clamp);
};

class Texture3D: public Texture {
public:
	// Default
	Texture3D() = default;
	virtual ~Texture3D() = default;

	static Reference<Texture3D> Create(const std::string &path);
	static Reference<Texture3D> Create(const uint32_t width, const uint32_t height, TextureFormat format = TextureFormat::RGBA, TextureWrap wrap = TextureWrap::Clamp);
};

}

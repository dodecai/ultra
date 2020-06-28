#pragma once

#include "Ultra.pch"
#include "Core.h"

namespace Ultra {

class Texture {
public:
	virtual ~Texture() = default;
	
	virtual void Bind(uint32_t slot = 0) const = 0;
	virtual void UnBind(uint32_t slot = 0) const = 0;

	virtual uint32_t GetHeight() const = 0;
	virtual uint32_t GetWidth() const = 0;
	virtual uint32_t GetRendererID() const = 0;

	virtual void SetData(void *data, uint32_t size) const = 0;

	virtual bool operator==(const Texture &other) const = 0;
};

class Texture2D: public Texture {
public:
	virtual ~Texture2D() = default;
	static Reference<Texture2D> Create(const std::string &path);
	static Reference<Texture2D> Create(const uint32_t width, const uint32_t height);
};

}

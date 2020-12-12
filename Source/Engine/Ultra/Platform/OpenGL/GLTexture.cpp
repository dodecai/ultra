#include "GLTexture.h"

#include <glad/gl.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Ultra {

static GLenum ConvertTextureFormat(TextureFormat format) {
	switch (format) {
		case TextureFormat::Red:	{ return GL_RED; }
		case TextureFormat::Green:	{ return GL_GREEN; }
		case TextureFormat::Blue:	{ return GL_BLUE; }
		case TextureFormat::Alpha:	{ return GL_ALPHA; }

		case TextureFormat::RGB:	{ return GL_RGB; }
		case TextureFormat::BGR:	{ return GL_BGR; }

		case TextureFormat::RGBA:	{ return GL_RGBA; }
		case TextureFormat::BGRA:	{ return GL_BGRA; }
		
		default: {
			AppLogCritical("[Ultra::GLTexture2D::ConvertTextureFormat]: ", "The specified texture format is currently not supported!");
			return 0x0;
		}
	}
}

/*
 * Texture2D
*/
// Default
GLTexture2D::GLTexture2D(const std::string &path) {
	int width = 0;
	int height = 0;
	int channels = 0;
	stbi_uc *data = nullptr;

	stbi_set_flip_vertically_on_load(1);
	data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	if (!data) return;

	Width = width;
	Height = height;

	if (channels == 4) {
		RenderFormat = GL_RGBA16;
		DataFormat = GL_RGBA;
	} else if (channels == 3) {
		RenderFormat = GL_RGB16;
		DataFormat = GL_RGB;
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
	glTextureStorage2D(mRendererID, 1, RenderFormat, Width, Height);

	glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureSubImage2D(mRendererID, 0, 0, 0, Width, Height, DataFormat, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

GLTexture2D::GLTexture2D(const uint32_t width, const uint32_t height, TextureFormat format, TextureWrap wrap): Width (width), Height(height) {
	RenderFormat = GL_RGBA8;
	DataFormat = GL_RGBA;

	glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
	glTextureStorage2D(mRendererID, 1, RenderFormat, Width, Height);

	glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	GLenum wrapType = wrap == TextureWrap::Clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT;
	glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_R, wrapType);
	glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, wrapType);
	glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, wrapType);
	glTextureParameterf(mRendererID, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::GetCapabilities().MaxAnisotropy);

	glTexImage2D(GL_TEXTURE_2D, 0, ConvertTextureFormat(format), width, height, 0, ConvertTextureFormat(format), GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLTexture2D::~GLTexture2D() {
	glDeleteTextures(1, &mRendererID);
}


void GLTexture2D::Bind(uint32_t slot) const {
	glBindTextureUnit(slot, mRendererID);
}

void GLTexture2D::UnBind(uint32_t slot) const {
	glBindTextureUnit(slot, 0);
}


// Accessors
TextureFormat GLTexture2D::GetFormat() const {
	return TextureFormat();
}


// Mutators
void GLTexture2D::SetData(void *data, uint32_t size) const {
    // ToDo: Thread Syns (Lock, Unlock)
	uint32_t bpc = DataFormat == GL_RGB8 ? 4 : 3;
	//if (size == (Width * Height * bpc)) {
	//	//Data must be entire texture!
	//}
	glTextureSubImage2D(mRendererID, 0, 0, 0, Width, Height, DataFormat, GL_UNSIGNED_BYTE, data);
}


// Operators
bool GLTexture2D::operator==(const Texture &other) const {
	return mRendererID == ((GLTexture2D&)other).mRendererID;

}

GLTexture2D::operator RendererID() const {
	return mRendererID;
}



/*
 * Texture3D
*/
// Default
GLTexture3D::GLTexture3D(const string &path) {
	int width = 0;
	int height = 0;
	int channels = 0;
	stbi_uc *data = nullptr;

	stbi_set_flip_vertically_on_load(false);
	data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	if (!data) return;

	Width = width;
	Height = height;

	if (channels == 4) {
		RenderFormat = GL_RGBA16;
		DataFormat = GL_RGBA;
	} else if (channels == 3) {
		RenderFormat = GL_RGB16;
		DataFormat = GL_RGB;
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
	glTextureStorage2D(mRendererID, 1, RenderFormat, Width, Height);

	glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureSubImage2D(mRendererID, 0, 0, 0, Width, Height, DataFormat, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

GLTexture3D::GLTexture3D(const uint32_t width, const uint32_t height, TextureFormat format, TextureWrap wrap) {
	RenderFormat = GL_RGBA8;
	DataFormat = GL_RGBA;

	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &mRendererID);
	glTextureStorage2D(mRendererID, 1, ConvertTextureFormat(format), Width, Height);

	glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	GLenum wrapType = wrap == TextureWrap::Clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT;
	glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_R, wrapType);
	glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, wrapType);
	glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, wrapType);
	glTextureParameterf(mRendererID, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::GetCapabilities().MaxAnisotropy);
}

GLTexture3D::~GLTexture3D() {
	glDeleteTextures(1, &mRendererID);
}


void GLTexture3D::Bind(uint32_t slot) const {
	glBindTextureUnit(slot, mRendererID);
}

void GLTexture3D::UnBind(uint32_t slot) const {
	glBindTextureUnit(slot, 0);
}


// Accessors
TextureFormat GLTexture3D::GetFormat() const {
	return TextureFormat();
}


// Mutators
void GLTexture3D::SetData(void *data, uint32_t size) const {
    // ToDo: Thread Syns (Lock, Unlock)
	uint32_t bpc = DataFormat == GL_RGB8 ? 4 : 3;
	//if (size == (Width * Height * bpc)) {
	//	//Data must be entire texture!
	//}
	glTextureSubImage2D(mRendererID, 0, 0, 0, Width, Height, DataFormat, GL_UNSIGNED_BYTE, data);
}


// Operators
bool GLTexture3D::operator==(const Texture &other) const {
	return mRendererID == ((GLTexture3D&)other).mRendererID;
}

GLTexture3D::operator RendererID() const {
	return mRendererID;
}

}

#include "GLTexture.h"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Ultra {

GLTexture2D::GLTexture2D(const std::string &path) {
	int width;
	int height;
	int channels;

	stbi_set_flip_vertically_on_load(1);
	stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	if (!data) return;
	Width = width;
	Height = height;

	if (channels == 4) {
		RenderFormat = GL_RGBA8;
		DataFormat = GL_RGBA;
	} else if (channels == 3) {
		RenderFormat = GL_RGB8;
		DataFormat = GL_RGB;
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &RendererID);
	glTextureStorage2D(RendererID, 1, RenderFormat, Width, Height);

	glTextureParameteri(RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureSubImage2D(RendererID, 0, 0, 0, Width, Height, DataFormat, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

GLTexture2D::GLTexture2D(const uint32_t width, const uint32_t height): Width { width }, Height { height } {
	RenderFormat = GL_RGBA8;
	DataFormat = GL_RGBA;

	glCreateTextures(GL_TEXTURE_2D, 1, &RendererID);
	glTextureStorage2D(RendererID, 1, RenderFormat, Width, Height);

	glTextureParameteri(RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

GLTexture2D::~GLTexture2D() {
	glDeleteTextures(1, &RendererID);
}

void GLTexture2D::Bind(uint32_t slot) const {
	glBindTextureUnit(slot, RendererID);
}

void GLTexture2D::UnBind(uint32_t slot) const {
}

void GLTexture2D::SetData(void *data, uint32_t size) const {
	uint32_t bpc = DataFormat == GL_RGB8 ? 4 : 3;
	//if (size == (Width * Height * bpc)) {
	//	//Data must be entire texture!
	//}
	glTextureSubImage2D(RendererID, 0, 0, 0, Width, Height, DataFormat, GL_UNSIGNED_BYTE, data);
}

}

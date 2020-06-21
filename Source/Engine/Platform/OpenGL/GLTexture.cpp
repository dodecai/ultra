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

	GLenum dataFormat = 0;
	GLenum renderFormat = 0;
	if (channels == 4) {
		renderFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
	} else if (channels == 3) {
		renderFormat = GL_RGB8;
		dataFormat = GL_RGB;
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &RendererID);
	glTextureStorage2D(RendererID, 1, renderFormat, Width, Height);

	glTextureParameteri(RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureSubImage2D(RendererID, 0, 0, 0, Width, Height, dataFormat, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

GLTexture2D::~GLTexture2D() {
	glDeleteTextures(1, &RendererID);
}

void GLTexture2D::Bind(uint32_t slot) const {
	glBindTextureUnit(slot, RendererID);
}

}

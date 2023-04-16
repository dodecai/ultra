module;

#include <glad/gl.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

module Ultra.Platform.Renderer.GLTexture;

namespace Ultra {

GLTexture::GLTexture(const void *data, uint32_t width, uint32_t height, TextureFormat format): Texture(data, width, height, format) {
    
    const char *filename = "";
    glGenTextures(1, &mTextureID);
    glBindTexture(GL_TEXTURE_2D, mTextureID);

    int iwidth, iheight, channels;
    unsigned char *idata = stbi_load(filename, &iwidth, &iheight, &channels, 0);
    if (data) {
        GLenum format;
        if (channels == 1)
            format = GL_RED;
        else if (channels == 3)
            format = GL_RGB;
        else
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, iwidth, iheight, 0, format, GL_UNSIGNED_BYTE, idata);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // Free image data
        stbi_image_free(idata);

        std::cout << "Texture loaded successfully: " << filename << std::endl;
    } else {
        std::cout << "Failed to load texture: " << filename << std::endl;
    }
}

GLTexture::~GLTexture() {}


void GLTexture::Bind() const {
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
}

void GLTexture::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

}

module;

#include <glad/gl.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

module Ultra.Platform.Engine.GLTexture;

namespace Ultra {

static GLenum ConvertTextureFormat(TextureFormat format) {
    switch (format) {
        case TextureFormat::Red:    { return GL_RED; }
        case TextureFormat::Green:  { return GL_GREEN; }
        case TextureFormat::Blue:   { return GL_BLUE; }
        case TextureFormat::Alpha:  { return GL_ALPHA; }

        case TextureFormat::RGB:    { return GL_RGB; }
        case TextureFormat::BGR:    { return GL_BGR; }

        case TextureFormat::RGBA:   { return GL_RGBA; }
        case TextureFormat::BGRA:   { return GL_BGRA; }

        default: {
            LogFatal("[Ultra::Engine::GLTexture2D::ConvertTextureFormat]: ", "The specified texture format is currently not supported!");
            return 0x0;
        }
    }
}

/*
 * Texture2D
*/
// Default

GLTexture2D::GLTexture2D(const TextureProperties &properties): mProperties(properties) {
    RenderFormat = GL_RGBA8;

    glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
    glTextureStorage2D(mRendererID, 1, RenderFormat, mProperties.Width, mProperties.Height);

    glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLenum wrapType = mProperties.SamplerWrap == TextureWrap::Clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT;
    glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_R, wrapType);
    glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, wrapType);
    glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, wrapType);
    glTextureParameterf(mRendererID, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::GetCapabilities().MaxAnisotropy);

    glTexImage2D(GL_TEXTURE_2D, 0, ConvertTextureFormat(mProperties.Format), mProperties.Width, mProperties.Height, 0, ConvertTextureFormat(mProperties.Format), GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLTexture2D::GLTexture2D(const TextureProperties &properties, const void *data, size_t size): mProperties(properties) {
    RenderFormat = GL_RGBA8;

    glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
    glTextureStorage2D(mRendererID, 1, RenderFormat, mProperties.Width, mProperties.Height);

    glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLenum wrapType = mProperties.SamplerWrap == TextureWrap::Clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT;
    glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_R, wrapType);
    glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, wrapType);
    glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, wrapType);
    glTextureParameterf(mRendererID, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::GetCapabilities().MaxAnisotropy);

    glTexImage2D(GL_TEXTURE_2D, 0, ConvertTextureFormat(mProperties.Format), mProperties.Width, mProperties.Height, 0, ConvertTextureFormat(mProperties.Format), GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLTexture2D::GLTexture2D(const TextureProperties &properties, const string &path): mProperties(properties) {
    int channels = 0;
    int height = 0;
    int width = 0;

    void *data = nullptr;
    //stbi_set_flip_vertically_on_load(1);
    if (stbi_is_hdr(path.c_str())) {
        data = (float*)stbi_loadf(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if (!data) return;
        
        mProperties.Format = TextureFormat::RGBA32F;
    } else {
        data = (stbi_uc *)stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if (!data) return;
        if (channels == 4) {
            RenderFormat = GL_RGBA16;
            mProperties.Format = TextureFormat::RGBA;
        } else if (channels == 3) {
            RenderFormat = GL_RGB16;
            mProperties.Format = TextureFormat::RGB;
        }
    }
    mProperties.Height = height;
    mProperties.Width = width;

    //glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
    //glTextureStorage2D(mRendererID, 1, RenderFormat, Width, Height);

    //glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_R, GL_REPEAT);
    //glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //glTextureSubImage2D(mRendererID, 0, 0, 0, Width, Height, DataFormat, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

GLTexture2D::~GLTexture2D() {
    glDeleteTextures(1, &mRendererID);
}


void GLTexture2D::Bind(uint32_t slot) const {
    glBindTextureUnit(slot, mRendererID);
}

void GLTexture2D::Unbind(uint32_t slot) const {
    glBindTextureUnit(slot, 0);
}

// Mutators
//void GLTexture2D::SetData(void *data, uint32_t size) const {
//    // ToDo: Thread Sync (Lock, Unlock)
//    uint32_t bpc = DataFormat == GL_RGB8 ? 4 : 3;
//    //if (size == (Width * Height * bpc)) {
//    //	//Data must be entire texture!
//    //}
//    glTextureSubImage2D(mRendererID, 0, 0, 0, Width, Height, DataFormat, GL_UNSIGNED_BYTE, data);
//}



/*
 * Texture3D
*/
// Default

GLTexture3D::GLTexture3D(const TextureProperties &properties): mProperties(properties) {
    RenderFormat = GL_RGBA8;

    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &mRendererID);
    glTextureStorage2D(mRendererID, 1, ConvertTextureFormat(mProperties.Format), mProperties.Width, mProperties.Height);

    glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLenum wrapType = mProperties.SamplerWrap == TextureWrap::Clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT;
    glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_R, wrapType);
    glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, wrapType);
    glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, wrapType);
    glTextureParameterf(mRendererID, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::GetCapabilities().MaxAnisotropy);
}

GLTexture3D::GLTexture3D(const TextureProperties &properties, const void *data, size_t size): mProperties(properties) {
    RenderFormat = GL_RGBA8;

    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &mRendererID);
    glTextureStorage2D(mRendererID, 1, ConvertTextureFormat(mProperties.Format), mProperties.Width, mProperties.Height);

    glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLenum wrapType = mProperties.SamplerWrap == TextureWrap::Clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT;
    glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_R, wrapType);
    glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, wrapType);
    glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, wrapType);
    glTextureParameterf(mRendererID, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::GetCapabilities().MaxAnisotropy);
}

GLTexture3D::GLTexture3D(const TextureProperties &properties, const string &path): mProperties(properties) {
    int channels = 0;
    int height = 0;
    int width = 0;

    stbi_uc *data = nullptr;
    if (stbi_is_hdr(path.c_str())) {

    } else {
        stbi_set_flip_vertically_on_load(false);
        //stbi_set_flip_vertically_on_load(false);
        //data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        //if (!data) return;

        //Width = width;
        //Height = height;

        //if (channels == 4) {
        //    RenderFormat = GL_RGBA16;
        //    DataFormat = GL_RGBA;
        //} else if (channels == 3) {
        //    RenderFormat = GL_RGB16;
        //    DataFormat = GL_RGB;
        //}
        data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    }


    //glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
    //glTextureStorage2D(mRendererID, 1, RenderFormat, Width, Height);

    //glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_R, GL_REPEAT);
    //glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //glTextureSubImage2D(mRendererID, 0, 0, 0, Width, Height, DataFormat, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

GLTexture3D::~GLTexture3D() {
    glDeleteTextures(1, &mRendererID);
}


void GLTexture3D::Bind(uint32_t slot) const {
    glBindTextureUnit(slot, mRendererID);
}

void GLTexture3D::Unbind(uint32_t slot) const {
    glBindTextureUnit(slot, 0);
}


// Mutators
//void GLTexture3D::SetData(void *data, size_t size) const {
//    // ToDo: Thread Sycs (Lock, Unlock)
//    uint32_t bpc = DataFormat == GL_RGB8 ? 4 : 3;
//    //if (size == (Width * Height * bpc)) {
//    //	//Data must be entire texture!
//    //}
//    glTextureSubImage2D(mRendererID, 0, 0, 0, Width, Height, DataFormat, GL_UNSIGNED_BYTE, data);
//}
//

}

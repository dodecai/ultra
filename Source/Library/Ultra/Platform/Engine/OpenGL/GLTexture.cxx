module;

#include <glad/gl.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

module Ultra.Platform.Renderer.GLTexture;

namespace Ultra {

// Helpers
static GLenum ConvertTextureFormat(TextureFormat format) {
    switch (format) {
        case TextureFormat::Blue:       { return GL_BLUE; }
        case TextureFormat::Green:      { return GL_GREEN; }
        case TextureFormat::Red:        { return GL_RED; }
        case TextureFormat::Alpha:      { return GL_ALPHA; }
        
        case TextureFormat::BGR:        { return GL_BGR; }
        case TextureFormat::RGB:        { return GL_RGB; }
             
        case TextureFormat::BGRA:       { return GL_BGRA; }
        case TextureFormat::RGBA:       { return GL_RGBA; }
        case TextureFormat::RGBA16F:    { return GL_RGBA16F; }
        case TextureFormat::RGBA32F:    { return GL_RGBA32F; }
                                   
        case TextureFormat::Depth:      { return GL_DEPTH; }
        case TextureFormat::Stencil:    { return GL_STENCIL; }
        default: {
            LogFatal("[Ultra::Engine::GLTexture::ConvertTextureFormat]: ", "The specified texture format is currently not supported!");
            return 0x0;
        }
    }
}

namespace Helpers {

inline GLenum OpenGLTextureFormat(TextureFormat format) {
    switch (format) {
        case TextureFormat::RGB:     return GL_RGB;
        case TextureFormat::RGBA:
        case TextureFormat::RGBA16F:
        case TextureFormat::RGBA32F: return GL_RGBA;
    }
    return 0;
}

inline GLenum OpenGLImageInternalFormat(TextureFormat format) {
    switch (format) {
        case TextureFormat::RGB:             return GL_RGB8;
        case TextureFormat::RGBA:            return GL_RGBA8;
        case TextureFormat::RGBA16F:         return GL_RGBA16F;
        case TextureFormat::RGBA32F:         return GL_RGBA32F;
        case TextureFormat::DEPTH24STENCIL8: return GL_DEPTH24_STENCIL8;
        case TextureFormat::DEPTH32F:        return GL_DEPTH_COMPONENT32F;
    }
    AppAssert(false, "Unknown image format");
    return 0;
}

inline GLenum OpenGLFormatDataType(TextureFormat format) {
    switch (format) {
        case TextureFormat::RGB:
        case TextureFormat::RGBA:    return GL_UNSIGNED_BYTE;
        case TextureFormat::RGBA16F:
        case TextureFormat::RGBA32F: return GL_FLOAT;
    }
    AppAssert(false, "Unknown image format");
    return 0;
}

inline GLenum OpenGLSamplerWrap(TextureWrap wrap) {
    switch (wrap) {
        case TextureWrap::Clamp:   return GL_CLAMP_TO_EDGE;
        case TextureWrap::Repeat:  return GL_REPEAT;
    }
    AppAssert(false, "Unknown wrap mode");
    return 0;
}

// Note: should always be called with mipmap = false for magnification filtering
inline GLenum OpenGLSamplerFilter(TextureFilter filter, bool mipmap) {
    switch (filter) {
        case TextureFilter::Linear:   return mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
        case TextureFilter::Nearest:  return mipmap ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
    }
    AppAssert(false, "Unknown filter");
    return 0;
}

}


GLTexture::GLTexture(const TextureProperties &properties, const void *data, size_t size): Texture(properties, data, size) {
    auto renderFormat = GL_RGBA8;
    if (properties.Dimension == TextureDimension::Texture2D) {
        glCreateTextures(GL_TEXTURE_2D, 1, &mTextureID);
        glTextureStorage2D(mTextureID, 1, renderFormat, mProperties.Width, mProperties.Height);

        glTextureParameteri(mTextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(mTextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        GLenum wrapType = mProperties.SamplerWrap == TextureWrap::Clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT;
        glTextureParameteri(mTextureID, GL_TEXTURE_WRAP_R, wrapType);
        glTextureParameteri(mTextureID, GL_TEXTURE_WRAP_S, wrapType);
        glTextureParameteri(mTextureID, GL_TEXTURE_WRAP_T, wrapType);
        //glTextureParameterf(mTextureID, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::GetCapabilities().MaxAnisotropy);
    } else {
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &mTextureID);
        glTextureStorage2D(mTextureID, 1, ConvertTextureFormat(mProperties.Format), mProperties.Width, mProperties.Height);

        glTextureParameteri(mTextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(mTextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        GLenum wrapType = mProperties.SamplerWrap == TextureWrap::Clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT;
        glTextureParameteri(mTextureID, GL_TEXTURE_WRAP_R, wrapType);
        glTextureParameteri(mTextureID, GL_TEXTURE_WRAP_S, wrapType);
        glTextureParameteri(mTextureID, GL_TEXTURE_WRAP_T, wrapType);
        //glTextureParameterf(mTextureID, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::GetCapabilities().MaxAnisotropy);
    }
}

GLTexture::GLTexture(const TextureProperties &properties, string &path): Texture(properties, nullptr, 0) {
    int channels = {};
    int height = {};
    int width = {};
    void *data = nullptr;
    GLenum renderFormat = {};

    if (properties.Dimension == TextureDimension::Texture2D) {
        if (stbi_is_hdr(path.c_str())) {
            data = (float *)stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
            mProperties.Format = TextureFormat::RGBA32F;
        } else {
            data = (stbi_uc *)stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
            if (channels == 4) {
                mProperties.Format = TextureFormat::RGBA;
                renderFormat = GL_RGBA16;
            } else if (channels == 3) {
                mProperties.Format = TextureFormat::RGB;
                renderFormat = GL_RGB16;
            } else if (channels == 1) {
                renderFormat = GL_RED;
            }
        }
        mProperties.Height = height;
        mProperties.Width = width;

        if (data) {
            glCreateTextures(GL_TEXTURE_2D, 1, &mTextureID);
            glTextureStorage2D(mTextureID, 1, renderFormat, width, height);

            // Set texture parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTextureSubImage2D(mTextureID, 0, 0, 0, width, height, ConvertTextureFormat(mProperties.Format), GL_UNSIGNED_BYTE, data);
            //glGenerateMipmap(GL_TEXTURE_2D);

            // Free image data
            stbi_image_free(data);

            std::cout << "Texture loaded successfully: " << path << std::endl;
        } else {
            std::cout << "Failed to load texture: " << path << std::endl;
        }
    } else {
        data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (channels == 4) {
            mProperties.Format = TextureFormat::RGBA;
            renderFormat = GL_RGBA16;
        } else if (channels == 3) {
            mProperties.Format = TextureFormat::RGB;
            renderFormat = GL_RGB16;
        }
        mProperties.Height = height;
        mProperties.Width = width;

        if (data) {
            glCreateTextures(GL_TEXTURE_2D, 1, &mTextureID);
            glTextureStorage2D(mTextureID, 1, renderFormat, width, height);

            // Set texture parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTextureSubImage2D(mTextureID, 0, 0, 0, width, height, ConvertTextureFormat(mProperties.Format), GL_UNSIGNED_BYTE, data);
        }
    }
}

GLTexture::~GLTexture() {
    glDeleteTextures(1, &mTextureID);
}


void GLTexture::Bind(uint32_t slot) const {
    glBindTextureUnit(slot, mTextureID);
    //glActiveTexture(GL_TEXTURE0 + 0);
    //glBindTexture(GL_TEXTURE_2D, mTextureID);
}

void GLTexture::Unbind(uint32_t slot) const {
    glBindTextureUnit(slot, 0);
    //glBindTexture(GL_TEXTURE_2D, 0);
}



//void GLImage::Invalidate() {
//    if (m_RendererID) Release();
//
//    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
//
//    GLenum internalFormat = Utils::OpenGLImageInternalFormat(m_Specification.Format);
//    uint32_t mipCount = Utils::CalculateMipCount(m_Width, m_Height);
//    glTextureStorage2D(m_RendererID, mipCount, internalFormat, m_Width, m_Height);
//    if (m_ImageData) {
//        GLenum format = Utils::OpenGLImageFormat(m_Specification.Format);
//        GLenum dataType = Utils::OpenGLFormatDataType(m_Specification.Format);
//        //glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, format, dataType, m_ImageData.Data);
//        glGenerateTextureMipmap(m_RendererID); // TODO: optional
//    }
//}

//void GLImage::CreateSampler(TextureProperties properties) {
//    glCreateSamplers(1, &m_SamplerRendererID);
//    glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_MIN_FILTER, Utils::OpenGLSamplerFilter(properties.SamplerFilter, properties.GenerateMips));
//    glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_MAG_FILTER, Utils::OpenGLSamplerFilter(properties.SamplerFilter, false));
//    glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_WRAP_R, Utils::OpenGLSamplerWrap(properties.SamplerWrap));
//    glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_WRAP_S, Utils::OpenGLSamplerWrap(properties.SamplerWrap));
//    glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_WRAP_T, Utils::OpenGLSamplerWrap(properties.SamplerWrap));
//}


}

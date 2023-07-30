module;

#include <glad/gl.h>

#pragma warning(push)
#pragma warning(disable:4242)
#pragma warning(disable:4244)
//https://github.com/nothings/stb/issues/334
#ifndef STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_IMPLEMENTATION
#endif
#define STB_IMAGE_STATIC
#include <stb/stb_image.h>
//#define STB_IMAGE_WRITE_IMPLEMENTATION
#pragma warning(pop)

module Ultra.Platform.Renderer.GLTexture;

namespace Ultra {

// Helpers
static GLenum ConvertTextureFormat(TextureFormat format) {
    switch (format) {
        case TextureFormat::R8: return GL_R8;
        case TextureFormat::R16: return GL_R16;
        case TextureFormat::R16F: return GL_R16F;
        case TextureFormat::R32F: return GL_R32F;
        case TextureFormat::RG8: return GL_RG8;
        case TextureFormat::RG16: return GL_RG16;
        case TextureFormat::RG16F: return GL_RG16F;
        case TextureFormat::RG32F: return GL_RG32F;
        case TextureFormat::RGB8: return GL_RGB8;
        case TextureFormat::RGBA8: return GL_RGBA8;
        case TextureFormat::RGBA16: return GL_RGBA16;
        case TextureFormat::RGBA16F: return GL_RGBA16F;
        case TextureFormat::RGBA32F: return GL_RGBA32F;
        case TextureFormat::Depth16: return GL_DEPTH_COMPONENT16;
        case TextureFormat::Depth24: return GL_DEPTH_COMPONENT24;
        case TextureFormat::Depth32F: return GL_DEPTH_COMPONENT32F;
        default: {
            LogFatal("[Ultra::Engine::GLTexture::ConvertTextureFormat]: ", "The specified texture format is currently not supported!");
            return 0x0;
        }
    }
}

//GLint GetGLDataFormat(TextureDataFormat format) {
//    switch (format) {
//        case TextureDataFormat::U8: return GL_UNSIGNED_BYTE;
//        case TextureDataFormat::I8: return GL_BYTE;
//        case TextureDataFormat::U16: return GL_UNSIGNED_SHORT;
//        case TextureDataFormat::I16: return GL_SHORT;
//        case TextureDataFormat::U32: return GL_UNSIGNED_INT;
//        case TextureDataFormat::I32: return GL_INT;
//        case TextureDataFormat::Float: return GL_FLOAT;
//    }
//}

//GLint GetGLPixelFormat(PhxPixelFormat format) {
//    switch (format) {
//        case PhxPixelFormat::Red: return GL_RED;
//        case PhxPixelFormat::RG: return GL_RG;
//        case PhxPixelFormat::RGB: return GL_RGB;
//        case PhxPixelFormat::BGR: return GL_BGR;
//        case PhxPixelFormat::RGBA: return GL_RGBA;
//        case PhxPixelFormat::BGRA: return GL_BGRA;
//        case PhxPixelFormat::DepthComponent: return GL_DEPTH_COMPONENT;
//    }
//}

namespace Helpers {

void CreateSampler(TextureProperties properties) {
    //glCreateSamplers(1, &mTextureID);
    //glSamplerParameteri(mTextureID, GL_TEXTURE_MIN_FILTER, GLSamplerFilter(properties.SamplerFilter, properties.GenerateMips));
    //glSamplerParameteri(mTextureID, GL_TEXTURE_MAG_FILTER, GLSamplerFilter(properties.SamplerFilter, false));
    //glSamplerParameteri(mTextureID, GL_TEXTURE_WRAP_R, GLSamplerWrap(properties.SamplerWrap));
    //glSamplerParameteri(mTextureID, GL_TEXTURE_WRAP_S, GLSamplerWrap(properties.SamplerWrap));
    //glSamplerParameteri(mTextureID, GL_TEXTURE_WRAP_T, GLSamplerWrap(properties.SamplerWrap));
}

inline GLenum GLTextureFormat(TextureFormat format) {
    switch (format) {
        case TextureFormat::RGB8:    return GL_RGB;
        case TextureFormat::RGBA8:
        case TextureFormat::RGBA16F:
        case TextureFormat::RGBA32F: return GL_RGBA;
    }
    return 0;
}

inline GLenum GLImageInternalFormat(TextureFormat format) {
    switch (format) {
        case TextureFormat::RGB8:           return GL_RGB8;
        case TextureFormat::RGBA8:          return GL_RGBA8;
        case TextureFormat::RGBA16F:        return GL_RGBA16F;
        case TextureFormat::RGBA32F:        return GL_RGBA32F;
        case TextureFormat::Dept24Stencil8: return GL_DEPTH24_STENCIL8;
        case TextureFormat::Depth32F:       return GL_DEPTH_COMPONENT32F;
    }
    AppAssert(false, "Unknown image format");
    return 0;
}

inline GLenum GLImageFormat(TextureFormat format) {
    switch (format) {
        case TextureFormat::RGB8:            return GL_RGB;
        case TextureFormat::RGBA8:           return GL_RGBA;
    }
    AppAssert(false, "Unknown image format");
    return 0;
}

inline GLenum GLFormatDataType(TextureFormat format) {
    switch (format) {
        case TextureFormat::RGB8:
        case TextureFormat::RGBA8:    return GL_UNSIGNED_BYTE;
        case TextureFormat::RGBA16F:
        case TextureFormat::RGBA32F: return GL_FLOAT;
    }
    AppAssert(false, "Unknown image format");
    return 0;
}

inline GLenum GLSamplerWrap(TextureWrap wrap) {
    switch (wrap) {
        case TextureWrap::Clamp:        return GL_CLAMP_TO_EDGE;
        case TextureWrap::MirrorClamp:  return GL_MIRROR_CLAMP_TO_EDGE;
        case TextureWrap::MirrorRepeat: return GL_MIRRORED_REPEAT;
        case TextureWrap::Repeat:       return GL_REPEAT;
    }
    AppAssert(false, "Unknown wrap mode");
    return 0;
}

// Note: should always be called with mipmap = false for magnification filtering
inline GLenum GLSamplerFilter(TextureFilter filter, bool mipmap) {
    switch (filter) {
        case TextureFilter::Linear:             return mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
        case TextureFilter::LinearMipPoint:     return GL_LINEAR_MIPMAP_NEAREST;
        case TextureFilter::LinearMipLinear:    return GL_LINEAR_MIPMAP_LINEAR;
        case TextureFilter::Nearest:            return mipmap ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
        case TextureFilter::Point:              return GL_NEAREST;
        case TextureFilter::PointMipPoint:      return GL_NEAREST_MIPMAP_NEAREST;
        case TextureFilter::PointMipLinear:     return GL_NEAREST_MIPMAP_LINEAR;
    }
    AppAssert(false, "Unknown filter");
    return 0;
}

}


GLTexture::GLTexture(const TextureProperties &properties, const void *data, size_t size): Texture(properties, data, size) {
    if (properties.Width == 0 && properties.Height == 0) {
        glCreateTextures(GL_TEXTURE_1D, 1, &mTextureID);
        return;
    }
    switch (properties.Dimension) {
        case TextureDimension::Texture1D: {
            glCreateTextures(GL_TEXTURE_1D, 1, &mTextureID);
            break;
        }
        case TextureDimension::Texture2D: {
            glCreateTextures(GL_TEXTURE_2D, 1, &mTextureID);
            glTextureStorage2D(mTextureID, 1, ConvertTextureFormat(mProperties.Format), mProperties.Width, mProperties.Height);
            // ToDo: Allow to setup data type (was GL_UNSIGNED_BYTE)
            glTextureSubImage2D(mTextureID, 0, 0, 0, mProperties.Width, mProperties.Height, Helpers::GLImageFormat(mProperties.Format), GL_FLOAT, data);
            break;
        }
        case TextureDimension::Texture3D: {
            glCreateTextures(GL_TEXTURE_3D, 1, &mTextureID);
            glTextureStorage2D(mTextureID, 1, ConvertTextureFormat(mProperties.Format), mProperties.Width, mProperties.Height);
            break;

        }
        case TextureDimension::TextureCube: {
            glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &mTextureID);
            glTextureStorage2D(mTextureID, 1, ConvertTextureFormat(mProperties.Format), mProperties.Width, mProperties.Height);
            break;
        }
    }

    GLenum wrapType = mProperties.SamplerWrap == TextureWrap::Clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT;
    glTextureParameteri(mTextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(mTextureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTextureParameteri(mTextureID, GL_TEXTURE_WRAP_R, wrapType);
    glTextureParameteri(mTextureID, GL_TEXTURE_WRAP_S, wrapType);
    glTextureParameteri(mTextureID, GL_TEXTURE_WRAP_T, wrapType);
    glTextureParameterf(mTextureID, GL_TEXTURE_MAX_ANISOTROPY, 16); // ToDo: RenderDevice::GetCapabilities().MaxAnisotropy

    //glGenerateMipmap(GL_TEXTURE_2D);
}

GLTexture::GLTexture(const TextureProperties &properties, const string &path): Texture(properties, nullptr, 0) {
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
                mProperties.Format = TextureFormat::RGBA8;
                renderFormat = GL_RGBA16;
            } else if (channels == 3) {
                mProperties.Format = TextureFormat::RGB8;
                renderFormat = GL_RGB16;
            } else if (channels == 1) {
                renderFormat = GL_RGBA16;
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
            mProperties.Format = TextureFormat::RGBA8;
            renderFormat = GL_RGBA16;
        } else if (channels == 3) {
            mProperties.Format = TextureFormat::RGB8;
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

void GLTexture::Draw(int index, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1) {
    glEnable(GL_TEXTURE_2D);
    Bind(index);
    glBegin(GL_QUADS);
    glTexCoord2f(u0, v0); glVertex2f(x0, y0); // 0, 0
    glTexCoord2f(u0, v1); glVertex2f(x0, y1); // 0, 1
    glTexCoord2f(u1, v1); glVertex2f(x1, y1); // 1, 1
    glTexCoord2f(u1, v0); glVertex2f(x1, y0); // 1, 0
    glEnd();
    Unbind(index);
    glDisable(GL_TEXTURE_2D);
    return;


    GLfloat vertices[] = {
        // Positionen  // Texturkoordinaten
        x0, y0, 0.0f,  u0, v0, // Unten links
        x1, y0, 0.0f,  u1, v0, // Unten rechts
        x1, y1, 0.0f,  u1, v1, // Oben rechts
        x0, y1, 0.0f,  u0, v1  // Oben links
    };

    // Definieren Sie die Reihenfolge, in der die Eckpunkte gezeichnet werden
    GLuint indices[] = { 0, 1, 2, 2, 3, 0 };

    // Erstellen Sie einen Vertex-Array-Object (VAO)
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    // Erstellen Sie einen Vertex-Buffer-Object (VBO)
    GLuint VBO;
    glGenBuffers(1, &VBO);

    // Erstellen Sie einen Element-Buffer-Object (EBO)
    GLuint EBO;
    glGenBuffers(1, &EBO);

    // Binden Sie den VAO
    glBindVertexArray(VAO);

    // Laden Sie die Vertices-Daten in den VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Laden Sie die Indizes-Daten in den EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Definieren Sie die Attributpointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Zeichnen Sie das Quad
    Bind(GL_TEXTURE_2D);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    Unbind(GL_TEXTURE_2D);
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

}

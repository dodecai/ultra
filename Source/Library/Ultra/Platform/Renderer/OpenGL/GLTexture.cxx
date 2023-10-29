module Ultra.Platform.Renderer.GLTexture;

import <glad/gl.h>;

#pragma warning(push, 0)
//https://github.com/nothings/stb/issues/334
#ifndef STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_IMPLEMENTATION
#endif
#define STB_IMAGE_STATIC
import <stb/stb_image.h>;
//#define STB_IMAGE_WRITE_IMPLEMENTATION
#pragma warning(pop)

import Ultra.System.FileSystem;

namespace Ultra {

// Helpers
void GLCreateSampler(TextureProperties properties) {
    //glCreateSamplers(1, &mTextureID);
    //glSamplerParameteri(mTextureID, GL_TEXTURE_MIN_FILTER, GLSamplerFilter(properties.SamplerFilter, properties.GenerateMips));
    //glSamplerParameteri(mTextureID, GL_TEXTURE_MAG_FILTER, GLSamplerFilter(properties.SamplerFilter, false));
    //glSamplerParameteri(mTextureID, GL_TEXTURE_WRAP_R, GLSamplerWrap(properties.SamplerWrap));
    //glSamplerParameteri(mTextureID, GL_TEXTURE_WRAP_S, GLSamplerWrap(properties.SamplerWrap));
    //glSamplerParameteri(mTextureID, GL_TEXTURE_WRAP_T, GLSamplerWrap(properties.SamplerWrap));
}

inline GLenum GLFormatDataType(TextureFormat format) {
    switch (format) {
        case TextureFormat::R8:
        case TextureFormat::RGB8:
        case TextureFormat::RGBA8:    return GL_UNSIGNED_BYTE;
        case TextureFormat::RGBA16F:
        case TextureFormat::RGBA32F:  return GL_FLOAT;
//        case TextureDataFormat::U8: return GL_UNSIGNED_BYTE;
//        case TextureDataFormat::I8: return GL_BYTE;
//        case TextureDataFormat::U16: return GL_UNSIGNED_SHORT;
//        case TextureDataFormat::I16: return GL_SHORT;
//        case TextureDataFormat::U32: return GL_UNSIGNED_INT;
//        case TextureDataFormat::I32: return GL_INT;
//        case TextureDataFormat::Float: return GL_FLOAT;
    }
    AppAssert(false, "Unknown image format");
    return 0;
}

static GLenum GLImageInternalFormat(TextureFormat format) {
    switch (format) {
        case TextureFormat::R8:             return GL_R8;
        case TextureFormat::R16:            return GL_R16;
        case TextureFormat::R16F:           return GL_R16F;
        case TextureFormat::R32F:           return GL_R32F;
        case TextureFormat::RG8:            return GL_RG8;
        case TextureFormat::RG16:           return GL_RG16;
        case TextureFormat::RG16F:          return GL_RG16F;
        case TextureFormat::RG32F:          return GL_RG32F;
        case TextureFormat::RGB8:           return GL_RGB8;
        case TextureFormat::RGBA8:          return GL_RGBA8;
        case TextureFormat::RGBA16:         return GL_RGBA16;
        case TextureFormat::RGBA16F:        return GL_RGBA16F;
        case TextureFormat::RGBA32F:        return GL_RGBA32F;
        case TextureFormat::Depth16:        return GL_DEPTH_COMPONENT16;
        case TextureFormat::Depth24:        return GL_DEPTH_COMPONENT24;
        case TextureFormat::Dept24Stencil8: return GL_DEPTH24_STENCIL8;
        case TextureFormat::Depth32F:       return GL_DEPTH_COMPONENT32F;
//        case PhxPixelFormat::Red: return GL_RED;
//        case PhxPixelFormat::RG: return GL_RG;
//        case PhxPixelFormat::RGB: return GL_RGB;
//        case PhxPixelFormat::BGR: return GL_BGR;
//        case PhxPixelFormat::RGBA: return GL_RGBA;
//        case PhxPixelFormat::BGRA: return GL_BGRA;
//        case PhxPixelFormat::DepthComponent: return GL_DEPTH_COMPONENT;
        default: {
            LogFatal("The specified texture format is currently not supported!");
            return 0x0;
        }
    }
}

inline GLenum GLImageFormat(TextureFormat format) {
    switch (format) {
        case TextureFormat::R8:      return GL_RED;
        case TextureFormat::RGB8:    return GL_RGB;
        case TextureFormat::RGBA8:
        case TextureFormat::RGBA16F:
        case TextureFormat::RGBA32F: return GL_RGBA;
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



GLTexture::GLTexture(const TextureProperties &properties, const void *data, size_t size): Texture(properties, data, size) {
    if (properties.Width == 0 && properties.Height == 0) {
        glCreateTextures(GL_TEXTURE_1D, 1, &mTextureID);
        return;
    }
    switch (properties.Dimension) {
        case TextureDimension::Texture1D: {
            glCreateTextures(GL_TEXTURE_1D, 1, &mTextureID);
            glTextureStorage1D(mTextureID, 1, GLImageInternalFormat(mProperties.Format), mProperties.Width);
            break;
        }
        case TextureDimension::Texture2D: {
            glCreateTextures(GL_TEXTURE_2D, 1, &mTextureID);
            glTextureStorage2D(mTextureID, 1, GLImageInternalFormat(mProperties.Format), mProperties.Width, mProperties.Height);
            glTextureSubImage2D(mTextureID, 0, 0, 0, mProperties.Width, mProperties.Height, GLImageFormat(mProperties.Format), GLFormatDataType(properties.Format), data);
            break;
        }
        case TextureDimension::Texture3D: {
            glCreateTextures(GL_TEXTURE_3D, 1, &mTextureID);
            glTextureStorage2D(mTextureID, 1, GLImageInternalFormat(mProperties.Format), mProperties.Width, mProperties.Height);
            break;

        }
        case TextureDimension::TextureCube: {
            glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &mTextureID);
            glTextureStorage2D(mTextureID, 1, GLImageInternalFormat(mProperties.Format), mProperties.Width, mProperties.Height);
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
    // Properties
    void *data = nullptr;
    int width {};
    int height {};
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Automatic detection of Cube Textures
    if (Directory::Exists(path)) {
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &mTextureID);
        auto images = Directory::GetFiles(path, "Right|Left|Top|Bottom|Front|Back|PositiveX|NegativeX|PositiveY|NegativeY|PositiveZ|NegativeZ");

        bool first = false;
        for (auto &&image : images) {
            if (Load(image, data, width, height)) {
                auto name = File::GetName(image);

                int index = 0;
                if (name == "Right" || name == "PositiveX")        { index = GL_TEXTURE_CUBE_MAP_POSITIVE_X; }
                else if (name == "Left" || name == "NegativeX")    { index = GL_TEXTURE_CUBE_MAP_NEGATIVE_X; }
                else if (name == "Top" || name == "PositiveY")     { index = GL_TEXTURE_CUBE_MAP_POSITIVE_Y; }
                else if (name == "Bottom" || name == "NegativeY")  { index = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y; }
                else if (name == "Front" || name == "PositiveZ")   { index = GL_TEXTURE_CUBE_MAP_POSITIVE_Z; }
                else if (name == "Back" || name == "NegativeZ")    { index = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; }

                if (!first) glTextureStorage2D(mTextureID, 1, GLImageInternalFormat(mProperties.Format), width, height);
                first = true;

                glTextureSubImage3D(mTextureID, 0, 0, 0, index, width, height, 1, GLImageFormat(mProperties.Format), GLFormatDataType(mProperties.Format), data);
                stbi_image_free(data);
                data = nullptr;

                // Set texture parameters
                // ToDo: Use value from Texture Properties
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                //glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
                //glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, { 1.0f, 1.0f, 0.0f, 1.0f });

                LogTrace("The image '{}' was loaded successfully.", image);
            } else {
                LogError("An error occurred while loading image '{}'!", image);
            }
        }

    } else if (File::Exists(path)) {
        if (properties.Dimension == TextureDimension::Texture2D) {
            glCreateTextures(GL_TEXTURE_2D, 1, &mTextureID);
            if (Load(path, data, width, height)) {
                glTextureStorage2D(mTextureID, 1, GLImageInternalFormat(mProperties.Format), width, height);

                glTextureSubImage2D(mTextureID, 0, 0, 0, width, height, GLImageFormat(mProperties.Format), GLFormatDataType(mProperties.Format), data);
                stbi_image_free(data);
                data = nullptr;

                // Set texture parameters
                // ToDo: Use value from Texture Properties
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                //glGenerateMipmap(GL_TEXTURE_2D);
                //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, { 1.0f, 1.0f, 0.0f, 1.0f });
                

                LogTrace("The image '{}' was loaded successfully.", path);
            } else {
                LogError("An error occurred while loading image '{}'!", path);
            }
        } else {
            LogError("The current dimension isn't implemented!");
        }
    } else {
        LogError("The specified directory/file '{}' doesn't exist!", path);
    }
}

GLTexture::~GLTexture() {
    glDeleteTextures(1, &mTextureID);
}


void GLTexture::Bind(uint32_t slot) const {
    glBindTextureUnit(slot, mTextureID);
}

void GLTexture::Unbind(uint32_t slot) const {
    glBindTextureUnit(slot, 0);
}

bool GLTexture::Load(const string &path, void *&data, int &width, int &height) {
    int channels = {};

    // Get Meta Data
    stbi_info(path.c_str(), &width, &height, &channels);
    // Load Image
    //stbi_set_flip_vertically_on_load(true); // OpenGL expects the 0.0 coordinate on the y-axis to be on the bottom side of the image
    if (stbi_is_hdr(path.c_str())) {
        data = (float *)stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
        mProperties.Format = TextureFormat::RGBA32F;
    } else {
        int requiredChannels = 0;
        switch (channels) {
            case 4: {
                requiredChannels = STBI_rgb_alpha;
                mProperties.Format = TextureFormat::RGBA8;
                break;
            }
            case 3: {
                requiredChannels = STBI_rgb;
                mProperties.Format = TextureFormat::RGB8;
                break;
            }
            case 2: {
                requiredChannels = STBI_grey_alpha;
                mProperties.Format = TextureFormat::RG8;
                break;
            }
            case 1: {
                requiredChannels = STBI_grey;
                mProperties.Format = TextureFormat::R8;
                break;
            }
            default: {
                return false;
            }
        }
        data = (stbi_uc *)stbi_load(path.c_str(), &width, &height, &channels, requiredChannels);
    }

    if (data) {
        mProperties.Height = height;
        mProperties.Width = width;
        return true;
    }
    return false;
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

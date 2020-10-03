#pragma once

#include "Ultra/Renderer/Texture.h"

namespace Ultra {

class VKTexture2D: public Texture2D {
    std::string Patch;
    uint32_t DataFormat; //GLenum
    uint32_t RenderFormat; //GLenum
    uint32_t Height;
    uint32_t Width;
    RendererID mRendererID;

public:
    // Default
    VKTexture2D(const string &path);
    VKTexture2D(const uint32_t width, const uint32_t height, TextureFormat format, TextureWrap wrap);
    virtual ~VKTexture2D();

    virtual void Bind(uint32_t slot) const override;
    virtual void UnBind(uint32_t slot) const override;

    // Accessors
    virtual TextureFormat GetFormat() const;
    virtual uint32_t GetHeight() const override { return Height; }
    virtual uint32_t GetWidth() const override { return Width; }
    virtual uint32_t GetRendererID() const override { return mRendererID; };

    // Mutators
    virtual void SetData(void *data, uint32_t size) const override;

    // Operators
    virtual bool operator==(const Texture &other) const override;
    virtual operator RendererID() const override;
};

class VKTexture3D: public Texture3D {
    std::string Patch;
    uint32_t DataFormat; //GLenum
    uint32_t RenderFormat; //GLenum
    uint32_t Height;
    uint32_t Width;
    RendererID mRendererID;

public:
    // Default
    VKTexture3D(const string &path);
    VKTexture3D(const uint32_t width, const uint32_t height, TextureFormat format, TextureWrap wrap);
    virtual ~VKTexture3D();

    virtual void Bind(uint32_t slot) const override;
    virtual void UnBind(uint32_t slot) const override;

    // Accessors
    virtual TextureFormat GetFormat() const;
    virtual uint32_t GetHeight() const override { return Height; }
    virtual uint32_t GetWidth() const override { return Width; }
    virtual uint32_t GetRendererID() const override { return mRendererID; };

    // Mutators
    virtual void SetData(void *data, uint32_t size) const override;

    // Operators
    virtual bool operator==(const Texture &other) const override;
    virtual operator RendererID() const override;
};

}

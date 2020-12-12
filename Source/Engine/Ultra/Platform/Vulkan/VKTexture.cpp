#include "VKTexture.h"

namespace Ultra {

VKTexture2D::VKTexture2D(const string &path) {}

VKTexture2D::VKTexture2D(const uint32_t width, const uint32_t height, TextureFormat format, TextureWrap wrap) {}

VKTexture2D::~VKTexture2D() {}

void VKTexture2D::Bind(uint32_t slot) const {}

void VKTexture2D::UnBind(uint32_t slot) const {}

TextureFormat VKTexture2D::GetFormat() const {
    return TextureFormat();
}

void VKTexture2D::SetData(void *data, uint32_t size) const {
}

bool VKTexture2D::operator==(const Texture &other) const {
    return false;
}

VKTexture2D::operator RendererID() const {
    return 0;
}

VKTexture3D::VKTexture3D(const string &path) {}

VKTexture3D::VKTexture3D(const uint32_t width, const uint32_t height, TextureFormat format, TextureWrap wrap) {}

VKTexture3D::~VKTexture3D() {}

void VKTexture3D::Bind(uint32_t slot) const {}

void VKTexture3D::UnBind(uint32_t slot) const {}

TextureFormat VKTexture3D::GetFormat() const {
    return TextureFormat();
}

void VKTexture3D::SetData(void *data, uint32_t size) const {
}

bool VKTexture3D::operator==(const Texture &other) const {
    return false;
}

VKTexture3D::operator RendererID() const {
    return 0;
}

}

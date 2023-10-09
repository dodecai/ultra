module Ultra.Platform.Renderer.VKTexture;

#pragma warning(push)
#pragma warning(disable: 4100)

namespace Ultra {

VKTexture::VKTexture(const TextureProperties &properties, const void *data, size_t size): Texture(properties, data, size) {}

VKTexture::VKTexture(const TextureProperties &properties, const string &path): Texture(properties, nullptr, 0) {}

VKTexture::~VKTexture() {}


void VKTexture::Bind(uint32_t slot) const {}

void VKTexture::Unbind(uint32_t slot) const {}

}

#pragma warning(pop)

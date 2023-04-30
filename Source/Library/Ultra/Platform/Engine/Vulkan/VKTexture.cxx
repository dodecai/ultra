module Ultra.Platform.Renderer.VKTexture;

namespace Ultra {

VKTexture::VKTexture(const TextureProperties &properties, const void *data, size_t size): Texture(properties, data, size) {}

VKTexture::VKTexture(const TextureProperties &properties, string &path): Texture(properties, nullptr, 0) {}

VKTexture::~VKTexture() {}


void VKTexture::Bind(uint32_t slot) const {}

void VKTexture::Unbind(uint32_t slot) const {}

}

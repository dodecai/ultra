module Ultra.Platform.Renderer.DXTexture;

namespace Ultra {

DXTexture::DXTexture(const TextureProperties &properties, const void *data, size_t size): Texture(properties, data, size) {}

DXTexture::DXTexture(const TextureProperties &properties, const string &path): Texture(properties, nullptr, 0) {}

DXTexture::~DXTexture() {}


void DXTexture::Bind(uint32_t slot) const {}

void DXTexture::Unbind(uint32_t slot) const {}

}

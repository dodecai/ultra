module Ultra.Platform.Renderer.SWTexture;

#pragma warning(push)
#pragma warning(disable: 4100)

namespace Ultra {

SWTexture::SWTexture(const TextureProperties &properties, const void *data, size_t size): Texture(properties, data, size) {}

SWTexture::SWTexture(const TextureProperties &properties, const string &path): Texture(properties, nullptr, 0) {}

SWTexture::~SWTexture() {}


void SWTexture::Bind(uint32_t slot) const {}

void SWTexture::Unbind(uint32_t slot) const {}

}

#pragma warning(pop)

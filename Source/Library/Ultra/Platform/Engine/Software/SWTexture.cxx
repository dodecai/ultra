module Ultra.Platform.Renderer.SWTexture;

namespace Ultra {

SWTexture::SWTexture(const void *data, uint32_t width, uint32_t height, TextureFormat format): Texture(data, width, height, format) {}

SWTexture::~SWTexture() {}


void SWTexture::Bind() const {}

void SWTexture::Unbind() const {}

}

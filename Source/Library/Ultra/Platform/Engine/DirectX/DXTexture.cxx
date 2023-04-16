module Ultra.Platform.Renderer.DXTexture;

namespace Ultra {

DXTexture::DXTexture(const void *data, uint32_t width, uint32_t height, TextureFormat format): Texture(data, width, height, format) {}

DXTexture::~DXTexture() {}


void DXTexture::Bind() const {}

void DXTexture::Unbind() const {}

}

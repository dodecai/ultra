module Ultra.Platform.Renderer.VKTexture;

namespace Ultra {

VKTexture::VKTexture(const void *data, uint32_t width, uint32_t height, TextureFormat format): Texture(data, width, height, format) {}

VKTexture::~VKTexture() {}


void VKTexture::Bind() const {}

void VKTexture::Unbind() const {}

}

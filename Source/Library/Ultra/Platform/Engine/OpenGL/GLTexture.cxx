module Ultra.Platform.Renderer.GLTexture;

namespace Ultra {

GLTexture::GLTexture(const void *data, uint32_t width, uint32_t height, TextureFormat format): Texture(data, width, height, format) {}

GLTexture::~GLTexture() {}


void GLTexture::Bind() const {}

void GLTexture::Unbind() const {}

}

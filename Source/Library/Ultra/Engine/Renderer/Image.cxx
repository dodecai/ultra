module Ultra.Engine.Image;

import Ultra.GFX.Context;
//import Ultra.Platform.Engine.GLImage;
//import Ultra.Platform.Engine.VKImage;

namespace Ultra {

Reference<Image> Image::Create(ImageSpecification specification, BufferData buffer) {
    switch (Context::API) {
        case GraphicsAPI::Null: return nullptr;
        //case GraphicsAPI::OpenGL: return Reference<GLImage>::Create(specification, buffer);
        //case GraphicsAPI::Vulkan: return Reference<VKImage>::Create(specification);
    }
    AppAssert(false, "Unknown RendererAPI");
    return nullptr;
}

Reference<Image> Image::Create(ImageSpecification specification, const void *data) {
    switch (Context::API) {
        case GraphicsAPI::Null: return nullptr;
        //case GraphicsAPI::OpenGL: return Reference<OpenGLImage2D>::Create(specification, data);
        //case GraphicsAPI::Vulkan: return Reference<VulkanImage2D>::Create(specification);
    }
    AppAssert(false, "Unknown RendererAPI");
    return nullptr;
}

}

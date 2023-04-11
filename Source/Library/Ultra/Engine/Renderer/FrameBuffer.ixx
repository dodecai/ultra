module;

#include <glm/glm.hpp>
#include <map>

export module Ultra.Engine.Framebuffer;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.Engine.Texture;

export namespace Ultra {

class Framebuffer;

struct FramebufferTextureSpecification {
    FramebufferTextureSpecification() = default;
    FramebufferTextureSpecification(TextureFormat format): Format(format) {}

    TextureFormat Format;
};

struct FramebufferAttachmentSpecification {
    FramebufferAttachmentSpecification() = default;
    FramebufferAttachmentSpecification(const std::initializer_list<FramebufferTextureSpecification> &attachments)
        : Attachments(attachments) {}

    std::vector<FramebufferTextureSpecification> Attachments;
};

struct FramebufferProperties {
    string Name;

    glm::vec4 ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
    uint32_t Height = 0;
    uint32_t Width = 0;
    float Scale = 1.0f;

    FramebufferAttachmentSpecification Attachments;
    uint32_t Samples = 1;

    bool NoResize = false;
    bool SwapChainTarget = false;

    // Will it be used for transfer ops?
    bool Transfer = false;

    //Reference<Image2D> ExistingImage;
    std::vector<uint32_t> ExistingImageLayers;
    //std::map<uint32_t, Reference<Image2D>> ExistingImages;

    // At the moment this will just create a new render pass
    // with an existing framebuffer
    Reference<Framebuffer> ExistingFramebuffer;
};


class Framebuffer {
public:
    Framebuffer() = default;
    ~Framebuffer() = default;

    static Reference<Framebuffer> Create(const FramebufferProperties &properties) { return {}; }

    virtual void Bind() = 0;
    virtual void BindTexture(uint32_t slot = 0) const = 0;
    virtual void Unbind() = 0;

    virtual const FramebufferProperties & GetProperties() const = 0;
    virtual uint32_t GetColorAttachmentRendererID() const = 0;
    virtual void *GetColorAttachmentRendererIDRaw() const { return nullptr; }
    virtual uint32_t GetDepthAttachmentRendererID() const = 0;
    virtual uint32_t GetRendererID() const = 0;

    virtual void Resize(uint32_t width, uint32_t height, bool reload = false) = 0;
};

class FramebufferPool {
public:
    FramebufferPool(uint32_t maxBuffers = 32);
    ~FramebufferPool();

    void Add(const Reference<Framebuffer> &framebuffer);
    weak_ptr<Framebuffer> Allocate();

    static FramebufferPool *GetGlobal() { return Instance; }
    const vector<Reference<Framebuffer>> &GetPool() const { return Pool; }


private:
    vector<Reference<Framebuffer>> Pool;
    static inline FramebufferPool *Instance = nullptr;
};

}

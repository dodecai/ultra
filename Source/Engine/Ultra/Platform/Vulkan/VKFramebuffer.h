#pragma once

#include "Ultra/Renderer/Framebuffer.h"

#include <vulkan/vulkan.hpp>

namespace Ultra {

class VKFramebuffer: public Framebuffer {
    struct FramebufferAttachment {
        vk::Image Image;
        vk::DeviceMemory Memory;
        vk::ImageView View;
    };

public:
    VKFramebuffer(const FramebufferProperties &properties);
    virtual ~VKFramebuffer();

    // Methods
    virtual void Bind() override;
    virtual void BindTexture(uint32_t slot = 0) const override {}
    virtual void Unbind() override;

    // Accessors
    virtual uint32_t GetRendererID() const override;
    virtual const FramebufferProperties &GetProperties() const override;
    virtual uint32_t GetColorAttachmentRendererID() const override;
    virtual uint32_t GetDepthAttachmentRendererID() const override;

    const vk::DescriptorImageInfo &GetNativeDescriptorInfo() const;
    vk::Framebuffer GetNativeFramebuffer() const;
    vk::RenderPass GetRenderPass() const;

    // Mutators
    virtual void Resize(uint32_t width, uint32_t height, bool reload) override;

private:
    FramebufferProperties Properties;

    uint32_t RendererID = 0;
    FramebufferAttachment ColorAttachment;
    FramebufferAttachment DepthAttachment;

    vk::DescriptorImageInfo DescriptorImageInfo;
    vk::Framebuffer Framebuffer;
    vk::RenderPass RenderPass;
    vk::Sampler ColorAttachmentSampler;
};

}

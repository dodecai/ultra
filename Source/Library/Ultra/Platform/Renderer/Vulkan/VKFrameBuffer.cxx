module Ultra.Platform.Renderer.VKFramebuffer;

namespace Ultra {

VKFramebuffer::VKFramebuffer(uint32_t width, uint32_t height, TextureFormat format): Framebuffer(width, height, format) {}

VKFramebuffer::~VKFramebuffer() {}


void VKFramebuffer::Bind() const {

    //vk::ClearValue clearValues[2];
    //clearValues[0].color = array<float, 4> { 0.0f, 0.0f, 0.0f, 0.72f};
    //clearValues[1].depthStencil = { 1.0f, 0 };

    //vk::RenderPassBeginInfo renderPassInfo = {};
    //renderPassInfo.renderPass = RenderPass;
    //renderPassInfo.renderArea.offset = { 0, 0 };
    //renderPassInfo.framebuffer = Framebuffer;
    //renderPassInfo.renderArea.extent = { Properties.Width, Properties.Height };
    //renderPassInfo.clearValueCount = 2;
    //renderPassInfo.pClearValues = clearValues;

    //vk::CommandBuffer buffer = VKContext->GetSwapChain()->GetCurrentDrawCommandBuffer();

    //buffer.begin(vk::CommandBufferBeginInfo());
    //buffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

    //TextureID = ImGui_ImplVulkan_AddTexture(DescriptorImageInfo.sampler, DescriptorImageInfo.imageView, (VkImageLayout)DescriptorImageInfo.imageLayout);

    //buffer.endRenderPass();
    //buffer.end();
}

void VKFramebuffer::Unbind() const {}


Reference<Texture> VKFramebuffer::GetColorAttachment() const {
    return nullptr;
}

Reference<Texture> VKFramebuffer::GetDepthAttachment() const {
    return nullptr;
}

/// Resize...
//Properties.Width = width;
//Properties.Height = height;
//
//if (!Properties.SwapChainTarget) {
//    static bool once = false;
//    if (!once) {
//
//        vk::Device device = *VKContext->GetDevice();
//
//        if (Framebuffer) device.destroyFramebuffer(Framebuffer, nullptr);
//        array<vk::AttachmentDescription, 2> attachmentDescriptions;
//
//        VKAllocator allocator(VKContext->GetDevice(), "Framebuffer");
//
//        ///
//        /// Color Attachment
//        /// 
//        {
//            const vk::Format COLOR_BUFFER_FORMAT = vk::Format::eB8G8R8A8Unorm;
//
//            vk::ImageCreateInfo imageInfo = {};
//            imageInfo.imageType = vk::ImageType::e2D;
//            imageInfo.format = COLOR_BUFFER_FORMAT;
//            imageInfo.extent.width = width;
//            imageInfo.extent.height = height;
//            imageInfo.extent.depth = 1;
//            imageInfo.mipLevels = 1;
//            imageInfo.arrayLayers = 1;
//            imageInfo.samples = vk::SampleCountFlagBits::e1;
//            imageInfo.tiling = vk::ImageTiling::eOptimal;
//            imageInfo.usage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled;
//            ColorAttachment.Image = device.createImage(imageInfo, nullptr);
//
//            vk::MemoryRequirements memoryRequirements;
//            memoryRequirements = device.getImageMemoryRequirements(ColorAttachment.Image);
//
//            allocator.Allocate(memoryRequirements, &ColorAttachment.Memory);
//            device.bindImageMemory(ColorAttachment.Image, ColorAttachment.Memory, 0);
//
//            vk::ImageViewCreateInfo viewInfo = {};
//            viewInfo.viewType = vk::ImageViewType::e2D;
//            viewInfo.format = COLOR_BUFFER_FORMAT;
//            viewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
//            viewInfo.subresourceRange.baseMipLevel = 0;
//            viewInfo.subresourceRange.levelCount = 1;
//            viewInfo.subresourceRange.baseArrayLayer = 0;
//            viewInfo.subresourceRange.layerCount = 1;
//            viewInfo.image = ColorAttachment.Image;
//            ColorAttachment.View = device.createImageView(viewInfo, nullptr);
//
//            vk::SamplerCreateInfo samplerInfo = {};
//            samplerInfo.maxAnisotropy = 1.0f;
//            samplerInfo.magFilter = vk::Filter::eLinear;
//            samplerInfo.minFilter = vk::Filter::eLinear;
//            samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
//            samplerInfo.addressModeU = vk::SamplerAddressMode::eClampToEdge;
//            samplerInfo.addressModeV = vk::SamplerAddressMode::eClampToEdge;
//            samplerInfo.addressModeW = vk::SamplerAddressMode::eClampToEdge;
//            samplerInfo.mipLodBias = 0.0f;
//            samplerInfo.maxAnisotropy = 1.0f;
//            samplerInfo.minLod = 0.0f;
//            samplerInfo.maxLod = 1.0f;
//            samplerInfo.borderColor = vk::BorderColor::eFloatOpaqueWhite;
//            ColorAttachmentSampler = device.createSampler(samplerInfo, nullptr);
//
//            attachmentDescriptions[0].flags = vk::AttachmentDescriptionFlagBits::eMayAlias;
//            attachmentDescriptions[0].format = COLOR_BUFFER_FORMAT;
//            attachmentDescriptions[0].samples = vk::SampleCountFlagBits::e1;
//            attachmentDescriptions[0].loadOp = vk::AttachmentLoadOp::eClear;
//            attachmentDescriptions[0].storeOp = vk::AttachmentStoreOp::eStore;
//            attachmentDescriptions[0].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
//            attachmentDescriptions[0].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
//            attachmentDescriptions[0].initialLayout = vk::ImageLayout::eUndefined;
//            attachmentDescriptions[0].finalLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
//        }
//
//        ///
//        /// Depth and Stencil Attachment
//        /// 
//        {
//            const vk::Format DEPTH_BUFFER_FORMAT = vk::Format::eD16UnormS8Uint;
//
//            vk::ImageCreateInfo imageInfo = {};
//            imageInfo.imageType = vk::ImageType::e2D;
//            imageInfo.format = DEPTH_BUFFER_FORMAT;
//            imageInfo.extent = vk::Extent3D { width, height, 1U };
//            imageInfo.mipLevels = 1U;
//            imageInfo.arrayLayers = 1U;
//            imageInfo.samples = vk::SampleCountFlagBits::e1;
//            imageInfo.tiling = vk::ImageTiling::eOptimal;
//            imageInfo.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;
//            DepthAttachment.Image = device.createImage(imageInfo, nullptr);
//
//            vk::MemoryRequirements memoryRequirements;
//            memoryRequirements = device.getImageMemoryRequirements(DepthAttachment.Image);
//
//            allocator.Allocate(memoryRequirements, &DepthAttachment.Memory);
//            device.bindImageMemory(DepthAttachment.Image, DepthAttachment.Memory, 0);
//
//            vk::ImageViewCreateInfo viewInfo = {};
//            viewInfo.image = DepthAttachment.Image;
//            viewInfo.viewType = vk::ImageViewType::e2D;
//            viewInfo.format = DEPTH_BUFFER_FORMAT;
//            viewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
//            if (DEPTH_BUFFER_FORMAT >= vk::Format::eD16UnormS8Uint) {
//                viewInfo.subresourceRange.aspectMask |= vk::ImageAspectFlagBits::eStencil;
//            }
//            viewInfo.subresourceRange.baseMipLevel = 0;
//            viewInfo.subresourceRange.levelCount = 1;
//            viewInfo.subresourceRange.baseArrayLayer = 0;
//            viewInfo.subresourceRange.layerCount = 1;
//            DepthAttachment.View = device.createImageView(viewInfo, nullptr);
//
//            attachmentDescriptions[1].flags = vk::AttachmentDescriptionFlagBits::eMayAlias;
//            attachmentDescriptions[1].format = DEPTH_BUFFER_FORMAT;
//            attachmentDescriptions[1].samples = vk::SampleCountFlagBits::e1;
//            attachmentDescriptions[1].loadOp = vk::AttachmentLoadOp::eClear;
//            attachmentDescriptions[1].storeOp = vk::AttachmentStoreOp::eDontCare;
//            attachmentDescriptions[1].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
//            attachmentDescriptions[1].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
//            attachmentDescriptions[1].initialLayout = vk::ImageLayout::eUndefined;
//            attachmentDescriptions[1].finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
//        }
//
//        /// 
//        vk::AttachmentReference colorReference = { 0, vk::ImageLayout::eColorAttachmentOptimal };
//        vk::AttachmentReference depthReference = { 1, vk::ImageLayout::eDepthStencilAttachmentOptimal };
//
//        vk::SubpassDescription subpassDescription = {};
//        subpassDescription.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
//        subpassDescription.colorAttachmentCount = 1;
//        subpassDescription.pColorAttachments = &colorReference;
//        subpassDescription.pDepthStencilAttachment = &depthReference;
//
//        /// Create the renderpass
//        std::array<vk::SubpassDependency, 2> dependencies;
//        dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
//        dependencies[0].dstSubpass = 0;
//        dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eFragmentShader;
//        dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
//        dependencies[0].srcAccessMask = vk::AccessFlagBits::eShaderRead;
//        dependencies[0].dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
//        dependencies[0].dependencyFlags = vk::DependencyFlagBits::eByRegion;
//
//        dependencies[1].srcSubpass = 0;
//        dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
//        dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eFragmentShader;
//        dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
//        dependencies[1].srcAccessMask = vk::AccessFlagBits::eShaderRead;
//        dependencies[1].dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
//        dependencies[1].dependencyFlags = vk::DependencyFlagBits::eByRegion;
//
//        vk::RenderPassCreateInfo renderPassInfo = {};
//        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachmentDescriptions.size());
//        renderPassInfo.pAttachments = attachmentDescriptions.data();
//        renderPassInfo.subpassCount = 1;
//        renderPassInfo.pSubpasses = &subpassDescription;
//        renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
//        renderPassInfo.pDependencies = dependencies.data();
//        RenderPass = device.createRenderPass(renderPassInfo, nullptr);
//
//        vk::ImageView attachments[2];
//        attachments[0] = ColorAttachment.View;
//        attachments[1] = DepthAttachment.View;
//
//        vk::FramebufferCreateInfo framebufferInfo = {};
//        framebufferInfo.renderPass = RenderPass;
//        framebufferInfo.attachmentCount = 2;
//        framebufferInfo.pAttachments = attachments;
//        framebufferInfo.width = width;
//        framebufferInfo.height = height;
//        framebufferInfo.layers = 1;
//        Framebuffer = device.createFramebuffer(framebufferInfo, nullptr);
//
//        DescriptorImageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
//        DescriptorImageInfo.imageView = ColorAttachment.View;
//        DescriptorImageInfo.sampler = ColorAttachmentSampler;
//
//        once = true;
//    }
//} else {
//    auto &swapChain = VKContext->GetSwapChain();
//    RenderPass = swapChain->GetRenderPass();
//}

}

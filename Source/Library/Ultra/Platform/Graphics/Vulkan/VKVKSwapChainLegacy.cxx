module;

#include <vulkan/vulkan.hpp>

///
/// @brief Hack: This is an nasty fix for Microsoft's STL implementation
/// The logger fails in multiple modules due to chrono template resolutions
///
/// @note
/// type_traits(1344,53): error C2794: 'type': is not a member of any direct or indirect base class of 'std::common_type<_Rep1,_Rep2>'
///     with
///     [
///         _Rep1 = __int64,
///         _Rep2 = __int64
///     ]
/// __msvc_chrono.hpp(268,35): error C2938: 'std::common_type_t' : Failed to specialize alias template
/// __msvc_chrono.hpp(268,56): error C2752: 'std::common_type<_Rep1,_Rep2>': more than one partial specialization matches the template argument list
///     with
///     [
///         _Rep1 = __int64,
///         _Rep2 = __int64
///     ]
/// __msvc_chrono.hpp(113,54): error C2955: 'std::chrono::duration': use of class template requires template argument list
/// __msvc_chrono.hpp(118,54): error C2955: 'std::chrono::duration': use of class template requires template argument list
///
#include <chrono>

module Ultra.Platform.Graphics.VKSwapChainLegacy;

import Ultra.Logger;
//import Ultra.Platform.Graphics.Vulkan;
import Ultra.System.FileSystem;
import Ultra.UI.GUIBuilder;

namespace Ultra {

// Default
VKSwapChainLegacy::VKSwapChainLegacy(const Reference<VKDevice> &device, const vk::SurfaceKHR &surface): mDevice(device), mSurface(surface) {
    // Verify if the surface is supported by the selected physical device
    if (!mDevice->GetPhysicalDevice()->Call().getSurfaceSupportKHR(GraphicsQueueIndex, mSurface)) {
        LogFatal("The requested surface isn't supported on the selected physical device!");
    }

    ComputeQueueIndex = mDevice->GetPhysicalDevice()->GetQueueFamilyIndex(vk::QueueFlagBits::eCompute);
    GraphicsQueueIndex = mDevice->GetPhysicalDevice()->GetQueueFamilyIndex(vk::QueueFlagBits::eGraphics);

    mAllocator = { mDevice, "SwapChain" };
}

VKSwapChainLegacy::~VKSwapChainLegacy() {
    Destroy();
}

void VKSwapChainLegacy::Create(uint32_t width, uint32_t height, bool synchronizedDraw) {
    // Check SwapChain support details (capabilities, formats, present modes)
    vk::SurfaceCapabilitiesKHR surfaceCapabilities = mDevice->GetPhysicalDevice()->Call().getSurfaceCapabilitiesKHR(mSurface);
    ChooseCapabilities(surfaceCapabilities, width, height);
    vector<vk::SurfaceFormatKHR> surfaceFormats = mDevice->GetPhysicalDevice()->Call().getSurfaceFormatsKHR(mSurface);
    ChooseSurfaceFormat(surfaceFormats);
    vector<vk::PresentModeKHR> surfacePresentModes = mDevice->GetPhysicalDevice()->Call().getSurfacePresentModesKHR(mSurface);
    ChoosePresentModes(surfacePresentModes, synchronizedDraw);

    // Swapchain Properties
    mDevice->Call().waitIdle();
    vk::SwapchainKHR oldSwapchain = mSwapchain;
    
    // Check if there is no limit
    auto maxImageCount = surfaceCapabilities.maxImageCount;
    if (maxImageCount == 0) maxImageCount = 10;
    mImageCount = std::clamp(maxImageCount, surfaceCapabilities.minImageCount, mMaxImageCount);
    mSurfaceProperties.ClearValues[0].color = array<float, 4> { 0.0f, 0.0f, 0.0f, 0.72f};
    mSurfaceProperties.ClearValues[1].depthStencil = vk::ClearDepthStencilValue { 1, 0 };

    // Get the transformation of the surface
    vk::SurfaceTransformFlagBitsKHR surfaceTransform;
    if (surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity) {
        surfaceTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
    } else {
        surfaceTransform = surfaceCapabilities.currentTransform;
    }

    // Select a supported composite alpha format (not all devices support alpha opaque)
    vk::CompositeAlphaFlagBitsKHR compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    vector<vk::CompositeAlphaFlagBitsKHR> compositeAlphaFlags = {
        vk::CompositeAlphaFlagBitsKHR::eOpaque,
        vk::CompositeAlphaFlagBitsKHR::ePreMultiplied,
        vk::CompositeAlphaFlagBitsKHR::ePostMultiplied,
        vk::CompositeAlphaFlagBitsKHR::eInherit,
    };
    for (auto &flag : compositeAlphaFlags) {
        if (surfaceCapabilities.supportedCompositeAlpha & flag) {
            compositeAlpha = flag;
            break;
        }
    }

    // Create SwapChain
    vk::SwapchainCreateInfoKHR createInfo = {};
    createInfo.flags = vk::SwapchainCreateFlagsKHR();
    createInfo.surface = mSurface;
    createInfo.minImageCount = mImageCount;
    createInfo.imageFormat = mSurfaceProperties.ColorFormat;
    createInfo.imageColorSpace = mSurfaceProperties.ColorSpace;
    createInfo.imageExtent = mSurfaceProperties.Size;
    createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
    createInfo.imageArrayLayers = 1;
    createInfo.imageSharingMode = vk::SharingMode::eExclusive;
    createInfo.queueFamilyIndexCount = GraphicsQueueIndex;
    createInfo.preTransform = surfaceTransform;
    createInfo.pQueueFamilyIndices = nullptr;
    createInfo.compositeAlpha = compositeAlpha;
    createInfo.presentMode = mPresentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = oldSwapchain;
    if (surfaceCapabilities.supportedUsageFlags & vk::ImageUsageFlagBits::eTransferSrc) {
        createInfo.imageUsage |= vk::ImageUsageFlagBits::eTransferSrc;
    }
    if (surfaceCapabilities.supportedUsageFlags & vk::ImageUsageFlagBits::eTransferDst) {
        createInfo.imageUsage |= vk::ImageUsageFlagBits::eTransferDst;
    }
    try {
        mSwapchain = mDevice->Call().createSwapchainKHR(createInfo);
    } catch (vk::SystemError exception) {
        LogFatal("{}", exception.what());
    }

    // Destroy previous SwapChain and ImageViews
    if (oldSwapchain != vk::SwapchainKHR(nullptr)) {
        mDevice->Call().destroySwapchainKHR(oldSwapchain, nullptr);
    }

    CreateImageViews();
    CreateRenderPass();
    CreateGraphicsPipeline();
    CreateFrameBuffer();
    CreateCommandBuffers();

    // Create Fences and Semaphores
    mSynchronization.PresentComplete.resize(mSwapchainBuffers.size());
    mSynchronization.RenderComplete.resize(mSwapchainBuffers.size());
    mSynchronization.WaitFences.resize(mSwapchainBuffers.size());

    for (size_t i = 0; i < mSwapchainBuffers.size(); i++) {
        // Semaphore used to ensures that image presentation is complete before starting to submit again
        mSynchronization.PresentComplete[i] = mDevice->Call().createSemaphore(vk::SemaphoreCreateInfo());

        // Semaphore used to ensures that all commands submitted have been finished before submitting the image to the queue
        mSynchronization.RenderComplete[i] = mDevice->Call().createSemaphore(vk::SemaphoreCreateInfo());

        // Fence for command buffer completion
        mSynchronization.WaitFences[i] = mDevice->Call().createFence(vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled));
    }
}

void VKSwapChainLegacy::Resize(uint32_t width, uint32_t height) {
    if (!mSwapchain) return;
    mDevice->Call().waitIdle();
    for (vk::Fence &fence : mSynchronization.WaitFences) {
        mDevice->Call().destroyFence(fence, nullptr);
    }
    for (vk::Semaphore &semaphore : mSynchronization.RenderComplete) {
        mDevice->Call().destroySemaphore(semaphore, nullptr);
    }
    for (vk::Semaphore &semaphore : mSynchronization.PresentComplete) {
        mDevice->Call().destroySemaphore(semaphore, nullptr);
    }

    mDevice->Call().freeCommandBuffers(mDrawCommandPool, mDrawCommandBuffers);
    mDevice->Call().resetCommandPool(mDrawCommandPool, vk::CommandPoolResetFlagBits::eReleaseResources);

    mDevice->Call().destroyImageView(mDepthStencilBuffer.View, nullptr);
    mDevice->Call().destroyImage(mDepthStencilBuffer.Image, nullptr);
    mDevice->Call().freeMemory(mDepthStencilBuffer.Memory, nullptr);

    for (auto &&buffer : mSwapchainBuffers) {
        mDevice->Call().destroyImageView(buffer.View, nullptr);
        mDevice->Call().destroyFramebuffer(buffer.FrameBuffer, nullptr);
    }

    Create(width, height, mSurfaceProperties.SynchronizedDraw);
}

void VKSwapChainLegacy::Destroy() {
    mDevice->Call().waitIdle();

    for (vk::Fence &fence : mSynchronization.WaitFences) {
        mDevice->Call().destroyFence(fence, nullptr);
    }
    for (vk::Semaphore &semaphore : mSynchronization.RenderComplete) {
        mDevice->Call().destroySemaphore(semaphore, nullptr);
    }
    for (vk::Semaphore &semaphore : mSynchronization.PresentComplete) {
        mDevice->Call().destroySemaphore(semaphore, nullptr);
    }

    mDevice->Call().freeCommandBuffers(mDrawCommandPool, mDrawCommandBuffers);
    mDevice->Call().destroyCommandPool(mDrawCommandPool, nullptr);

    mDevice->Call().destroyRenderPass(mRenderPass, nullptr);

    if (mSwapchain) {
        mDevice->Call().destroyImageView(mDepthStencilBuffer.View, nullptr);
        mDevice->Call().destroyImage(mDepthStencilBuffer.Image, nullptr);
        mDevice->Call().freeMemory(mDepthStencilBuffer.Memory, nullptr);

        for (auto &&buffer : mSwapchainBuffers) {
            mDevice->Call().destroyImageView(buffer.View, nullptr);
            mDevice->Call().destroyFramebuffer(buffer.FrameBuffer, nullptr);
        }

        mDevice->Call().destroySwapchainKHR(mSwapchain, nullptr);
        mSwapchain = nullptr;
    }
}


void VKSwapChainLegacy::Prepare() {
    [[maybe_unused]] auto result = mDevice->Call().waitForFences(1, &mSynchronization.WaitFences[CurrentFrame], VK_TRUE, mSynchronization.Timeout);
    CurrentBufferIndex = mDevice->Call().acquireNextImageKHR(mSwapchain, UINT64_MAX, mSynchronization.PresentComplete[CurrentFrame], nullptr).value;
    mDevice->Call().resetFences(mSynchronization.WaitFences[CurrentFrame]);

    vk::CommandBuffer drawCommandBuffer = GetCurrentDrawCommandBuffer();
    drawCommandBuffer.begin(vk::CommandBufferBeginInfo());

    vk::RenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.renderPass = mRenderPass;
    renderPassInfo.renderArea.offset = vk::Offset2D { 0, 0 };
    renderPassInfo.framebuffer = GetCurrentFramebuffer();
    renderPassInfo.renderArea.extent = mSurfaceProperties.Size;
    renderPassInfo.clearValueCount = 2u;
    renderPassInfo.pClearValues = mSurfaceProperties.ClearValues;

    drawCommandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
}

void VKSwapChainLegacy::Submit([[maybe_unused]] vk::CommandBuffer buffer) {
}

void VKSwapChainLegacy::Finish() {
    vk::CommandBuffer drawCommandBuffer = GetCurrentDrawCommandBuffer();
    drawCommandBuffer.endRenderPass();
    drawCommandBuffer.end();

    vk::Semaphore signalSemaphores[] = { mSynchronization.RenderComplete[CurrentBufferIndex] };
    vk::Semaphore waitSemaphores[] = { mSynchronization.PresentComplete[CurrentFrame] };
    vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
    vk::SubmitInfo submitInfo = {};
    submitInfo.commandBufferCount = 1;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pCommandBuffers = &mDrawCommandBuffers[CurrentBufferIndex];
    submitInfo.pSignalSemaphores = signalSemaphores;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    mDevice->GetQueue().submit(submitInfo, mSynchronization.WaitFences[CurrentFrame]);
}

void VKSwapChainLegacy::Present() {
    [[maybe_unused]] auto result2 = mDevice->Call().waitForFences(1, &mSynchronization.WaitFences[CurrentFrame], VK_TRUE, mSynchronization.Timeout);
    mDevice->Call().resetFences(mSynchronization.WaitFences[CurrentFrame]);

    vk::Result result = QueuePresent(CurrentBufferIndex, mSynchronization.RenderComplete[CurrentBufferIndex]);
    // Swapchain lost, we'll try again next poll
    if (mRebuildRequested || result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR) {
        Resize(mSurfaceProperties.Size.width, mSurfaceProperties.Size.height);
        mRebuildRequested = false;
        return;
    } else if (result == vk::Result::eErrorDeviceLost) {
        LogFatal("Device Lost");
    }

    CurrentFrame = (CurrentFrame + 1) % mImageCount;
    mDrawCommandBuffers[CurrentBufferIndex].reset(vk::CommandBufferResetFlagBits::eReleaseResources);
}


// Helpers
void VKSwapChainLegacy::CreateImageViews() {
    // SwapChain Color Attachment
    /// Images
    vector<vk::Image> images;
    auto result = mDevice->Call().getSwapchainImagesKHR(mSwapchain, &mImageCount, nullptr);
    images.resize(mImageCount);
    result = mDevice->Call().getSwapchainImagesKHR(mSwapchain, &mImageCount, images.data());

    /// Views
    mSwapchainBuffers.resize(mImageCount);
    for (size_t i = 0; i < mImageCount; i++) {
        vk::ImageViewCreateInfo createInfo = {};
        createInfo.format = mSurfaceProperties.ColorFormat;
        createInfo.viewType = vk::ImageViewType::e2D;
        createInfo.components = {       // eIdentity for all?
            vk::ComponentSwizzle::eR,
            vk::ComponentSwizzle::eG,
            vk::ComponentSwizzle::eB,
            vk::ComponentSwizzle::eA,
        };
        createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        mSwapchainBuffers[i].Image = images[i];
        createInfo.image = mSwapchainBuffers[i].Image;

        mSwapchainBuffers[i].View = mDevice->Call().createImageView(createInfo);
    }

    // SwapChain Depth\Stencil Attachment
    /// Image
    vk::ImageCreateInfo imageCreateInfo = {};
    imageCreateInfo.imageType = vk::ImageType::e2D;
    imageCreateInfo.format = mSurfaceProperties.DepthFormat;
    imageCreateInfo.extent = vk::Extent3D(mSurfaceProperties.Size, 1U);
    imageCreateInfo.mipLevels = 1U;
    imageCreateInfo.arrayLayers = 1U;
    imageCreateInfo.samples = vk::SampleCountFlagBits::e1;
    imageCreateInfo.tiling = vk::ImageTiling::eOptimal;
    imageCreateInfo.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits::eTransferSrc;
    imageCreateInfo.sharingMode = vk::SharingMode::eExclusive;
    imageCreateInfo.queueFamilyIndexCount = 1;
    imageCreateInfo.pQueueFamilyIndices = &GraphicsQueueIndex;
    imageCreateInfo.initialLayout = vk::ImageLayout::eUndefined;
    mDepthStencilBuffer.Image = mDevice->Call().createImage(imageCreateInfo);

    /// Memory (Search through GPU memory properies to see if this can be device local)
    vk::MemoryRequirements memoryRequirements = mDevice->Call().getImageMemoryRequirements(mDepthStencilBuffer.Image);
    mAllocator.Allocate(memoryRequirements, &mDepthStencilBuffer.Memory, vk::MemoryPropertyFlagBits::eDeviceLocal);
    mDevice->Call().bindImageMemory(mDepthStencilBuffer.Image, mDepthStencilBuffer.Memory, 0);

    /// View
    vk::ImageViewCreateInfo viewCreateInfo = {};
    viewCreateInfo.image = mDepthStencilBuffer.Image;
    viewCreateInfo.viewType = vk::ImageViewType::e2D;
    viewCreateInfo.format = mSurfaceProperties.DepthFormat;
    viewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
    if (mSurfaceProperties.DepthFormat >= vk::Format::eD16UnormS8Uint) {
        viewCreateInfo.subresourceRange.aspectMask |= vk::ImageAspectFlagBits::eStencil;
    }
    viewCreateInfo.subresourceRange.baseMipLevel = 0;
    viewCreateInfo.subresourceRange.levelCount = 1;
    viewCreateInfo.subresourceRange.baseArrayLayer = 0;
    viewCreateInfo.subresourceRange.layerCount = 1;
    mDepthStencilBuffer.View = mDevice->Call().createImageView(viewCreateInfo);
}

void VKSwapChainLegacy::CreateGraphicsPipeline() {
    // Shader Modules
    auto vertexShader = File::LoadAsBinary("./Data/Cache/Shaders/Test.vertex.spirv");
    vk::ShaderModuleCreateInfo vertexCreateInfo {};
    vertexCreateInfo.codeSize = vertexShader.size() * sizeof(uint32_t);
    vertexCreateInfo.pCode = reinterpret_cast<const uint32_t *>(vertexShader.data());

    vk::ShaderModule vertexModule;
    auto result = mDevice->Call().createShaderModule(&vertexCreateInfo, nullptr, &vertexModule);
    if (result == vk::Result::eSuccess) { /* ToDo: Check Result */ }

    auto fragmentShader = File::LoadAsBinary("./Data/Cache/Shaders/Test.fragment.spirv");
    vk::ShaderModuleCreateInfo fragmentCreateInfo {};
    fragmentCreateInfo.codeSize = fragmentShader.size() * sizeof(uint32_t);
    fragmentCreateInfo.pCode = reinterpret_cast<const uint32_t *>(fragmentShader.data());

    vk::ShaderModule fragmentModule;
    result = mDevice->Call().createShaderModule(&fragmentCreateInfo, nullptr, &fragmentModule);
    if (result == vk::Result::eSuccess) { /* ToDo: Check Result */ }

    // Shader Stage
    vk::PipelineShaderStageCreateInfo vertexShaderStageCreateInfo {};
    vertexShaderStageCreateInfo.module = vertexModule;
    vertexShaderStageCreateInfo.pName = "main";
    vertexShaderStageCreateInfo.stage = vk::ShaderStageFlagBits::eVertex;

    vk::PipelineShaderStageCreateInfo fragmentShaderStageCreateInfo {};
    fragmentShaderStageCreateInfo.module = fragmentModule;
    fragmentShaderStageCreateInfo.pName = "main";
    fragmentShaderStageCreateInfo.stage = vk::ShaderStageFlagBits::eFragment;

    vector<vk::PipelineShaderStageCreateInfo> shaderStages { vertexShaderStageCreateInfo, fragmentShaderStageCreateInfo };

    // Fixed Functions
    vector<vk::DynamicState> dynamicStates {
        vk::DynamicState::eScissor,
        vk::DynamicState::eViewport,
    };

    vk::PipelineDynamicStateCreateInfo dynamicState {
        {},
        static_cast<uint32_t>(dynamicStates.size()),
        dynamicStates.data()
    };

    // Vertex Input
    vk::PipelineVertexInputStateCreateInfo vertexInputInfo {};
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;

    // Input Assembly
    vk::PipelineInputAssemblyStateCreateInfo inputAssembly {};
    inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
    inputAssembly.primitiveRestartEnable = false;

    // Viewports and Scissors
    mSurfaceProperties.Viewport.maxDepth = 1.0f;
    mSurfaceProperties.Viewport.minDepth = 0.0f;
    mSurfaceProperties.Viewport.height = (float)mSurfaceProperties.Size.height;
    mSurfaceProperties.Viewport.width = (float)mSurfaceProperties.Size.width;
    mSurfaceProperties.Viewport.x = 0.0f;
    mSurfaceProperties.Viewport.y = 0.0f;

    mSurfaceProperties.RenderArea.extent = mSurfaceProperties.Size;
    mSurfaceProperties.RenderArea.offset = vk::Offset2D { 0, 0 };

    vk::PipelineViewportStateCreateInfo viewportState {};
    viewportState.viewportCount = 1;
    viewportState.pViewports = &mSurfaceProperties.Viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &mSurfaceProperties.RenderArea;

    // Rasterizer
    vk::PipelineRasterizationStateCreateInfo rasterizer {};
    rasterizer.depthClampEnable = false;
    rasterizer.rasterizerDiscardEnable = false;
    rasterizer.lineWidth = 1.0f; // Requires enabling GPU wideLines GPU feature
    rasterizer.polygonMode = vk::PolygonMode::eFill;
    rasterizer.cullMode = vk::CullModeFlagBits::eBack;
    rasterizer.frontFace = vk::FrontFace::eClockwise;
    rasterizer.depthBiasEnable = false;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;

    // Multisampling
    vk::PipelineMultisampleStateCreateInfo multisampling {};
    multisampling.sampleShadingEnable = false;
    multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = false;
    multisampling.alphaToOneEnable = false;

    // ToDo: Depth and stencil testing
    vk::PipelineDepthStencilStateCreateInfo depthStencilStateInfo {};

    // Color Blending
    vk::PipelineColorBlendAttachmentState colorBlendAttachment {};
    colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    colorBlendAttachment.blendEnable = false;
    colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eOne;
    colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eZero;
    colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;
    colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
    colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
    colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd;

    // - AlphaBlending 
    //colorBlendAttachment.blendEnable = true;
    //colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
    //colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
    //colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;
    //colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
    //colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
    //colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd;

    vk::PipelineColorBlendStateCreateInfo colorBlending {};
    colorBlending.logicOpEnable = false;
    colorBlending.logicOp = vk::LogicOp::eCopy;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    // Pipeline Layout
    vk::PipelineLayoutCreateInfo pipelineLayoutInfo {};
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = VK_NULL_HANDLE;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = VK_NULL_HANDLE;

    vk::PipelineLayout pipelineLayout;
    result = mDevice->Call().createPipelineLayout(&pipelineLayoutInfo, nullptr, &pipelineLayout); // ToDo: Must be manually destroyed
    if (result == vk::Result::eSuccess) { /* ToDo: Check Result */ }

    vk::GraphicsPipelineCreateInfo pipelineInfo {};
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencilStateInfo;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = mRenderPass; // Attention: Depends on renderpass, needs refactor;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = nullptr;
    pipelineInfo.basePipelineIndex = -1;

    vk::Pipeline graphicsPipeline;
    result = mDevice->Call().createGraphicsPipelines(nullptr, 1, &pipelineInfo, nullptr, &graphicsPipeline);
    if (result == vk::Result::eSuccess) { /* ToDo: Check Result */ }
    // ToDo: Destroy Pipeline and PipelineLayout
}

void VKSwapChainLegacy::CreateRenderPass() {
    // Attachments
    vector<vk::AttachmentDescription> attachments = {
        // Color Attachment
        vk::AttachmentDescription(
            vk::AttachmentDescriptionFlags(),
            mSurfaceProperties.ColorFormat,
            vk::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eStore,
            vk::AttachmentLoadOp::eDontCare,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::ePresentSrcKHR
        ),
        // Depth\Stencil Attachment
        vk::AttachmentDescription(
            vk::AttachmentDescriptionFlags(),
            mSurfaceProperties.DepthFormat,
            vk::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eStore,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eDepthStencilAttachmentOptimal
        )
    };
    vector<vk::AttachmentReference> colorReferences = {
        vk::AttachmentReference(0, vk::ImageLayout::eColorAttachmentOptimal),
    };
    vector<vk::AttachmentReference> depthReferences = {
        vk::AttachmentReference(1, vk::ImageLayout::eDepthStencilAttachmentOptimal),
    };

    // Subpasses
    // ToDo: Error after migration ~ Severity Code Description Project File Line Suppression StateError C2280 'vk::ArrayProxyNoTemporaries<const uint32_t>::ArrayProxyNoTemporaries<int>(V &&)': attempting to reference a deleted function Ultra D:\Development\Apps\Ultra Spectra\Source\Library\Ultra\Platform\GFX\Vulkan\VKVKSwapChainLegacy.cxx 356
    vector<vk::SubpassDescription> subpasses(1);
    subpasses[0].setFlags(vk::SubpassDescriptionFlags())
        .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
        .setColorAttachments(colorReferences);
    subpasses[0].pDepthStencilAttachment = depthReferences.empty() ? nullptr : &depthReferences.front();
    //vector<vk::SubpassDescription> subpasses {
    //    vk::SubpassDescription(
    //        vk::SubpassDescriptionFlags(),
    //        vk::PipelineBindPoint::eGraphics,
    //        static_cast<uint32_t>(colorReferences.size()),
    //        colorReferences.data(),
    //        nullptr,
    //        depthReferences.empty() ? nullptr : &depthReferences.front(),
    //        0,
    //        nullptr
    //    )
    //};

    // Dependencies
    vector<vk::SubpassDependency> dependencies = {
        vk::SubpassDependency(
            VK_SUBPASS_EXTERNAL,
            0,
            vk::PipelineStageFlagBits::eBottomOfPipe,
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            vk::AccessFlagBits::eMemoryRead,
            vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,
            vk::DependencyFlagBits::eByRegion
        ),
        vk::SubpassDependency(
            0,
            VK_SUBPASS_EXTERNAL,
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            vk::PipelineStageFlagBits::eBottomOfPipe,
            vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,
            vk::AccessFlagBits::eMemoryRead,
            vk::DependencyFlagBits::eByRegion
        )
    };

    // Finally
    mRenderPass = mDevice->Call().createRenderPass(
        vk::RenderPassCreateInfo(
            vk::RenderPassCreateFlags(),
            attachments,
            subpasses,
            dependencies
        )
    );
}

void VKSwapChainLegacy::CreateFrameBuffer() {
    vector<vk::ImageView> attachments;
    attachments.resize(2);

    vk::FramebufferCreateInfo createInfo = {};
    createInfo.renderPass = mRenderPass;
    createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    createInfo.pAttachments = attachments.data();
    createInfo.width = mSurfaceProperties.Size.width;
    createInfo.height = mSurfaceProperties.Size.height;
    createInfo.layers = 1;

    for (size_t i = 0; i < mSwapchainBuffers.size(); i++) {
        attachments[0] = mSwapchainBuffers[i].View;
        attachments[1] = mDepthStencilBuffer.View;

        mSwapchainBuffers[i].FrameBuffer = mDevice->Call().createFramebuffer(createInfo);
    }
}

void VKSwapChainLegacy::CreateCommandBuffers() {
    mDrawCommandBuffers.resize(mImageCount);

    vk::CommandPoolCreateInfo createInfo = {};
    createInfo.queueFamilyIndex = GraphicsQueueIndex;
    createInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer | vk::CommandPoolCreateFlagBits::eTransient;
    mDrawCommandPool = mDevice->Call().createCommandPool(createInfo);

    vk::CommandBufferAllocateInfo commandBufferAllocateInfo = {};
    commandBufferAllocateInfo.commandPool = mDrawCommandPool;
    commandBufferAllocateInfo.level = vk::CommandBufferLevel::ePrimary;
    commandBufferAllocateInfo.commandBufferCount = static_cast<uint32_t>(mDrawCommandBuffers.size());
    mDrawCommandBuffers = mDevice->Call().allocateCommandBuffers(commandBufferAllocateInfo);

    for (size_t i = 0; i < mDrawCommandBuffers.size(); i++) {
        mDrawCommandBuffers[i].begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eSimultaneousUse));
        mDrawCommandBuffers[i].end();
    }
}


// Accessors
const vk::CommandBuffer &VKSwapChainLegacy::GetCurrentDrawCommandBuffer() const {
    return mDrawCommandBuffers[CurrentBufferIndex];
}

const vk::Framebuffer &VKSwapChainLegacy::GetCurrentFramebuffer() const {
    return mSwapchainBuffers[CurrentBufferIndex].FrameBuffer;
}

const uint32_t VKSwapChainLegacy::GetImageCount() const {
    return mImageCount;
}

const vk::Rect2D &VKSwapChainLegacy::GetRenderArea() const {
    return mSurfaceProperties.RenderArea;
}

const vk::RenderPass &VKSwapChainLegacy::GetRenderPass() const {
    return mRenderPass;
}


// Mutators
void VKSwapChainLegacy::SetSyncronizedDraw(bool enable) {
    if (mSurfaceProperties.SynchronizedDraw != enable) {
        mSurfaceProperties.SynchronizedDraw = enable;
        mRebuildRequested = true;
    }
}


// Internal
void VKSwapChainLegacy::ChooseCapabilities(const vk::SurfaceCapabilitiesKHR &capabilities, uint32_t width, uint32_t height) {
    if (!(capabilities.currentExtent.width == -1 || capabilities.currentExtent.height == -1)) {
        mSurfaceProperties.Size = capabilities.currentExtent;
    } else {
        mSurfaceProperties.Size = vk::Extent2D(
            std::clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
            std::clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
        );
    }
    mSurfaceProperties.RenderArea = vk::Rect2D(vk::Offset2D(), mSurfaceProperties.Size);
    mSurfaceProperties.Viewport = vk::Viewport(
        0.0f,
        0.0f,
        static_cast<float>(mSurfaceProperties.Size.width),
        static_cast<float>(mSurfaceProperties.Size.height),
        0.0f,
        1.0f
    );
}

void VKSwapChainLegacy::ChooseSurfaceFormat(const vector<vk::SurfaceFormatKHR> &surfaceFormats) {
    // Color Buffer
    if (surfaceFormats.size() == 1 && surfaceFormats[0].format == vk::Format::eUndefined) {
        mSurfaceProperties.ColorFormat = vk::Format::eB8G8R8A8Unorm;
        mSurfaceProperties.ColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
    } else {
        mSurfaceProperties.ColorFormat = surfaceFormats[0].format;
        mSurfaceProperties.ColorSpace = surfaceFormats[0].colorSpace;

        for (auto &&format : surfaceFormats) {
            if (format.format == vk::Format::eB8G8R8A8Unorm) {
                mSurfaceProperties.ColorFormat = format.format;
                mSurfaceProperties.ColorSpace = format.colorSpace;
                break;
            }
        }
    }

    // Depth Buffer
    vector<vk::Format> depthFormats = {
        vk::Format::eD32SfloatS8Uint,
        vk::Format::eD32Sfloat,
        vk::Format::eD24UnormS8Uint,
        vk::Format::eD16UnormS8Uint,
        vk::Format::eD16Unorm
    };
    for (vk::Format &format : depthFormats) {
        vk::FormatProperties depthFormatProperties = mDevice->GetPhysicalDevice()->Call().getFormatProperties(format);

        // Format must support depth stencil attachment for optimal tiling
        if (depthFormatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment) {
            mSurfaceProperties.DepthFormat = format;
            break;
        }
    }
}

void VKSwapChainLegacy::ChoosePresentModes(const vector<vk::PresentModeKHR> &presentModes, [[maybe_unused]] bool sync) {
    mPresentMode = vk::PresentModeKHR::eFifo;
    if (!mSurfaceProperties.SynchronizedDraw) {
        for (auto &mode : presentModes) {
            switch (mode) {
                case vk::PresentModeKHR::eMailbox: { mPresentMode = vk::PresentModeKHR::eMailbox;   break; }
                case vk::PresentModeKHR::eImmediate: { mPresentMode = vk::PresentModeKHR::eImmediate; break; }
                default: { break; }
            }
            if (mPresentMode != vk::PresentModeKHR::eFifo) break;
        }
    }
}

vk::Result VKSwapChainLegacy::QueuePresent(uint32_t imageIndex, vk::Semaphore renderComplete) {
    vk::PresentInfoKHR presentInfo = {};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &mSwapchain;
    presentInfo.pImageIndices = &imageIndex;
    if (renderComplete) {
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &renderComplete;
    }
    return mDevice->GetQueue().presentKHR(presentInfo);
}

}

#include "VKTest.h"

#include "Omnia/Core/Application.h"
#include "Omnia/System/FileSystem.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Ultra {

//static Reference<VKPipeline> s_CompositePipeline;
static vk::CommandBuffer s_ImGuiCommandBuffer;
static vk::CommandBuffer s_CompositeCommandBuffer;

// ToDo: CleanUp everything not needed...
struct Vertex {
    float position[3];
    float color[3];
};

Vertex VertexBufferData[3] = {
    { {  1.0f,  1.0f,  0.0f }, {  1.0f,  0.0f,  0.0f } },
    { { -1.0f,  1.0f,  0.0f }, {  0.0f,  1.0f,  0.0f } },
    { {  0.0f, -1.0f,  0.0f }, {  0.0f,  0.0f,  1.0f } }
};

uint32_t IndexBufferData[3] = { 0, 1, 2 };

struct {
    glm::mat4 projectionMatrix;
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
} uboVS;
// ~ToDo


void CompositeRenderPass(vk::CommandBufferInheritanceInfo& inheritanceInfo) {

    auto *context = reinterpret_cast<VKContext*>(Application::Get().pContext.get());
    auto &swapChain = context->GetSwapChain();

    vk::CommandBuffer commandBuffer = s_CompositeCommandBuffer;

    uint32_t width = swapChain->GetRenderArea().extent.width;
    uint32_t height = swapChain->GetRenderArea().extent.height;

    vk::CommandBufferBeginInfo cmdBufInfo = {};
    cmdBufInfo.flags = vk::CommandBufferUsageFlagBits::eRenderPassContinue;
    cmdBufInfo.pInheritanceInfo = &inheritanceInfo;

    commandBuffer.begin(cmdBufInfo);

    // Update dynamic viewport state
    vk::Viewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = (float)height;
    viewport.height = -(float)height;
    viewport.width = (float)width;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    commandBuffer.setViewport(1, viewport);

    // Update dynamic scissor state
    VkRect2D scissor = {};
    scissor.extent.width = width;
    scissor.extent.height = height;
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    //commandBuffer.setScissor(0, scissor);

    //// Copy 3D scene here!
    //Reference<VKPipeline> vulkanPipeline = s_CompositePipeline.As<VulkanPipeline>();

    //VkPipelineLayout layout = vulkanPipeline->GetVulkanPipelineLayout();

    //auto vulkanMeshVB = s_QuadVertexBuffer.As<VulkanVertexBuffer>();
    //VkBuffer vbMeshBuffer = vulkanMeshVB->GetVulkanBuffer();
    //VkDeviceSize offsets[1] = { 0 };
    //vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vbMeshBuffer, offsets);

    //auto vulkanMeshIB = s_QuadIndexBuffer.As<VulkanIndexBuffer>();
    //VkBuffer ibBuffer = vulkanMeshIB->GetVulkanBuffer();
    //vkCmdBindIndexBuffer(commandBuffer, ibBuffer, 0, VK_INDEX_TYPE_UINT32);

    //VkPipeline pipeline = vulkanPipeline->GetVulkanPipeline();
    //vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    //// Bind descriptor sets describing shader binding points
    //vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &s_QuadDescriptorSet, 0, nullptr);

    //vkCmdDrawIndexed(commandBuffer, s_QuadIndexBuffer->GetCount(), 1, 0, 0, 0);

    //VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer));
}



VKTest::VKTest() {
    pContext = reinterpret_cast<VKContext*>(Application::Get().pContext.get());

    // Command Buffers
    LoadResources();
    SetupPipeline();
    SetupCommands();
}

VKTest::~VKTest() {
    // Vertices
    pContext->GetDevice()->Call().freeMemory(Vertices.memory);
    pContext->GetDevice()->Call().destroyBuffer(Vertices.buffer);

    // Index buffer
    pContext->GetDevice()->Call().freeMemory(Indices.memory);
    pContext->GetDevice()->Call().destroyBuffer(Indices.buffer);

    // Shader Module
    pContext->GetDevice()->Call().destroyShaderModule(VertModule);
    pContext->GetDevice()->Call().destroyShaderModule(FragModule);

    // Graphics Pipeline
    pContext->GetDevice()->Call().destroyPipeline(Pipeline);
    pContext->GetDevice()->Call().destroyPipelineLayout(PipelineLayout);

    // Descriptor Pool
    pContext->GetDevice()->Call().destroyDescriptorPool(DescriptorPool);
    for (vk::DescriptorSetLayout &dsl : DescriptorSetLayouts) {
        pContext->GetDevice()->Call().destroyDescriptorSetLayout(dsl);
    }

    // Uniform block object
    pContext->GetDevice()->Call().freeMemory(UniformDataVS.memory);
    pContext->GetDevice()->Call().destroyBuffer(UniformDataVS.buffer);
}

void VKTest::Resize() {
    SetupCommands();
}

void VKTest::Update() {
    #define OLD_TEST
    #ifndef OLD_TEST
    vk::CommandBuffer drawCommandBuffer = pContext->GetSwapChain()->GetCurrentDrawCommandFramebuffer();

    VkClearValue clearValues[2];
    clearValues[0].color = { { 0.1f, 0.1f, 0.1f, 1.0f } };
    clearValues[1].depthStencil = { 1.0f, 0 };

    vk::RenderPassBeginInfo renderPassBeginInfo = {};
    renderPassBeginInfo.renderPass = pContext->GetSwapChain()->GetRenderPass();
    renderPassBeginInfo.renderArea.offset.x = 0;
    renderPassBeginInfo.renderArea.offset.y = 0;
    renderPassBeginInfo.renderArea.extent.width = pContext->GetSwapChain()->GetRenderArea().extent.width;
    renderPassBeginInfo.renderArea.extent.height = pContext->GetSwapChain()->GetRenderArea().extent.height;
    renderPassBeginInfo.clearValueCount = 2; // Color + depth
    //renderPassBeginInfo.pClearValues = clearValues;
    renderPassBeginInfo.framebuffer = pContext->GetSwapChain()->GetCurrentFramebuffer();

    drawCommandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eSecondaryCommandBuffers);
    vk::CommandBufferInheritanceInfo inheritanceInfo = {};
    inheritanceInfo.renderPass = pContext->GetSwapChain()->GetRenderPass();
    inheritanceInfo.framebuffer = pContext->GetSwapChain()->GetCurrentFramebuffer();
    CompositeRenderPass(inheritanceInfo);


    std::vector<vk::CommandBuffer> commandBuffers;
    commandBuffers.push_back(s_CompositeCommandBuffer);


    drawCommandBuffer.executeCommands(commandBuffers);
    drawCommandBuffer.endRenderPass();
    #endif

    #ifdef OLD_TEST
    CurrentBuffer = pContext->GetSwapChain()->GetCurrentBufferIndex();

    vk::CommandBuffer buffer = pContext->GetDevice()->GetCommandBuffer(true);
    ReloadCommandBuffer(buffer);

    // Update Uniforms
    uboVS.modelMatrix = glm::rotate(uboVS.modelMatrix, (float)(0.001f *0.32f *2.0f), glm::vec3( 0.2f, 0.2f, 0.2f ));
    void *pData;
    pData =  pContext->GetDevice()->Call().mapMemory(UniformDataVS.memory, 0, sizeof(uboVS));
    memcpy(pData, &uboVS, sizeof(uboVS));
    pContext->GetDevice()->Call().unmapMemory(UniformDataVS.memory);

    // Submit
    vk::SubmitInfo submitInfo;
    submitInfo
        .setCommandBufferCount(1)
        .setSignalSemaphoreCount(1)
        .setPCommandBuffers(&CommandBuffers[CurrentBuffer])
        .setPSignalSemaphores(&pContext->GetSwapChain()->mSemaphores.RenderComplete)
        ;

    pContext->GetDevice()->FlushCommandBuffer(buffer, submitInfo);
    //pContext->GetSwapChain()->Present();
    pContext->GetSwapChain()->QueuePresent(pContext->GetDevice()->GetQueue(), CurrentBuffer, pContext->GetSwapChain()->mSemaphores.RenderComplete);
    #endif
}


void VKTest::LoadResources() {
    VKAllocator allocator(pContext->GetDevice(), "Test");

    /**
    * Create Shader uniform binding data structures:
    */

    //Descriptor Pool
    vector<vk::DescriptorPoolSize> descriptorPoolSizes = { vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, 1) };

    DescriptorPool =  pContext->GetDevice()->Call().createDescriptorPool(
        vk::DescriptorPoolCreateInfo( vk::DescriptorPoolCreateFlags(), 1, static_cast<uint32_t>(descriptorPoolSizes.size()),  descriptorPoolSizes.data())
    );

    // Descriptor Set Layout ~ Binding 0: Uniform buffer (Vertex shader)
    vector<vk::DescriptorSetLayoutBinding> descriptorSetLayoutBindings = {
        vk::DescriptorSetLayoutBinding(0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex, nullptr)
    };

    DescriptorSetLayouts = {
        pContext->GetDevice()->Call().createDescriptorSetLayout( vk::DescriptorSetLayoutCreateInfo( vk::DescriptorSetLayoutCreateFlags(), static_cast<uint32_t>(descriptorSetLayoutBindings.size()), descriptorSetLayoutBindings.data()))
    };

    DescriptorSets =  pContext->GetDevice()->Call().allocateDescriptorSets(
        vk::DescriptorSetAllocateInfo(DescriptorPool, static_cast<uint32_t>(DescriptorSetLayouts.size()), DescriptorSetLayouts.data())
    );

    PipelineLayout =  pContext->GetDevice()->Call().createPipelineLayout(
        vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(), static_cast<uint32_t>(DescriptorSetLayouts.size()), DescriptorSetLayouts.data(), 0, nullptr)
    );

    // Setup vertices and indices data
    uint32_t vertexBufferSize = static_cast<uint32_t>(3) * sizeof(Vertex);
    Indices.count = 3;
    uint32_t indexBufferSize = Indices.count * sizeof(uint32_t);

    mVertexBuffer = VertexBuffer::Create(VertexBufferData, vertexBufferSize);
    mIndexBuffer = IndexBuffer::Create(IndexBufferData, indexBufferSize);

    // Buffer copies have to be submitted to a queue, so we need a command buffer for them
    // Note: Some devices offer a dedicated transfer queue (with only the transfer bit set) that may be faster when doing lots of copies
    vk::CommandBuffer copyCmd =  pContext->GetDevice()->GetCommandBuffer(true);
   
    // Vertex buffer
    // Put buffer region copies into command buffer
    Vertices.buffer = ((VKVertexBuffer *)mVertexBuffer.get())->GetNativeBufferT();
    vector<vk::BufferCopy> copyRegions = { vk::BufferCopy(0, 0, vertexBufferSize) };
    copyCmd.copyBuffer(
        ((VKVertexBuffer *)mVertexBuffer.get())->GetNativeBuffer(),
        Vertices.buffer,
        copyRegions
    );

    // Index buffer
    Indices.buffer = ((VKIndexBuffer *)mIndexBuffer.get())->GetNativeBufferT();
    copyRegions = { vk::BufferCopy(0, 0, indexBufferSize) };
    copyCmd.copyBuffer(
        ((VKIndexBuffer *)mIndexBuffer.get())->GetNativeBuffer(),
        Indices.buffer,
        copyRegions
    );

    // Flushing the command buffer will also submit it to the queue and uses a fence to ensure that all commands have been executed before returning
    pContext->GetDevice()->FlushCommandBuffer(copyCmd);
    


    // Vertex input binding
    Vertices.inputBinding.binding = 0;
    Vertices.inputBinding.stride = sizeof(Vertex);
    Vertices.inputBinding.inputRate = vk::VertexInputRate::eVertex;


    // Inpute attribute binding describe shader attribute locations and memory layouts
    // These match the following shader layout (see assets/shaders/triangle.vert):
    //	layout (location = 0) in vec3 inPos;
    //	layout (location = 1) in vec3 inColor;
    Vertices.inputAttributes.resize(2);
    // Attribute location 0: Position
    Vertices.inputAttributes[0].binding = 0;
    Vertices.inputAttributes[0].location = 0;
    Vertices.inputAttributes[0].format = vk::Format::eR32G32B32Sfloat;
    Vertices.inputAttributes[0].offset = offsetof(Vertex, position);
    // Attribute location 1: Color
    Vertices.inputAttributes[1].binding = 0;
    Vertices.inputAttributes[1].location = 1;
    Vertices.inputAttributes[1].format = vk::Format::eR32G32B32Sfloat;
    Vertices.inputAttributes[1].offset = offsetof(Vertex, color);

    // Assign to the vertex input state used for pipeline creation
    Vertices.inputState.flags = vk::PipelineVertexInputStateCreateFlags();
    Vertices.inputState.vertexBindingDescriptionCount = 1;
    Vertices.inputState.pVertexBindingDescriptions = &Vertices.inputBinding;
    Vertices.inputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(Vertices.inputAttributes.size());
    Vertices.inputState.pVertexAttributeDescriptions = Vertices.inputAttributes.data();

    // Prepare and initialize a uniform buffer block containing shader uniforms
    // Single uniforms like in OpenGL are no longer present in Vulkan. All Shader uniforms are passed via uniform buffer blocks

    // Vertex shader uniform buffer block
    // Create a new buffer
    UniformDataVS.buffer = pContext->GetDevice()->Call().createBuffer(
        vk::BufferCreateInfo(
            vk::BufferCreateFlags(),
            sizeof(uboVS),
            vk::BufferUsageFlagBits::eUniformBuffer
        )
    );
    // Get memory requirements including size, alignment and memory type 

    vk::MemoryRequirements memReqs;
    memReqs = pContext->GetDevice()->Call().getBufferMemoryRequirements(UniformDataVS.buffer);

    allocator.Allocate(memReqs, &UniformDataVS.memory, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
    // Bind memory to buffer
    pContext->GetDevice()->Call().bindBufferMemory(UniformDataVS.buffer, UniformDataVS.memory, 0);

    // Store information in the uniform's descriptor that is used by the descriptor set
    UniformDataVS.descriptor.buffer = UniformDataVS.buffer;
    UniformDataVS.descriptor.offset = 0;
    UniformDataVS.descriptor.range = sizeof(uboVS);

    // Update Uniforms
    float zoom = -2.5f;

    // Update matrices
    uboVS.projectionMatrix = glm::perspective(45.0f, (float)pContext->GetSwapChain()->mViewport.width / (float)pContext->GetSwapChain()->mViewport.height, 0.01f, 1024.0f);
    uboVS.viewMatrix = glm::translate(glm::identity<glm::mat4>(), { 0.0f, 0.0f, zoom });
    uboVS.modelMatrix = glm::identity<glm::mat4>();

    // Map uniform buffer and update it
    void *pData;
    pData = pContext->GetDevice()->Call().mapMemory(UniformDataVS.memory, 0, sizeof(uboVS));
    memcpy(pData, &uboVS, sizeof(uboVS));
    pContext->GetDevice()->Call().unmapMemory(UniformDataVS.memory);



    vector<vk::WriteDescriptorSet> descriptorWrites = {
        vk::WriteDescriptorSet(
            DescriptorSets[0],
        0,
        0,
        1,
        vk::DescriptorType::eUniformBuffer,
        nullptr,
        &UniformDataVS.descriptor,
        nullptr
        )
    };

    pContext->GetDevice()->Call().updateDescriptorSets(descriptorWrites, nullptr);
}

void VKTest::SetupPipeline() {
    // Create Graphics Pipeline
    string vertShaderCode = ReadFile("Assets/Shaders/triangle.vert.spv");
    string fragShaderCode = ReadFile("Assets/Shaders/triangle.frag.spv");

    VertModule = pContext->GetDevice()->Call().createShaderModule(
        vk::ShaderModuleCreateInfo(
            vk::ShaderModuleCreateFlags(),
            vertShaderCode.size(),
            (uint32_t*)vertShaderCode.data()
        )
    );

    FragModule = pContext->GetDevice()->Call().createShaderModule(
        vk::ShaderModuleCreateInfo(
            vk::ShaderModuleCreateFlags(),
            fragShaderCode.size(),
            (uint32_t*)fragShaderCode.data()
        )
    );

    std::vector<vk::PipelineShaderStageCreateInfo> pipelineShaderStages = {
        vk::PipelineShaderStageCreateInfo(
            vk::PipelineShaderStageCreateFlags(),
            vk::ShaderStageFlagBits::eVertex,
            VertModule,
            "main",
            nullptr
        ),
        vk::PipelineShaderStageCreateInfo(
            vk::PipelineShaderStageCreateFlags(),
            vk::ShaderStageFlagBits::eFragment,
            FragModule,
            "main",
            nullptr
        )
    };

    vk::PipelineVertexInputStateCreateInfo pvi = Vertices.inputState;

    vk::PipelineInputAssemblyStateCreateInfo pia (
        vk::PipelineInputAssemblyStateCreateFlags(),
        vk::PrimitiveTopology::eTriangleList
    );

    vk::PipelineViewportStateCreateInfo pv (
        vk::PipelineViewportStateCreateFlagBits(),
        1,
        &pContext->GetSwapChain()->mViewport,
        1,
        &pContext->GetSwapChain()->mRenderArea
    );

    vk::PipelineRasterizationStateCreateInfo pr (
        vk::PipelineRasterizationStateCreateFlags(),
        VK_FALSE,
        VK_FALSE,
        vk::PolygonMode::eFill,
        vk::CullModeFlagBits::eNone,
        vk::FrontFace::eCounterClockwise,
        VK_FALSE,
        0,
        0,
        0,
        1.0f
    );

    vk::PipelineMultisampleStateCreateInfo pm (
        vk::PipelineMultisampleStateCreateFlags(),
        vk::SampleCountFlagBits::e1
    );

    // Dept and Stencil state for primative compare/test operations
    vk::PipelineDepthStencilStateCreateInfo pds = vk::PipelineDepthStencilStateCreateInfo (
        vk::PipelineDepthStencilStateCreateFlags(),
        VK_TRUE,
        VK_TRUE,
        vk::CompareOp::eLessOrEqual,
        VK_FALSE,
        VK_FALSE,
        vk::StencilOpState(),
        vk::StencilOpState(),
        0,
        0
    );

    // Blend State - How two primatives should draw on top of each other.
    vector<vk::PipelineColorBlendAttachmentState> colorBlendAttachments = {
        vk::PipelineColorBlendAttachmentState(
            VK_FALSE,
            vk::BlendFactor::eZero,
            vk::BlendFactor::eOne,
            vk::BlendOp::eAdd,
            vk::BlendFactor::eZero,
            vk::BlendFactor::eZero,
            vk::BlendOp::eAdd,
            vk::ColorComponentFlags(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA)
        )
    };

    vk::PipelineColorBlendStateCreateInfo pbs(
        vk::PipelineColorBlendStateCreateFlags(),
        0,
        vk::LogicOp::eClear,
        static_cast<uint32_t>(colorBlendAttachments.size()),
        colorBlendAttachments.data()
    );

    vector<vk::DynamicState> dynamicStates = {
        vk::DynamicState::eViewport,
        vk::DynamicState::eScissor
    };

    vk::PipelineDynamicStateCreateInfo pdy(
        vk::PipelineDynamicStateCreateFlags(),
        static_cast<uint32_t>(dynamicStates.size()),
        dynamicStates.data()
    );

    Pipeline = static_cast<vk::Pipeline>(pContext->GetDevice()->Call().createGraphicsPipeline(
        pContext->GetPipelineCache(),
        vk::GraphicsPipelineCreateInfo(
            vk::PipelineCreateFlags(),
            static_cast<uint32_t>(pipelineShaderStages.size()),
            pipelineShaderStages.data(),
            &pvi,
            &pia,
            nullptr,
            &pv,
            &pr,
            &pm,
            &pds,
            &pbs,
            &pdy,
            PipelineLayout,
            pContext->GetSwapChain()->RenderPass,
            0
        )
    ));
}

void VKTest::SetupCommands() {
    vector<vk::ClearValue> clearValues = {
        vk::ClearColorValue(
            std::array<float, 4>{0.2f, 0.2f, 0.2f, 1.0f}),
        vk::ClearDepthStencilValue(1.0f, 0)
    };

    CommandBuffers = pContext->GetSwapChain()->GetCommandBuffers();
    for (size_t i = 0; i < CommandBuffers.size(); ++i){
        vk::CommandBuffer& cmd = CommandBuffers[i];
        cmd.reset(vk::CommandBufferResetFlagBits::eReleaseResources);
        cmd.begin(vk::CommandBufferBeginInfo());
        cmd.beginRenderPass(
            vk::RenderPassBeginInfo(
            pContext->GetSwapChain()->RenderPass,
            pContext->GetSwapChain()->GetSwapchainBuffer()[i].frameBuffer,
            pContext->GetSwapChain()->mRenderArea,
            static_cast<uint32_t>(clearValues.size()),
            clearValues.data()),
            vk::SubpassContents::eInline);

        cmd.setViewport(0, 1, &pContext->GetSwapChain()->mViewport);

        cmd.setScissor(0, 1, &pContext->GetSwapChain()->mRenderArea);

        // Bind Descriptor Sets, these are attribute/uniform "descriptions"
        cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, Pipeline);

        cmd.bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics,
            PipelineLayout,
            0,
            DescriptorSets,
            nullptr
        );

        vk::DeviceSize offsets = 0;
        cmd.bindVertexBuffers(0, 1, &Vertices.buffer, &offsets);
        cmd.bindIndexBuffer(Indices.buffer, 0, vk::IndexType::eUint32);
        cmd.drawIndexed(Indices.count, 1, 0, 0, 1);
        cmd.endRenderPass();
        cmd.end();
    }
}

void VKTest::ReloadCommandBuffer(vk::CommandBuffer &cmd) {
    vector<vk::ClearValue> clearValues = {
        vk::ClearColorValue(
            std::array<float, 4>{0.2f, 0.2f, 0.2f, 1.0f}),
        vk::ClearDepthStencilValue(1.0f, 0)
    };

    cmd.reset(vk::CommandBufferResetFlagBits::eReleaseResources);
    cmd.begin(vk::CommandBufferBeginInfo());
    cmd.beginRenderPass(
        vk::RenderPassBeginInfo(
            pContext->GetSwapChain()->RenderPass,
            pContext->GetSwapChain()->GetSwapchainBuffer()[0].frameBuffer,
            pContext->GetSwapChain()->mRenderArea,
            static_cast<uint32_t>(clearValues.size()),
            clearValues.data()),
        vk::SubpassContents::eInline);

    cmd.setViewport(0, 1, &pContext->GetSwapChain()->mViewport);

    cmd.setScissor(0, 1, &pContext->GetSwapChain()->mRenderArea);

    // Bind Descriptor Sets, these are attribute/uniform "descriptions"
    cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, Pipeline);

    cmd.bindDescriptorSets(
        vk::PipelineBindPoint::eGraphics,
        PipelineLayout,
        0,
        DescriptorSets,
        nullptr
    );

    vk::DeviceSize offsets = 0;
    cmd.bindVertexBuffers(0, 1, &Vertices.buffer, &offsets);
    cmd.bindIndexBuffer(Indices.buffer, 0, vk::IndexType::eUint32);
    cmd.drawIndexed(Indices.count, 1, 0, 0, 1);
    cmd.endRenderPass();
}

}

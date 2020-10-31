#include "VKPipeline.h"

#include "VKContext.h"
#include "VKFramebuffer.h"
#include "VKShader.h"

#include <glm/glm.hpp>

#include <Omnia/System/FileSystem.h>
#include <Platform/GFX/Vulkan/VKAllocator.h>

namespace Ultra {

static vk::Format ShaderDataTypeToVKBaseType(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::Float:			return vk::Format::eR32Sfloat;
        case ShaderDataType::Float2:		return vk::Format::eR32G32Sfloat;
        case ShaderDataType::Float3:		return vk::Format::eR32G32B32Sfloat;
        case ShaderDataType::Float4:		return vk::Format::eR32G32B32A32Sfloat;
        case ShaderDataType::Int:			return vk::Format::eR32Sint;
        case ShaderDataType::Int2:			return vk::Format::eR32G32Sint;
        case ShaderDataType::Int3:			return vk::Format::eR32G32B32Sint;
        case ShaderDataType::Int4:			return vk::Format::eR32G32B32A32Sint;
        default:							return vk::Format::eUndefined;
    }
    AppLogCritical("Unknown ShaderDataType!");
    return vk::Format::eUndefined;
}

//float: VK_FORMAT_R32_SFLOAT
//vec2: VK_FORMAT_R32G32_SFLOAT
//vec3: VK_FORMAT_R32G32B32_SFLOAT
//vec4: VK_FORMAT_R32G32B32A32_SFLOAT
//ivec2: VK_FORMAT_R32G32_SINT, a 2-component vector of 32-bit signed integers
//uvec4: VK_FORMAT_R32G32B32A32_UINT, a 4-component vector of 32-bit unsigned integers
//double: VK_FORMAT_R64_SFLOAT, a double-precision (64-bit) float

struct Vertex {
    // ToDo: alginas(16)
    glm::vec3 Position;
    glm::vec4 Color;

    static vk::VertexInputBindingDescription GetBindingDescription() {
        vk::VertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = vk::VertexInputRate::eVertex;
        return bindingDescription;
    }

    static std::array<vk::VertexInputAttributeDescription, 2> GetAttributeDescriptions() {
        std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions = {};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = vk::Format::eR32G32B32Sfloat;
        attributeDescriptions[0].offset = offsetof(Vertex, Position);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format =  vk::Format::eR32G32B32A32Sfloat;
        attributeDescriptions[1].offset = offsetof(Vertex, Color);

        return attributeDescriptions;
    }
};

static vk::Buffer VertexBuffer;
static vk::DeviceMemory VertexMemory;

const std::vector<Vertex> Vertices = {
    { {  0.0f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
    { {  0.5f,  0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
    { { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
};

VKPipeline::VKPipeline(const PipelineProperties &properties): Properties(properties) {
    auto mContext = Application::GetContext().As<VKContext>();
    mDevice = mContext->GetDevice();

    // Load Shader Source
    Properties.Shader = CreateReference<VKShader>("Assets/Shaders/Basic.glsl");

    VKShader *shaderInfo = reinterpret_cast<VKShader *>(Properties.Shader.get());
    auto shaderStages = shaderInfo->GetStages();


    auto bindingDescription = Vertex::GetBindingDescription();
    auto attributeDescriptions = Vertex::GetAttributeDescriptions();

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    vk::PipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    vk::PipelineViewportStateCreateInfo viewportState = {};
    viewportState.viewportCount = 1;
    viewportState.pViewports = &mContext->GetSwapChain()->GetSurfaceProperties().Viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &mContext->GetSwapChain()->GetSurfaceProperties().RenderArea;

    vk::PipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = vk::PolygonMode::eFill;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = vk::CullModeFlagBits::eBack;
    rasterizer.frontFace = vk::FrontFace::eClockwise;
    rasterizer.depthBiasEnable = VK_FALSE;

    vk::PipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

    vk::PipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    colorBlendAttachment.blendEnable = VK_FALSE;

    vk::PipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = vk::LogicOp::eCopy;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
    depthStencilState.depthTestEnable = VK_TRUE;
    depthStencilState.depthWriteEnable = VK_TRUE;
    depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
    depthStencilState.depthBoundsTestEnable = VK_FALSE;
    depthStencilState.back.failOp = vk::StencilOp::eKeep;
    depthStencilState.back.passOp = vk::StencilOp::eKeep;
    depthStencilState.back.compareOp = vk::CompareOp::eAlways;
    depthStencilState.stencilTestEnable = VK_FALSE;
    depthStencilState.front = depthStencilState.back;

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pushConstantRangeCount = 0;

    Layout = mDevice->Call().createPipelineLayout(pipelineLayoutInfo);

    vk::GraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDepthStencilState = &depthStencilState;
    pipelineInfo.layout = Layout;
    pipelineInfo.renderPass = mContext->GetSwapChain()->GetRenderPass();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = nullptr;

    auto result = mDevice->Call().createGraphicsPipeline(nullptr, pipelineInfo);
    Pipeline = result.value;
    Invalidate();

    CreateVertexBuffer();
}

VKPipeline::~VKPipeline() {
    mDevice->Call().destroyBuffer(VertexBuffer);
    mDevice->Call().freeMemory(VertexMemory);

    mDevice->Call().destroyPipeline(Pipeline);
    mDevice->Call().destroyPipelineLayout(Layout);
}

void VKPipeline::Bind() {
}

void VKPipeline::Bind(vk::CommandBuffer buffer) {
    vector<vk::Buffer> vertexBuffers = { VertexBuffer };
    vector<vk::DeviceSize> offsets = { 0 };
    buffer.bindVertexBuffers(0u, vertexBuffers, offsets);
    buffer.draw(static_cast<uint32_t>(Vertices.size()), 1, 0, 0);
}

void VKPipeline::Invalidate() {

}


PipelineProperties &VKPipeline::GetProperties() {
    return Properties;
}

const PipelineProperties &VKPipeline::GetProperties() const {
    return Properties;
}

vk::Pipeline VKPipeline::GetNativePipeline() {
    return Pipeline;
}

vk::PipelineLayout VKPipeline::GetNativePipelineLayout() {
    return Layout;
}


void VKPipeline::CreateVertexBuffer() {
    vk::BufferCreateInfo bufferInfo = {};
    bufferInfo.size = sizeof(Vertices[0]) * Vertices.size();
    bufferInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer;
    bufferInfo.sharingMode = vk::SharingMode::eExclusive;

    VertexBuffer = mDevice->Call().createBuffer(bufferInfo);

    VKAllocator allocator(mDevice);
    vk::MemoryRequirements memoryRequirements = mDevice->Call().getBufferMemoryRequirements(VertexBuffer);
    allocator.Allocate(memoryRequirements, &VertexMemory, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

    mDevice->Call().bindBufferMemory(VertexBuffer, VertexMemory, 0);

    void *data;
    data = mDevice->Call().mapMemory(VertexMemory, 0, bufferInfo.size, vk::MemoryMapFlagBits());
    memcpy(data, Vertices.data(), static_cast<size_t>(bufferInfo.size));
    mDevice->Call().unmapMemory(VertexMemory);
}


}

#include "VKPipeline.h"

#include "VKContext.h"
#include "VKFramebuffer.h"
#include "VKShader.h"

#include <Omnia/System/FileSystem.h>

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


VKPipeline::VKPipeline(const PipelineProperties &properties): Properties(properties) {
    auto mContext = reinterpret_cast<VKContext *>(&Application::GetContext());
    mDevice = mContext->GetDevice();

    // Load Shader Source
    auto vertexShaderCode = ReadFile("assets/shaders/Basic.vert.spv");
    auto vertexShaderModule = mDevice->Call().createShaderModuleUnique(
        vk::ShaderModuleCreateInfo(
            vk::ShaderModuleCreateFlags(),
            vertexShaderCode.size(),
            reinterpret_cast<const uint32_t *>(vertexShaderCode.data())
        )
    );

    auto fragmentShaderCode = ReadFile("assets/shaders/Basic.frag.spv");
    auto fragmentShaderModule = mDevice->Call().createShaderModuleUnique(
        vk::ShaderModuleCreateInfo(
            vk::ShaderModuleCreateFlags(),
            fragmentShaderCode.size(),
            reinterpret_cast<const uint32_t *>(fragmentShaderCode.data())
        )
    );

    vk::PipelineShaderStageCreateInfo shaderStages[] = { 
        {
            vk::PipelineShaderStageCreateFlags(),
            vk::ShaderStageFlagBits::eVertex,
            *vertexShaderModule,
            "main"
        }, 
        {
            vk::PipelineShaderStageCreateFlags(),
            vk::ShaderStageFlagBits::eFragment,
            *fragmentShaderModule,
            "main"
        } 
    };

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;

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
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
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
}

VKPipeline::~VKPipeline() {
    mDevice->Call().destroyPipeline(Pipeline);
    mDevice->Call().destroyPipelineLayout(Layout);
}

void VKPipeline::Bind() {}

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

}

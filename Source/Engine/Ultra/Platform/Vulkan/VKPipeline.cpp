#include "VKPipeline.h"

#include "VKContext.h"
#include "VKFramebuffer.h"
#include "VKShader.h"

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

static inline Omnia::VKContext *VKContext = nullptr;


VKPipeline::VKPipeline(const PipelineProperties &properties): Properties(properties) {
    Invalidate();
}

VKPipeline::~VKPipeline() {}

void VKPipeline::Bind() {}

void VKPipeline::Invalidate() {
    VKContext = reinterpret_cast<Omnia::VKContext *>(&Omnia::Application::Get().GetContext());

    auto &device = VKContext->GetDevice()->Call();

    //AppAssert(Properties.Shader, "");
    //Reference<VKShader> shader = Reference<VKShader>(Properties.Shader);
    //Reference<VKFramebuffer> framebuffer = Properties.RenderPass->GetProperties().Framebuffer.As<VKFramebuffer>();

    //vk::DescriptorSetLayout descriptorSetLayout = shader->GetDescriptorSetLayout();
    //const auto &pushConstantRanges = shader->GetPushConstantRanges();



}

PipelineProperties &VKPipeline::GetProperties() {
    return Properties;
}

const PipelineProperties &VKPipeline::GetProperties() const {
    return Properties;
}

vk::Pipeline VKPipeline::GetNativePipeline() {
    return vk::Pipeline();
}

vk::PipelineLayout VKPipeline::GetNativePipelineLayout() {
    return vk::PipelineLayout();
}

}

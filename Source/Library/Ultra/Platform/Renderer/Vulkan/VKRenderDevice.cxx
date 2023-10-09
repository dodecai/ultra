module Ultra.Platform.Renderer.VKRenderDevice;

import Ultra.Renderer;

#pragma warning(push)
#pragma warning(disable: 4100)

namespace Ultra {

VKRenderDevice::VKRenderDevice() {}

VKRenderDevice::~VKRenderDevice() {}


void VKRenderDevice::Load() {
    //sContext = reinterpret_cast<VKContext *>(&Application::GetContext());
    //mDevice = sContext->GetDevice();

    //PipelineProperties properties;
    //mPipelineA = CreateReference<VKPipeline>(properties);
};

void VKRenderDevice::BeginFrame() {};

void VKRenderDevice::EndFrame() {};

void VKRenderDevice::Dispose() {}

void VKRenderDevice::SetLineThickness(float value) {}

void VKRenderDevice::SetPolygonMode(PolygonMode mode) {}

}

#pragma warning(pop)

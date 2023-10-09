module Ultra.Platform.Renderer.SWRenderDevice;

import Ultra.Renderer;

#pragma warning(push)
#pragma warning(disable: 4100)

namespace Ultra {

SWRenderDevice::SWRenderDevice() {}

SWRenderDevice::~SWRenderDevice() {}


void SWRenderDevice::Load() {};

void SWRenderDevice::BeginFrame() {};

void SWRenderDevice::EndFrame() {};

void SWRenderDevice::Dispose() {}

void SWRenderDevice::SetLineThickness(float value) {}

void SWRenderDevice::SetPolygonMode(PolygonMode mode) {}

}

#pragma warning(pop)

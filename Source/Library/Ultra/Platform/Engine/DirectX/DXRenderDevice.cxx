module Ultra.Platform.Renderer.DXRenderDevice;

import Ultra.Engine.Renderer;

#pragma warning(push)
#pragma warning(disable: 4100)

namespace Ultra {

DXRenderDevice::DXRenderDevice() {}

DXRenderDevice::~DXRenderDevice() {}


void DXRenderDevice::Load() {};

void DXRenderDevice::BeginFrame() {};

void DXRenderDevice::EndFrame() {};

void DXRenderDevice::Dispose() {}


void DXRenderDevice::SetLineThickness(float value) {}

void DXRenderDevice::SetPolygonMode(PolygonMode mode) {}

}

#pragma warning(pop)

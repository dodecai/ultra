#pragma once

#include "Ultra/Core.h"
#include "Ultra/Renderer/Framebuffer.h"
#include "Ultra/Renderer/Texture.h"

namespace Ultra {

class ViewportPanel {
public:
    ViewportPanel();
    ViewportPanel(FramebufferProperties &properties);

    void Update();
    void Resize(uint32_t width, uint32_t height) { Buffer->Resize(width, height, false); }

    bool IsActive() { return Active; }
    bool IsEnabled() { return Enabled; }

    void BindBuffer() { Buffer->Bind(); }
    void UnbindBuffer() { Buffer->Unbind(); }

    void Activate() { Active = true; }
    void Deactivate() { Active = false; }
    void Enable() { Enabled = true; }
    void Disable() { Enabled = false; }

private:
    bool Active = true;
    bool Enabled = true;

    Reference<Texture2D> Context;
    Reference<Framebuffer> Buffer;

    glm::vec2 ViewportSize = { 1000.0f, 1000.0f }; // ToDo: Since ImGui v.179 it is importand that the size is above zero, otherwize nothing will be visible.
};

}

#include <imgui/imgui.h>

#include "Viewport.h"

namespace Ultra {


ViewportPanel::ViewportPanel() {
    Context = Texture2D::Create("./Assets/Textures/Checkerboard.png");
    FramebufferProperties fbProperties;
    Buffer = Framebuffer::Create(fbProperties);
}

ViewportPanel::ViewportPanel(FramebufferProperties &properties) {
    Context = Texture2D::Create("./Assets/Textures/Checkerboard.png");
    Buffer = Framebuffer::Create(properties);
}

void ViewportPanel::Update() {
    if (Enabled) {
        static bool viewportOpen = true;
        uint32_t rendererID = Buffer->GetColorAttachmentRendererID();

        ImGuiWindowFlags viewportFlags = ImGuiWindowFlags_NoScrollbar;

        if (ImGui::Begin("Scene", &viewportOpen, viewportFlags)) {
            ImVec2 contentRegion = ImGui::GetContentRegionMax();
            Buffer->Resize(static_cast<uint32_t>(contentRegion.x), static_cast<uint32_t>(contentRegion.y));

            ImGui::Image((void *)rendererID, contentRegion, ImVec2(0,1), ImVec2(1,0));
            Active = ImGui::IsWindowFocused() ? true : false;
            ImGui::End();
        };
    }
}

}

#include <imgui/imgui.h>

#include "Viewport.h"

namespace Ultra {

static bool ShowScene(bool *show, intptr_t rendererID) {
    static bool active = false;
    ImGuiWindowFlags viewportFlags = ImGuiWindowFlags_NoScrollbar;

    if (ImGui::Begin("Scene", show, viewportFlags)) {
        // ToDo: GetContentRegionMax returns to high y value
        ImVec2 contentRegion = ImGui::GetContentRegionMax();
        contentRegion.y -= 32;
        ImGui::Image((void *)rendererID, contentRegion, ImVec2(0,1), ImVec2(1,0));
        active = ImGui::IsWindowFocused() ? true : false;
        ImGui::End();
        return active;
    };
    ImGui::End();
    return false;
}


ViewportPanel::ViewportPanel() {
    Context = Texture2D::Create("./Assets/Textures/Checkerboard.png");
    FramebufferProperties fbProperties;
    //auto [width, height] = Application::GetWindow().GetDisplaySize();
    fbProperties.Width = 1280;
    fbProperties.Height = 1024;
    Buffer = Framebuffer::Create(fbProperties);
}

ViewportPanel::ViewportPanel(FramebufferProperties &properties) {
    Context = Texture2D::Create("./Assets/Textures/Checkerboard.png");
    Buffer = Framebuffer::Create(properties);
}

void ViewportPanel::GuiUpdate() {
    static bool viewportOpen = true;
    if (viewportOpen && Enabled) {
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        uint32_t rendererID = Buffer->GetColorAttachmentRendererID();
        Active = ShowScene(&viewportOpen, rendererID);
    } else {
        Enabled = false;
    }
}

void ViewportPanel::Update() {
    if (FramebufferProperties fbp = Buffer->GetProperties();
        ViewportSize.x > 0.0f && ViewportSize.y > 0.0f &&
        (fbp.Width != ViewportSize.x || fbp.Height != ViewportSize.y)) {

        //auto &width = Application::GetWindow().GetContexttSize().Width;
        //auto &height = Application::GetWindow().GetContexttSize().Height;
        uint32_t width = 1280;
        uint32_t height = 1024;

        //Renderer::Resize(width, height);
        //SceneCamera.Resize(height, height);
        //CurrentScene->Resize((uint32_t)ViewportSize.x, (uint32_t)ViewportSize.y);
        Buffer->Resize((uint32_t)ViewportSize.x, (uint32_t)ViewportSize.y);
    }
}

}

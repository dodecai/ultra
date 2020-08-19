#pragma once

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

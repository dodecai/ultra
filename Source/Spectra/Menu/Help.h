#pragma once

static void ShowAboutWindow(bool *show) {
    if (ImGui::Begin("About Ultra", show, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::TextColored(ImTextColorHighlight, "Ultra Spectra");
        ImGui::Text("- License: %s", "MIT");
        ImGui::Text("- Version: v%s", Ultra::AppVersion);
        ImGui::Text("This project was created for educational purposes, use it at your own risk!");

        ImGui::Separator();
        if (ImGui::CollapsingHeader("Information")) {
            bool copyToCliipboard = ImGui::Button("Copy to clipboard");
            if (copyToCliipboard) {
                ImGui::LogToClipboard();
                ImGui::LogText("```\n");
            }

            ImVec2 childSize = ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 8);
            ImGui::BeginChildFrame(ImGui::GetID("cfg_infos"), childSize, ImGuiWindowFlags_NoMove);
            ImGui::TextColored(ImTextColorHighlight, "Build Information");
            ImGui::Text("Compiler %s [%s]", APP_COMPILER, APP_COMPILER_VERSION);
            ImGui::Text("- DebugMode: %d", (int)Ultra::DebugMode);
            ImGui::Separator();
            ImGui::Text("Language Version %d", (int)__cplusplus);

            ImGui::Separator();
            ImGui::EndChildFrame();

            if (copyToCliipboard) {
                ImGui::LogText("\n```\n");
                ImGui::LogFinish();
            }
        }

        ImGui::Separator();
        if (ImGui::CollapsingHeader("3rd-Party Notes")) {
            ImVec2 childSize = ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 8);
            ImGui::BeginChildFrame(ImGui::GetID("cfg_infos2"), childSize, ImGuiWindowFlags_NoMove);
            ImGui::TextColored(ImTextColorHighlight, "Dear ImGui");
            ImGui::Text("- Information: %s" "By Omar Cornut and all Dear ImGui contributors");
            ImGui::Text("- License: %s", "MIT");
            ImGui::Text("- Version: %s", ImGui::GetVersion());
            ImGui::Separator();
            ImGui::EndChildFrame();
        }

        ImGui::End();
    }
}

static void ShowMenuHelp() {
    static bool showAbout = false;

	if (ImGui::BeginMenu("Help")) {
		ImGui::Separator();
        ImGui::MenuItem("About", NULL, &showAbout);
		ImGui::EndMenu();
	}

    if (showAbout) { ShowAboutWindow(&showAbout); };
}

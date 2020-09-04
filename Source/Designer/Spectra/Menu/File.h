#pragma once

static void ShowMenuFile() {
	if (ImGui::BeginMenu("File")) {
		ImGui::MenuItem("Load & Save", NULL, false, false);
		if (ImGui::MenuItem("New", "Ctrl+N")) {
            auto result = Omnia::Application::GetDialog().SaveFile();
        }
		if (ImGui::MenuItem("Open", "Ctrl+O")) {
            auto result = Omnia::Application::GetDialog().OpenFile();
        }
		if (ImGui::BeginMenu("Open Recent")) {
			ImGui::MenuItem("-");
			if (ImGui::BeginMenu("More..")) {
				ImGui::MenuItem("-");
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Save", "Ctrl+S")) {
            auto result = Omnia::Application::GetDialog().SaveFile();
        }
		if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S")) {
            auto result = Omnia::Application::GetDialog().SaveFile();
        }
		ImGui::Separator();
		if (ImGui::MenuItem("Exit", "Alt+F4")) { Ultra::Application::Get().Exit();  }

		ImGui::EndMenu();
	}
}

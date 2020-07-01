#pragma once

static void ShowMenuFile() {
	if (ImGui::BeginMenu("File")) {
		ImGui::MenuItem("Load & Save", NULL, false, false);
		if (ImGui::MenuItem("New", "Ctrl+N")) {}
		if (ImGui::MenuItem("Open", "Ctrl+O")) {}
		if (ImGui::BeginMenu("Open Recent")) {
			ImGui::MenuItem("-");
			if (ImGui::BeginMenu("More..")) {
				ImGui::MenuItem("-");
				if (ImGui::BeginMenu("Recurse...")) {
					ShowMenuFile();
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Save", "Ctrl+S")) {}
		if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S")) {}
		ImGui::Separator();
		if (ImGui::MenuItem("Exit", "Alt+F4")) { Ultra::Application::Get().Exit();  }

		ImGui::EndMenu();
	}
}

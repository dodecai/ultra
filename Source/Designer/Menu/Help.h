#pragma once

static bool showAbout = false;

static void ShowMenuHelp() {
	if (ImGui::BeginMenu("Help")) {
		ImGui::Separator();
		if (ImGui::MenuItem("About")) {
			showAbout = true;
		}

		ImGui::EndMenu();
	}
}

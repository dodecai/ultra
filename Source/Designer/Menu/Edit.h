
static void ShowMenuEdit() {
	if (ImGui::BeginMenu("Edit")) {
		if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
		if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}
		ImGui::Separator();
		if (ImGui::MenuItem("Cut", "CTRL+X")) {}
		if (ImGui::MenuItem("Copy", "CTRL+C")) {}
		if (ImGui::MenuItem("Paste", "CTRL+V")) {}

		ImGui::EndMenu();
	}
}

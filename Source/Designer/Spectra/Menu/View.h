#pragma once

bool ViewportRender = true;

static void ShowMenuView() {
	if (ImGui::BeginMenu("View")) {
		ImGui::Checkbox("Viewport", &ViewportRender);
		ImGui::EndMenu();
	}
}

#pragma once

static void ShowMenuTools() {
	if (ImGui::BeginMenu("Tools")) {
		ImGui::Text("Style");
		static int theme = 0;
		if (ImGui::Combo("Theme", &theme, "Dark\0Light\0\0")) {
			switch (theme) {
				case 0:		{ SetStyle(ColorThemes::Dark,	ColorPalette::Red, ColorPalette::Yellow);		break; }
				case 1:		{ SetStyle(ColorThemes::Light,	ColorPalette::Red, ColorPalette::Yellow);		break; }
				default:	{ SetRayTeakStyle();															break; }
			}
		}
		static int color = 0;
		if (ImGui::Combo("Accent Color", &color, "Default\0Red\0Pink\0Purple\0DeepPurple\0Indigo\0Blue\0LightBlue\0Cyan\0Teal\0Green\0LightGreen\0Lime\0Yellow\0Orange\0DeepOrange\0Brown\0Gray\0Amber\0\0")) {
			switch (color) {
				case 1:		{ SetStyle(ColorThemes::Dark, ColorPalette::Red, ColorPalette::Yellow);			break; }
				case 2:		{ SetStyle(ColorThemes::Dark, ColorPalette::Pink, ColorPalette::Yellow);		break; }
				case 3:		{ SetStyle(ColorThemes::Dark, ColorPalette::Purple, ColorPalette::Yellow);		break; }
				case 4:		{ SetStyle(ColorThemes::Dark, ColorPalette::DeepPurple, ColorPalette::Yellow);	break; }
				case 5:		{ SetStyle(ColorThemes::Dark, ColorPalette::Indigo, ColorPalette::Red);			break; }
				case 6:		{ SetStyle(ColorThemes::Dark, ColorPalette::Blue, ColorPalette::Red);			break; }
				case 7:		{ SetStyle(ColorThemes::Dark, ColorPalette::LightBlue, ColorPalette::Red);		break; }
				case 8:		{ SetStyle(ColorThemes::Dark, ColorPalette::Cyan, ColorPalette::Red);			break; }
				case 9:		{ SetStyle(ColorThemes::Dark, ColorPalette::Teal, ColorPalette::Red);			break; }
				case 10:	{ SetStyle(ColorThemes::Dark, ColorPalette::Green, ColorPalette::Red);			break; }
				case 11:	{ SetStyle(ColorThemes::Dark, ColorPalette::LightGreen, ColorPalette::Red);		break; }
				case 12:	{ SetStyle(ColorThemes::Dark, ColorPalette::Lime, ColorPalette::Red);			break; }
				case 13:	{ SetStyle(ColorThemes::Dark, ColorPalette::Yellow, ColorPalette::Red);			break; }
				case 14:	{ SetStyle(ColorThemes::Dark, ColorPalette::Orange, ColorPalette::Red);			break; }
				case 15:	{ SetStyle(ColorThemes::Dark, ColorPalette::DeepOrange, ColorPalette::Red);		break; }
				case 16:	{ SetStyle(ColorThemes::Dark, ColorPalette::Brown, ColorPalette::Red);			break; }
				case 17:	{ SetStyle(ColorThemes::Dark, ColorPalette::Gray, ColorPalette::Red);			break; }
				case 18:	{ SetStyle(ColorThemes::Dark, ColorPalette::Amber, ColorPalette::Red);			break; }
				default:	{ SetRayTeakStyle();															break; }
			}
		}

		if (ImGui::BeginMenu("Colors")) {
			float sz = ImGui::GetTextLineHeight();
			for (int i = 0; i < ImGuiCol_COUNT; i++) {
				const char *name = ImGui::GetStyleColorName((ImGuiCol)i);
				ImVec2 p = ImGui::GetCursorScreenPos();
				ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
				ImGui::Dummy(ImVec2(sz, sz));
				ImGui::SameLine();
				ImGui::MenuItem(name);
			}
			ImGui::EndMenu();
		}
		ImGui::Separator();

		static bool check = true;
		ImGui::Checkbox("Check", &check);

		ImGui::Separator();
		if (ImGui::BeginMenu("Renderer Settings")) {
			static bool wireframeMode = false;
			static bool verticalSync = false;
			if (ImGui::Checkbox("Wireframe", &wireframeMode)) {
				Ultra::RenderCommand::SetWireframeMode(wireframeMode);
			}
			if (ImGui::Checkbox("VSync", &verticalSync)) {
				//Ultra::Gfx::SetVSync(verticalSync);
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}
}

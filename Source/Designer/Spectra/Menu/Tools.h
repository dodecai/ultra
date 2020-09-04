#pragma once

static void ShowMenuTools() {
	if (ImGui::BeginMenu("Tools")) {
		ImGui::Text("Style");

		static ColorThemes themeSelected = ColorThemes::Dark;
		static ColorPalette primarySelected = ColorPalette::Cyan;
		static ColorPalette warnSelected = ColorPalette::Red;

		static int theme = 0;
		if (ImGui::Combo("Theme", &theme, "Dark\0Light\0\0")) {
			switch (theme) {
				case 0:		{ themeSelected = ColorThemes::Dark;	break; }
				case 1:		{ themeSelected = ColorThemes::Light;	break; }
				default:	{ SetRayTeakStyle();					break; }
			}
			SetStyle(themeSelected,	primarySelected, warnSelected);
		}
		static int color = 0;
		if (ImGui::Combo("Color", &color, "Default\0Red\0Pink\0Purple\0DeepPurple\0Indigo\0Blue\0LightBlue\0Cyan\0Teal\0Green\0LightGreen\0Lime\0Yellow\0Orange\0DeepOrange\0Brown\0Gray\0Amber\0\0")) {
			switch (color) {
				case 1:		{ primarySelected =  ColorPalette::Red;			warnSelected = ColorPalette::Yellow;	break; }
				case 2:		{ primarySelected =  ColorPalette::Pink;		warnSelected = ColorPalette::Yellow;	break; }
				case 3:		{ primarySelected =  ColorPalette::Purple;		warnSelected = ColorPalette::Yellow;	break; }
				case 4:		{ primarySelected =  ColorPalette::DeepPurple;	warnSelected = ColorPalette::Yellow;	break; }
				case 5:		{ primarySelected =  ColorPalette::Indigo;		warnSelected = ColorPalette::Red;		break; }
				case 6:		{ primarySelected =  ColorPalette::Blue;		warnSelected = ColorPalette::Red;		break; }
				case 7:		{ primarySelected =  ColorPalette::LightBlue;	warnSelected = ColorPalette::Red;		break; }
				case 8:		{ primarySelected =  ColorPalette::Cyan;		warnSelected = ColorPalette::Red;		break; }
				case 9:		{ primarySelected =  ColorPalette::Teal;		warnSelected = ColorPalette::Red;		break; }
				case 10:	{ primarySelected =  ColorPalette::Green;		warnSelected = ColorPalette::Red;		break; }
				case 11:	{ primarySelected =  ColorPalette::LightGreen;	warnSelected = ColorPalette::Red;		break; }
				case 12:	{ primarySelected =  ColorPalette::Lime;		warnSelected = ColorPalette::Red;		break; }
				case 13:	{ primarySelected =  ColorPalette::Yellow;		warnSelected = ColorPalette::Red;		break; }
				case 14:	{ primarySelected =  ColorPalette::Orange;		warnSelected = ColorPalette::Red;		break; }
				case 15:	{ primarySelected =  ColorPalette::DeepOrange;	warnSelected = ColorPalette::Red;		break; }
				case 16:	{ primarySelected =  ColorPalette::Brown;		warnSelected = ColorPalette::Red;		break; }
				case 17:	{ primarySelected =  ColorPalette::Gray;		warnSelected = ColorPalette::Red;		break; }
				case 18:	{ primarySelected =  ColorPalette::Amber;		warnSelected = ColorPalette::Red;		break; }
				default:	{ SetRayTeakStyle();																	break; }
			}
			SetStyle(themeSelected,	primarySelected, warnSelected);
		}

		if (ImGui::BeginMenu("Color Overview")) {
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

		ImGui::Separator();
		if (ImGui::BeginMenu("Renderer Settings")) {
			static bool wireframeMode = false;
			static bool verticalSync = false;
			if (ImGui::Checkbox("Wireframe", &wireframeMode)) {
				Ultra::RenderCommand::SetPolygonMode(wireframeMode ? Ultra::PolygonMode::Wireframe : Ultra::PolygonMode::Solid);
			}
			if (ImGui::Checkbox("VSync", &verticalSync)) {
				Ultra::Application::GetContext().SetVSync(verticalSync);
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}
}

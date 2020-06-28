#pragma once

static void ShowMenuTools() {
	if (ImGui::BeginMenu("Tools")) {
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
		static int style = 0;
		if (ImGui::Combo("Accent Color", &style, "Default\0Red\0Pink\0Purple\0DeepPurple\0Indigo\0Blue\0LightBlue\0Cyan\0Teal\0Green\0LightGreen\0Lime\0Yellow\0Orange\0DeepOrange\0Brown\0Gray\0Amber\0\0")) {
			switch (style) {
				case 1:		{ SetStyle(ColorPalette::BlueGray, ColorPalette::Red, ColorPalette::Yellow);		break; }
				case 2:		{ SetStyle(ColorPalette::BlueGray, ColorPalette::Pink, ColorPalette::Yellow);		break; }
				case 3:		{ SetStyle(ColorPalette::BlueGray, ColorPalette::Purple, ColorPalette::Yellow);		break; }
				case 4:		{ SetStyle(ColorPalette::BlueGray, ColorPalette::DeepPurple, ColorPalette::Yellow);	break; }
				case 5:		{ SetStyle(ColorPalette::BlueGray, ColorPalette::Indigo, ColorPalette::Red);		break; }
				case 6:		{ SetStyle(ColorPalette::BlueGray, ColorPalette::Blue, ColorPalette::Red);			break; }
				case 7:		{ SetStyle(ColorPalette::BlueGray, ColorPalette::LightBlue, ColorPalette::Red);		break; }
				case 8:		{ SetStyle(ColorPalette::BlueGray, ColorPalette::Cyan, ColorPalette::Red);			break; }
				case 9:		{ SetStyle(ColorPalette::BlueGray, ColorPalette::Teal, ColorPalette::Red);			break; }
				case 10:	{ SetStyle(ColorPalette::BlueGray, ColorPalette::Green, ColorPalette::Red);			break; }
				case 11:	{ SetStyle(ColorPalette::BlueGray, ColorPalette::LightGreen, ColorPalette::Red);	break; }
				case 12:	{ SetStyle(ColorPalette::BlueGray, ColorPalette::Lime, ColorPalette::Red);			break; }
				case 13:	{ SetStyle(ColorPalette::BlueGray, ColorPalette::Yellow, ColorPalette::Red);		break; }
				case 14:	{ SetStyle(ColorPalette::BlueGray, ColorPalette::Orange, ColorPalette::Red);		break; }
				case 15:	{ SetStyle(ColorPalette::BlueGray, ColorPalette::DeepOrange, ColorPalette::Red);	break; }
				case 16:	{ SetStyle(ColorPalette::BlueGray, ColorPalette::Brown, ColorPalette::Red);			break; }
				case 17:	{ SetStyle(ColorPalette::BlueGray, ColorPalette::Gray, ColorPalette::Red);			break; }
				case 18:	{ SetStyle(ColorPalette::BlueGray, ColorPalette::Amber, ColorPalette::Red);			break; }
				default:	{ SetRayTeakStyle();																break; }
			}
		}
		static bool check = true;
		ImGui::Checkbox("Check", &check);

		ImGui::EndMenu();
	}
}

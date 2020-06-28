#include "MaterialDesign.h"

static ColorSwatch &GetColorPalette(ColorPalette color) {
	switch (color) {
		case ColorPalette::Red:			{ Red50 result;			return result;	}
		case ColorPalette::Pink:		{ Pink50 result;		return result;	}
		case ColorPalette::Purple:		{ Purple50 result;		return result;	}
		case ColorPalette::DeepPurple:	{ DeepPurple50 result;	return result;	}
		case ColorPalette::Indigo:		{ Indigo50 result;		return result;	}
		case ColorPalette::Blue:		{ Blue50 result;		return result;	}
		case ColorPalette::LightBlue:	{ LightBlue50 result;	return result;	}
		case ColorPalette::Cyan:		{ Cyan50 result;		return result;	}
		case ColorPalette::Teal:		{ Teal50 result;		return result;	}
		case ColorPalette::Green:		{ Green50 result;		return result;	}
		case ColorPalette::LightGreen:	{ LightGreen50 result;	return result;	}
		case ColorPalette::Lime:		{ Lime50 result;		return result;	}
		case ColorPalette::Yellow:		{ Yellow50 result;		return result;	}
		case ColorPalette::Amber:		{ Amber50 result;		return result;	}
		case ColorPalette::Orange:		{ Orange50 result;		return result;	}
		case ColorPalette::DeepOrange:	{ DeepOrange50 result;	return result;	}
		case ColorPalette::Brown:		{ Brown50 result;		return result;	}
		case ColorPalette::Gray:		{ Gray50 result;		return result;	}
		case ColorPalette::BlueGray:	{ BlueGray50 result;	return result;	}
		case ColorPalette::Black:		{ Black50 result;		return result;	}
		case ColorPalette::White:		{ White50 result;		return result;	}
	}
}

static void SetStyle() {
	ImGuiStyle &style = ImGui::GetStyle();
	ImVec4 *colors = style.Colors;

	//ColorSwatch scheme = GetColorPalette(ColorPalette::BlueGray);

	//app::alog << "Hex:" << scheme.base << "\n";

	//ColorRgba test = ConvertColorRgba(scheme.base);
	//app::alog << "Red:" << test.r << " Green:" << test.g << " Blue:" << test.b << " Alpha:" << test.a << "\n";
}

static void SetDefaultStyle() {
	ImGuiStyle &style = ImGui::GetStyle();
	ImVec4 *colors = style.Colors;

	Teal50 tester;
	//SetStyle();

	BlueGray50 scheme;
	std::cout << "Hex:" << scheme.base << "\n";

	ColorRgba test = ConvertColorRgba(scheme.base);
	std::cout << "Red:" << test.r << " Green:" << test.g << " Blue:" << test.b << " Alpha:" << test.a << "\n";
	
	//colors[ImGuiCol_TitleBg] = ConvertColorRgbaVec(scheme.s400);
	colors[ImGuiCol_Text]					= ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
	colors[ImGuiCol_TextDisabled]			= ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	colors[ImGuiCol_WindowBg]				= ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
	colors[ImGuiCol_ChildBg]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg]				= ImVec4(0.11f, 0.11f, 0.14f, 0.92f);
	colors[ImGuiCol_Border]					= ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow]			= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg]				= ImVec4(0.43f, 0.43f, 0.43f, 0.39f);
	colors[ImGuiCol_FrameBgHovered]			= ImVec4(0.47f, 0.47f, 0.69f, 0.40f);
	colors[ImGuiCol_FrameBgActive]			= ImVec4(0.42f, 0.41f, 0.64f, 0.69f);
	colors[ImGuiCol_TitleBg]				= ImVec4(0.27f, 0.27f, 0.54f, 0.83f);
	colors[ImGuiCol_TitleBgActive]			= ImVec4(0.32f, 0.32f, 0.63f, 0.87f);
	colors[ImGuiCol_TitleBgCollapsed]		= ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
	colors[ImGuiCol_MenuBarBg]				= ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
	colors[ImGuiCol_ScrollbarBg]			= ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
	colors[ImGuiCol_ScrollbarGrab]			= ImVec4(0.40f, 0.40f, 0.80f, 0.30f);
	colors[ImGuiCol_ScrollbarGrabHovered]	= ImVec4(0.40f, 0.40f, 0.80f, 0.40f);
	colors[ImGuiCol_ScrollbarGrabActive]	= ImVec4(0.41f, 0.39f, 0.80f, 0.60f);
	colors[ImGuiCol_CheckMark]				= ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
	colors[ImGuiCol_SliderGrab]				= ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	colors[ImGuiCol_SliderGrabActive]		= ImVec4(0.41f, 0.39f, 0.80f, 0.60f);
	colors[ImGuiCol_Button]					= ImVec4(0.35f, 0.40f, 0.61f, 0.62f);
	colors[ImGuiCol_ButtonHovered]			= ImVec4(0.40f, 0.48f, 0.71f, 0.79f);
	colors[ImGuiCol_ButtonActive]			= ImVec4(0.46f, 0.54f, 0.80f, 1.00f);
	colors[ImGuiCol_Header]					= ImVec4(0.40f, 0.40f, 0.90f, 0.45f);
	colors[ImGuiCol_HeaderHovered]			= ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
	colors[ImGuiCol_HeaderActive]			= ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
	colors[ImGuiCol_Separator]				= ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
	colors[ImGuiCol_SeparatorHovered]		= ImVec4(0.60f, 0.60f, 0.70f, 1.00f);
	colors[ImGuiCol_SeparatorActive]		= ImVec4(0.70f, 0.70f, 0.90f, 1.00f);
	colors[ImGuiCol_ResizeGrip]				= ImVec4(1.00f, 1.00f, 1.00f, 0.16f);
	colors[ImGuiCol_ResizeGripHovered]		= ImVec4(0.78f, 0.82f, 1.00f, 0.60f);
	colors[ImGuiCol_ResizeGripActive]		= ImVec4(0.78f, 0.82f, 1.00f, 0.90f);
	colors[ImGuiCol_Tab]					= ImVec4(0.40f, 0.40f, 0.90f, 0.45f);
	colors[ImGuiCol_TabHovered]				= ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
	colors[ImGuiCol_TabActive]				= ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
	colors[ImGuiCol_TabUnfocused]			= ImVec4(0.40f, 0.40f, 0.90f, 0.45f);
	colors[ImGuiCol_TabUnfocusedActive]		= ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
	colors[ImGuiCol_PlotLines]				= ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered]		= ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram]			= ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]	= ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg]			= ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	colors[ImGuiCol_DragDropTarget]			= ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight]			= ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
	colors[ImGuiCol_NavWindowingHighlight]	= ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg]		= ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg]		= ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

#ifdef IMGUI_HAS_DOCK
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.85f, 0.85f, 0.85f, 0.28f);
#endif

	style.Alpha						= 1.0f;

	style.WindowPadding				= ImVec2(8, 8);
	style.WindowRounding			= 2.0f;
	style.WindowBorderSize			= 1.0f;
	style.WindowMinSize				= ImVec2(32, 32);
	style.WindowTitleAlign			= ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition	= ImGuiDir_None;

	style.ChildRounding				= 0.0f;
	style.ChildBorderSize			= 2.0f;
	style.PopupRounding				= 0.0f;
	style.PopupBorderSize			= 2.0f;
	style.FramePadding				= ImVec2(4, 4);
	style.FrameRounding				= 0.0f;
	style.FrameBorderSize			= 2.0f;

	style.ItemSpacing				= ImVec2(6, 4);
	style.ItemInnerSpacing			= ImVec2(4, 4);
	style.TouchExtraPadding			= ImVec2(2, 2);
	style.IndentSpacing				= 21.0f;
	style.ColumnsMinSpacing			= 6.0f;
	style.ScrollbarSize				= 12.0f;
	style.ScrollbarRounding			= 2.0f;

	style.GrabMinSize				= 12.0f;
	style.GrabRounding				= 0.0f;
	style.TabRounding				= 4.0f;
	style.TabBorderSize				= 0.0f;
	style.TabMinWidthForUnselectedCloseButton	= 0.0f;
	style.ColorButtonPosition		= ImGuiDir_Left;
	style.ButtonTextAlign			= ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign		= ImVec2(0.0f, 0.0f);
	style.DisplayWindowPadding		= ImVec2(19, 19);
	style.DisplaySafeAreaPadding	= ImVec2(2, 2);
	style.MouseCursorScale			= 1.0f;
	style.AntiAliasedLines			= true;
	style.AntiAliasedFill			= true;
	style.CurveTessellationTol		= 1.25f;
	style.CircleSegmentMaxError		= 1.60f;

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	style.ScaleAllSizes(1.25f);
}

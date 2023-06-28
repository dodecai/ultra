#pragma once

#include <imgui/imgui.h>

#include "MaterialDesign.h"

inline ColorThemes GColorTheme;
inline ImVec4 ImTextColorHighlight = {};
inline ImVec4 ImTextColorHighlightWarn = {};

static ImVec4 GetTextColorHighlight() {
    return ImTextColorHighlight;
}

static ImVec4 GetTextColorHighlightWarn() {
    return ImTextColorHighlightWarn;
}

static void SetDefaultStyle() {
	ImGuiStyle &style = ImGui::GetStyle();
	ImVec4 *colors = style.Colors;

	colors[ImGuiCol_Text]					= ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled]			= ImVec4(0.72f, 0.72f, 0.72f, 1.00f);
	colors[ImGuiCol_WindowBg]				= ImVec4(0.12f, 0.12f, 0.12f, 0.72f);
	colors[ImGuiCol_ChildBg]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg]				= ImVec4(0.09f, 0.09f, 0.09f, 0.92f);
	colors[ImGuiCol_Border]					= ImVec4(0.24f, 0.24f, 0.24f, 0.50f);
	colors[ImGuiCol_BorderShadow]			= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg]				= ImVec4(0.48f, 0.48f, 0.48f, 0.32f);
	colors[ImGuiCol_FrameBgHovered]			= ImVec4(0.64f, 0.64f, 0.64f, 0.48f);
	colors[ImGuiCol_FrameBgActive]			= ImVec4(0.40f, 0.40f, 0.64f, 0.72f);
	colors[ImGuiCol_TitleBg]				= ImVec4(0.24f, 0.24f, 0.24f, 0.92f);
	colors[ImGuiCol_TitleBgActive]			= ImVec4(0.32f, 0.32f, 0.32f, 0.92f);
	colors[ImGuiCol_TitleBgCollapsed]		= ImVec4(0.48f, 0.48f, 0.48f, 0.72f);
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
//	style.TabMinWidthForUnselectedCloseButton	= 0.0f;
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

	style.WindowPadding = ImVec2(15, 15);
	style.WindowRounding = 5.0f;
	style.FramePadding = ImVec2(5, 5);
	style.FrameRounding = 4.0f;
	style.ItemSpacing = ImVec2(12, 8);
	style.ItemInnerSpacing = ImVec2(8, 6);
	style.IndentSpacing = 25.0f;
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 5.0f;
	style.GrabRounding = 3.0f;

	style.ScaleAllSizes(1.0f);
}

static void SetStyle(ColorThemes theme = ColorThemes::Dark, ColorPalette accentPalette = ColorPalette::Blue, ColorPalette warnPalette = ColorPalette::Red) {
	// Preparation
	ImGuiStyle &style = ImGui::GetStyle();
	ImVec4 *colors = style.Colors;

	std::shared_ptr<ColorTheme> appTheme = GetColorTheme(theme);
	std::shared_ptr<ColorSwatch> primary = GetColorPalette(ColorPalette::BlueGray);
	std::shared_ptr<ColorSwatch> accent = GetColorPalette(accentPalette);
	std::shared_ptr<ColorSwatch> warn = GetColorPalette(warnPalette);
	GColorTheme = theme;

	/**
	 *	Styles
	*/
	ImVec4 test = ConvertColorRgbaVec(appTheme->text);
	//static ImVec4 accentTextColor = { 1.0f, 1.0f, 0.0f, 1.0f };
	ImTextColorHighlight = ConvertColorRgbaVec(accent->s500);
	ImTextColorHighlightWarn = ConvertColorRgbaVec(warn->s500);

	/* Colors */
	// Global
	colors[ImGuiCol_Text]					= ConvertColorRgbaVec(appTheme->text);
	colors[ImGuiCol_TextDisabled]			= ConvertColorRgbaVec(appTheme->disabled);
	colors[ImGuiCol_TextSelectedBg]			= ConvertColorRgbaVec(accent->s200, 0.32f);
	colors[ImGuiCol_Border]					= ConvertColorRgbaVec(accent->s200);
	colors[ImGuiCol_BorderShadow]			= ConvertColorRgbaVec(accent->s500);
	colors[ImGuiCol_FrameBg]				= ConvertColorRgbaVec(appTheme->surface);
	colors[ImGuiCol_FrameBgHovered]			= ConvertColorRgbaVec(accent->s200, 0.32f);
	colors[ImGuiCol_FrameBgActive]			= ConvertColorRgbaVec(accent->s300, 0.32f);
	colors[ImGuiCol_ModalWindowDimBg]		= ConvertColorRgbaVec(appTheme->background, 0.72f);
	colors[ImGuiCol_ModalWindowDarkening]	= ConvertColorRgbaVec(appTheme->background, 0.72f);

	// Window
	colors[ImGuiCol_WindowBg]				= ConvertColorRgbaVec(appTheme->background, 0.92f);
	colors[ImGuiCol_MenuBarBg]				= ConvertColorRgbaVec(appTheme->surface);
	colors[ImGuiCol_ChildBg]				= ConvertColorRgbaVec(appTheme->background, 0.92f);
	colors[ImGuiCol_PopupBg]				= ConvertColorRgbaVec(appTheme->background, 0.92f);
	colors[ImGuiCol_DragDropTarget]			= ConvertColorRgbaVec(appTheme->background, 0.72f);
	#ifdef IMGUI_HAS_DOCK
		colors[ImGuiCol_DockingEmptyBg]		= { 0.0f, 0.0f, 0.0f, 0.0f };
		colors[ImGuiCol_DockingPreview]		= ConvertColorRgbaVec(accent->s200, 0.72f);
	#endif
	colors[ImGuiCol_TitleBg]				= ConvertColorRgbaVec(appTheme->surface);
	colors[ImGuiCol_TitleBgActive]			= ConvertColorRgbaVec(accent->s200, 0.32f);
	colors[ImGuiCol_TitleBgCollapsed]		= ConvertColorRgbaVec(appTheme->surface, 0.72f);
	colors[ImGuiCol_Tab]					= ConvertColorRgbaVec(accent->s500, 0.32f);
	colors[ImGuiCol_TabHovered]				= ConvertColorRgbaVec(accent->s200, 0.32f);
	colors[ImGuiCol_TabActive]				= ConvertColorRgbaVec(accent->s500, 0.32f);
	colors[ImGuiCol_TabUnfocused]			= ConvertColorRgbaVec(accent->s300, 0.32f);
	colors[ImGuiCol_TabUnfocusedActive]		= ConvertColorRgbaVec(accent->s500, 0.32f);

	// Controls
	colors[ImGuiCol_Header]					= ConvertColorRgbaVec(appTheme->surface);
	colors[ImGuiCol_HeaderHovered]			= ConvertColorRgbaVec(accent->s200, 0.32f);
	colors[ImGuiCol_HeaderActive]			= ConvertColorRgbaVec(accent->s500);
    //~
	colors[ImGuiCol_CheckMark]				= ConvertColorRgbaVec(accent->s500);
    //~
	colors[ImGuiCol_SliderGrab]				= ConvertColorRgbaVec(accent->s500);
	colors[ImGuiCol_SliderGrabActive]		= ConvertColorRgbaVec(accent->s700);
    //~
	colors[ImGuiCol_Button]					= ConvertColorRgbaVec(appTheme->surface);
	colors[ImGuiCol_ButtonHovered]			= ConvertColorRgbaVec(accent->s200, 0.32f);
	colors[ImGuiCol_ButtonActive]			= ConvertColorRgbaVec(accent->s300, 0.32f);
	//~
	colors[ImGuiCol_NavHighlight]			= ConvertColorRgbaVec(primary->s700);
	colors[ImGuiCol_NavWindowingHighlight]	= ConvertColorRgbaVec(primary->s700);
	colors[ImGuiCol_NavWindowingDimBg]		= ConvertColorRgbaVec(appTheme->background, 0.72f);
    //~
	colors[ImGuiCol_ScrollbarBg]			= ConvertColorRgbaVec(appTheme->surface);
	colors[ImGuiCol_ScrollbarGrab]			= ConvertColorRgbaVec(accent->s500);
	colors[ImGuiCol_ScrollbarGrabHovered]	= ConvertColorRgbaVec(accent->s300);
	colors[ImGuiCol_ScrollbarGrabActive]	= ConvertColorRgbaVec(accent->s700);
    //~
	colors[ImGuiCol_Separator]				= ConvertColorRgbaVec(appTheme->surface);
	colors[ImGuiCol_SeparatorHovered]		= ConvertColorRgbaVec(accent->s200, 0.32f);
	colors[ImGuiCol_SeparatorActive]		= ConvertColorRgbaVec(accent->s300, 0.32f);
    //~
	colors[ImGuiCol_PlotLines]				= ConvertColorRgbaVec(accent->s500);
	colors[ImGuiCol_PlotLinesHovered]		= ConvertColorRgbaVec(accent->s700);
	colors[ImGuiCol_PlotHistogram]			= ConvertColorRgbaVec(accent->s500);
	colors[ImGuiCol_PlotHistogramHovered]	= ConvertColorRgbaVec(accent->s700);
    //~
	colors[ImGuiCol_ResizeGrip]				= ConvertColorRgbaVec(appTheme->surface);
	colors[ImGuiCol_ResizeGripHovered]		= ConvertColorRgbaVec(accent->s200, 0.32f);
	colors[ImGuiCol_ResizeGripActive]		= ConvertColorRgbaVec(accent->s300, 0.32f);
	
	/* Rendering */
	style.AntiAliasedLines			= true;
	style.AntiAliasedFill			= true;
	style.CurveTessellationTol		= 1.25f;
	style.CircleSegmentMaxError		= 1.60f;
	style.Alpha						= 1.0f;

	/* Sizes */
	// Main
	style.WindowPadding				= { 12.0f, 12.0f };
	style.FramePadding				= { 6.0f, 6.0f };;
	style.ItemSpacing				= { 12.0f, 8.0f };
	style.ItemInnerSpacing			= { 6.0f, 6.0f };
	style.TouchExtraPadding			= { 0, 0 };
	style.IndentSpacing				= 24.0f;
	style.ScrollbarSize				= 12.0f;
	style.GrabMinSize				= 12.0f;

	// Borders
	style.WindowBorderSize			= 1.0f;
	style.ChildBorderSize			= 0.0f;
	style.PopupBorderSize			= 1.0f;
	style.FrameBorderSize			= 0.0f;
	style.TabBorderSize				= 0.0f;

	// Rounding
	style.WindowRounding			= 2.0f;
	style.ChildRounding				= 2.0f;
	style.FrameRounding				= 4.0f;
	style.PopupRounding				= 2.0f;
	style.ScrollbarRounding			= 9.0f;
	style.GrabRounding				= 4.0f;
	style.TabRounding				= 2.0f;

	// Alignment
	style.WindowTitleAlign			= { 0.0f, 0.5f };
	style.WindowMenuButtonPosition	= ImGuiDir_Left; // ToDo: ImGuiDir_None: Not working as expected, the button is behind the text and can be clicked...
	style.ColorButtonPosition		= ImGuiDir_Left;
	style.ButtonTextAlign			= { 0.5f, 0.5f };
	style.SelectableTextAlign		= { 0.0f, 0.0f };
	
	// Miscellaneous
	//style.DisplayWindowPadding		= { 19.0f, 19.0f };
	style.DisplaySafeAreaPadding	= { 0.0f, 0.0f };
	//style.ColumnsMinSpacing			= 6.0f;
	//style.TabMinWidthForUnselectedCloseButton	= 0.0f;


	// Global
	style.MouseCursorScale			= 3.0f;
	//style.WindowMinSize				= { 32.0f, 32.0f };

	// Scaling
	style.ScaleAllSizes(1.0f);
}

static void SetRayTeakStyle() {
	ImGuiStyle * style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	//style->Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	//style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	//style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	//style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
	//style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
	//style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
}
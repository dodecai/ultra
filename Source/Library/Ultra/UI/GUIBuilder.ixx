module;

// Hack: The included header unit in the Core module doesn't work here.
#include "Ultra/Core/Core.h"

///
/// @brief This module contains the needed backends from DearImGui
///
export module Ultra.UI.GUIBuilder;

import Ultra.Core;

// We are not interested in warnings related to DearImGui
#pragma warning(push, 0)

// Graphics Backends
export import <imgui/backends/imgui_impl_opengl3.h>;
export import <imgui/backends/imgui_impl_vulkan.h>;

// System Backends
#ifdef APP_PLATFORM_WINDOWS
    export import <imgui/backends/imgui_impl_win32-patch.h>;
#endif

#pragma warning(pop)

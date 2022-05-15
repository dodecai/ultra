#pragma once

#include "Omnia/Omnia.pch"

// We are not interested in warnings related to DearImGui
#pragma warning(push, 0)

#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_vulkan-patch.h>
#include <imgui/backends/imgui_impl_win32-patch.h>

#pragma warning(pop)

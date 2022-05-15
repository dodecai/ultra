#include "GuiBuilder.h"

// We are not interested in warnings related to DearImGui
#pragma warning(push, 0)

#define IMGUI_IMPL_OPENGL_LOADER_GLAD2

#include <imgui/backends/imgui_impl_opengl3.cpp>
#include <imgui/backends/imgui_impl_vulkan-patch.cpp>
#include <imgui/backends/imgui_impl_win32-patch.cpp>

#pragma warning(pop)

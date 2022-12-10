module;

// Properties
#define __SPECSTRINGS_STRICT_LEVEL 0    // Hack: Workaround for problems with 'Windows.h' in combination with C++ modules!
#define IMGUI_IMPL_OPENGL_LOADER_GLAD2

// Library
#include <Ultra/Core/Private/Core.h>

// Graphics Backends
#include <imgui/backends/imgui_impl_opengl3.cpp>
//#include <imgui/backends/imgui_impl_vulkan-patch.cpp>


// System Backends
#ifdef APP_PLATFORM_WINDOWS
    #include <imgui/backends/imgui_impl_win32-patch.cpp>
#endif

module Ultra.UI.GUIBuilder;

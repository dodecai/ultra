project "DearImGui"
	kind "StaticLib"
	language "C++"
    characterset "Unicode"
    conformancemode "true"
    cdialect "C17"
	cppdialect "C++20"
    staticruntime "on"
    toolset "msc"
    warnings "Default"

	files {
        "DearImGui.lua",
		"include/**",
		"source/*",
	}

    includedirs {
        "include/imgui"
    }

    filter { "configurations:Debug" }
        defines { "_DEBUG" }
		runtime "Debug"
        symbols "On"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
		runtime "Release"
        optimize "On"
        symbols "Off"

    filter { "configurations:Distribution" }
        defines { "NDEBUG" }
        optimize "On"
        symbols "Off"

	filter "system:windows"
		systemversion "latest"

project "yaml-cpp"
	kind "StaticLib"
	language "C++"
    characterset "MBCS"
    conformancemode "true"
    cdialect "C17"
	cppdialect "C++17"
    staticruntime "on"
    toolset "msc"
    warnings "Default"

	files {
        "yaml-cpp.lua",
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

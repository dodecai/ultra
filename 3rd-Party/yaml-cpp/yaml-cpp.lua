project "yaml-cpp"
	kind "StaticLib"
	language "C++"
    characterset "Unicode"
    conformancemode "true"
    cdialect "C17"
	cppdialect "C++20"
    staticruntime "on"
    toolset "msc"
    warnings "Default"

    defines {
        "YAML_CPP_STATIC_DEFINE",
        "YAML_CPP_NO_CONTRIB",
    }

	files {
        "include/**",
        "source/**",
        "yaml-cpp.lua",
	}
    
    includedirs {
        "include",
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

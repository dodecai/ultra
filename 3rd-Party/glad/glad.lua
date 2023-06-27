project "glad"
	kind "StaticLib"
	language "C"
    cdialect "C17"
    staticruntime "on"
    toolset "msc"
    warnings "Default"

	files {
        "glad.lua",
        "gl.h",
        "gl.c"
	}

    includedirs {
		"%{Headers.ThirdParty}"
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


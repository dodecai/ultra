project "MSDFgen"
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
        "MSDFGEN_CORE_ONLY",
        "MSDFGEN_DISABLE_SVG",
		"MSDFGEN_USE_CPP11",
        "MSDFGEN_USE_OPENMP",
	}

	files {
		"include/**",
		"source/**.cpp",
        "MSDFgen.lua",
	}

    includedirs {
		"include",
	    "%{Headers.FreeType}",
    }

    links {
		"freetype"
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

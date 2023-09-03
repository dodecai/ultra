project "MSDF-Atlas-Gen"
	kind "StaticLib"
	language "C++"
    conformancemode "true"
    cdialect "C17"
	cppdialect "C++20"
    staticruntime "on"
    toolset "msc"
    warnings "Default"
    disablewarnings { "4267", "4996" }

    defines {
        "MSDFGEN_USE_CPP11",
        "MSDF_ATLAS_PUBLIC=",
        "MSDF_ATLAS_NO_ARTERY_FONT",
    }

	files {
        "include/**",
        "source/**.cpp",
        "MSDF-Atlas-Gen.lua",
	}

    includedirs {
        "include/msdf-atlas-gen",
	    "%{Headers.MSDFgen}/msdfgen",
    }

    links {
		"msdfgen"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
        symbols "off"

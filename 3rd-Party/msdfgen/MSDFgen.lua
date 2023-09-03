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

	files {
		"include/**",
        "MSDFgen.lua"
	}

    includedirs {
		"include"
    }

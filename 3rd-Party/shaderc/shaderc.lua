project "shaderc"
	kind "None"
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
        "shaderc.lua",
	}

    includedirs {
        "include",
    }

project "stb"
	kind "StaticLib"
	language "C"
    conformancemode "true"
    cdialect "C17"
    staticruntime "on"
    toolset "msc"
    warnings "Default"

	files {
        "stb.lua",
		"stb*.h",
	}

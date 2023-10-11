project "vulkan"
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
        "vulkan.lua",
	}

    includedirs {
        "include",
        "../vk_video/include",
    }

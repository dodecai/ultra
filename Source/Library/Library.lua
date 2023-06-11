project "Ultra"
    defines { "PROJECT_NAME=Ultra" }
    kind "StaticLib"
    language "C++"
    characterset "Unicode"
    conformancemode "true"
    cdialect "C17"
    cppdialect "C++20"
    cppmodules "true"
    nativewchar "on"
    scanformoduledependencies "on"
    staticruntime "on"
    toolset "msc"
    
    debugdir "%{wks.location}/Build/%{cfg.buildcfg}"
    files { "**.h", "**.cpp", "**.cppm", "**.cxx", "**.inl", "**.ixx" }
    
    externalincludedirs {
	    "%{Headers.ThirdParty}",
	    "%{Headers.assimp}",
	    "%{Headers.DearImGui}",
	    "%{Headers.EnTT}",
	    "%{Headers.glm}",
        "%{Headers.stb}",
	    "%{Headers.Vulkan}",
	    "%{Headers.VulkanVideo}",
    }

    includedirs {
	    "%{Headers.Library}",
    }

    links { 
        --"assimp",
        "DearImGui",
        "Glad",
	    "%{Library.Vulkan}",
	    --"%{Library.VulkanUtils}",
    }

    filter { "configurations:Debug" }
        defines { "_DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        optimize "On"
        symbols "On"
    
    filter { "configurations:Distribution" }
        defines { "NDEBUG" }
        optimize "On"
        symbols "Off"
    
    filter { }

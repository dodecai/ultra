project "Ultra"
    defines { "PROJECT_NAME=Ultra" }
    kind "StaticLib"
    language "C++"
    characterset "Unicode"
    conformancemode "true"
    cdialect "C17"
    cppdialect "C++latest"
    cppmodules "true"
    nativewchar "on"
    scanformoduledependencies "off"
    staticruntime "on"
    toolset "msc"
    
    debugdir "%{wks.location}/Build/%{cfg.buildcfg}"
    files { "**.h", "**.cpp", "**.cppm", "**.cxx", "**.inl", "**.ixx" }
    
    externalincludedirs {
	    "%{Headers.ThirdParty}",
	    "%{Headers.DearImGui}",
	    "%{Headers.EnTT}",
	    "%{Headers.Vulkan}",
	    "%{Headers.VulkanVideo}",
    }

    includedirs {
	    "%{Headers.Library}",
    }

    links { 
        "DearImGui",
        "Glad",
	    "%{Library.Vulkan}",
	    "%{Library.VulkanUtils}",
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

project "Ultra"
    defines { "PROJECT_NAME=Ultra" }
    kind "StaticLib"
    language "C++"
    characterset "Unicode"
    conformancemode "true"
    cdialect "C17"
    cppdialect "C++20"
    cppmodules "true"
    externalanglebrackets "on"
    externalwarnings "Default"
    nativewchar "on"
    scanformoduledependencies "on"
    staticruntime "on"
    toolset "msc"
    warnings "Extra"
    
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
        runtime "Debug"
        --sanitize { "Address", "Fuzzer" }
        symbols "on"
    
    filter { "configurations:Distribution" }
        defines { "NDEBUG" }
        optimize "on"
        runtime "Release"
        --sanitize { "Address", "Fuzzer" }
        symbols "on"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        optimize "on"
        runtime "Release"
        symbols "off"
    
    filter { }

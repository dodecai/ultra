project "Ultra"
    defines { "PROJECT_NAME=Ultra" }
    kind "StaticLib"
    language "C++"
    characterset "Unicode"
    conformancemode "true"
    cdialect "C17"
    cppdialect "C++latest"
    cppmodules "true"
    buildstlmodules "true"
    externalanglebrackets "on"
    externalwarnings "Default"
    nativewchar "on"
    scanformoduledependencies "on"
    staticruntime "on"
    toolset "msc"
    warnings "Extra"
    
    debugdir "%{wks.location}/Build/%{cfg.buildcfg}"
    files { "**.h", "**.cpp", "**.cppm", "**.cxx", "**.inl", "**.ixx" }
    postbuildcommands {
        "copy /b /y \"%{Package.ShaderC}\" \"%{cfg.targetdir}/\"",
        "exit /b 0",
    }
    
    externalincludedirs {
	    "%{Headers.assimp}",
	    "%{Headers.DearImGui}",
	    "%{Headers.EnTT}",
	    "%{Headers.glad}",
	    "%{Headers.glm}",
        "%{Headers.yamlcpp}",
        
        "%{Headers.ShaderC}",
        "%{Headers.Vulkan}",
        "%{Headers.VulkanVideo}",

        "%{Headers.LibPHX}",
        "%{Headers.LibPHXext}",
    }
    includedirs {
	    "%{Headers.Library}",
    }
    links {
        --"assimp",
        "DearImGui",
        "Glad",
        "yaml-cpp",
        "LibPHX",
        "%{Library.ShaderC}",
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

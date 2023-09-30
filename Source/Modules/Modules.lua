project "Modules"
    defines { "PROJECT_NAME=Modules" }
    kind "StaticLib"
    language "C++"
    characterset "Unicode"
    conformancemode "true"
    cdialect "C17"
    cppdialect "C++latest"
    cppmodules "true"
    buildstlmodules "true"
    externalanglebrackets "on"
    externalwarnings "Off"
    nativewchar "on"
    scanformoduledependencies "on"
    staticruntime "on"
    toolset "msc"
    warnings "Extra"

    debugdir "%{wks.location}/Build/%{cfg.buildcfg}"
    dependson { "Ultra" }
    files { "**.h", "**.cpp", "**.cppm", "**.cxx", "**.inl", "**.ixx", "**.lua" }
    
    externalincludedirs {
        "%{Headers.ThirdParty}",

        "%{Headers.LibPHX}",
        "%{Headers.LibPHXext}",
    }
    includedirs {
        "%{Headers.Library}",
    }
    links {
        "Ultra",
        "LibPHX",
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

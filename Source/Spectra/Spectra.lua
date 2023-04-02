project "Spectra"
    defines { "PROJECT_NAME=Spectra" }
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
    
    copylocal { "Assets", "Data" }
    debugdir "%{wks.location}/Build/%{cfg.buildcfg}"
    dependson { "Ultra" }
    entrypoint "mainCRTStartup"
    files { "**.h", "**.cpp", "**.cppm", "**.cxx", "**.inl", "**.ixx" }
    postbuildcommands {
        "robocopy /mir /nfl /ndl /njh /njs /np /r:2 /w:1 \"%{wks.location}Assets\" \"%{cfg.targetdir}/Assets\"",
        "robocopy /mir /nfl /ndl /njh /njs /np /r:2 /w:1 \"%{wks.location}Data\" \"%{cfg.targetdir}/Data\"",
        "exit /b 0",
    }
    
    includedirs {
        "%{Headers.Library}"
    }

    links {
        "Ultra"
    }

    filter { "configurations:Debug" }
        kind "ConsoleApp"
        defines { "_DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        kind "WindowedApp"
        defines { "NDEBUG" }
        optimize "On"
        symbols "On"
    
    filter { "configurations:Distribution" }
        kind "WindowedApp"
        defines { "NDEBUG" }
        optimize "On"
        symbols "Off"
    
    filter { }

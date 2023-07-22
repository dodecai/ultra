project "Game"
    defines { "PROJECT_NAME=Game" }
    language "C++"
    characterset "MBCS"
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
    dependson { "Ultra" }
    entrypoint "mainCRTStartup"
    files { "**.h", "**.cpp", "**.cppm", "**.cxx", "**.inl", "**.ixx", "*.lua" }
    postbuildcommands {
        "robocopy /mir /nfl /ndl /njh /njs /np /r:2 /w:1 \"%{wks.location}Assets\" \"%{cfg.targetdir}/Assets\"",
        "robocopy /mir /nfl /ndl /njh /njs /np /r:2 /w:1 \"%{wks.location}Data\" \"%{cfg.targetdir}/Data\"",
        "exit /b 0",
    }
    
    externalincludedirs {
	    "%{Headers.ThirdParty}/**",
        "%{Headers.LibPHXExt}",
    }
    includedirs {
        "%{Headers.Library}",
        "%{Headers.LibPHX}",
    }
    libdirs { "%{wks.location}/3rd-Party/LibPHX/ext/lib/win64" }
    links {
        "opengl32",
        "glew32",
        "freetype",
        "Ultra",
        "lua51",
        "SDL2",
        "user32",
        "winmm",
        "Ws2_32",
    }

    filter { "configurations:Debug" }
        kind "ConsoleApp"
        defines { "_DEBUG" }
        runtime "Debug"
        --sanitize { "Address", "Fuzzer" }
        symbols "on"
    
    filter { "configurations:Distribution" }
        kind "WindowedApp"
        defines { "NDEBUG" }
        optimize "on"
        runtime "Release"
        --sanitize { "Address", "Fuzzer" }
        symbols "on"

    filter { "configurations:Release" }
        kind "WindowedApp"
        defines { "NDEBUG" }
        optimize "on"
        runtime "Release"
        symbols "off"
    
    filter { }

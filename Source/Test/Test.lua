project "Test"
    defines { "PROJECT_NAME=Test" }
    language "C++"
    characterset "Unicode"
    confromancemode "true"
    cdialectx "C17"
    cppdialectx "C++20"
    cppmodules "true"
    nativewchar "on"
    staticruntime "on"
    toolset "msc"
		
    debugdir "%{wks.location}/Build/%{cfg.buildcfg}"
    dependson { "Ultra" }
    entrypoint "mainCRTStartup"

    links { "Ultra" }

    files { "**.h", "**.cpp", "**.cppm", "**.cxx", "**.inl", "**.ixx" }
    
    includedirs {
        "%{Headers.Library}"
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

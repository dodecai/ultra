project "App"
    defines { "PROJECT_NAME=App" }
    language "C++"
    characterset "Unicode"
    confromancemode "true"
    cdialectx "C17"
    cppdialect "C++Latest"
    cppmodules "true"
    nativewchar "on"
    toolset "msc"
    
    debugdir "%{wks.location}/Build/%{cfg.buildcfg}"
    dependson { "Library" }
    entrypoint "mainCRTStartup"

    links { "Library" }

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

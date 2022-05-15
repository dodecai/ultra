project "Library"
    defines { "PROJECT_NAME=Library" }
    kind "StaticLib"
    language "C++"
    characterset "Unicode"
    confromancemode "true"
    cdialectx "C17"
    cppdialect "C++Latest"
    cppmodules "true"
    nativewchar "on"
    toolset "msc"
    
    debugdir "%{wks.location}/Build/%{cfg.buildcfg}"
    files { "**.h", "**.cpp", "**.cppm", "**.cxx", "**.inl", "**.ixx" }

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

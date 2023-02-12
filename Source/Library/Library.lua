project "Ultra"
    defines { "PROJECT_NAME=Ultra" }
    kind "StaticLib"
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
    files { "**.h", "**.cpp", "**.cppm", "**.cxx", "**.inl", "**.ixx" }
    
    includedirs {
	    "%{Headers.Library}",
	    "%{Headers.ThirdParty}",
	    "%{Headers.DearImGui}",
	    "%{Headers.EnTT}",
    }

    links { 
        "DearImGui",
        "Glad",
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

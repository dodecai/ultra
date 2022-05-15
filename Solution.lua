include "3rd-Party/Build/Modules.lua"

workspace "Ultra Spectra"
    defines { "SOLUTION_NAME=Solution" }
    architecture "x64"
    configurations { "Debug", "Release", "Distribution" }
    buildoptions "/utf-8"
    flags { "MultiProcessorCompile" }
    startproject "App"
    systemversion "latest"

    includedirs { "Source" }
    objdir "Cache/"
    targetdir "Build/%{cfg.buildcfg}"
	
    Headers = {}
    Headers["Library"] = "%{wks.location}/Source/Library"

    Library = {}
    
    include "Source/Projects.lua"
    
    group "3rd-Party"
        include "3rd-Party/3rd-Party.lua"
    group ""
    
    filter { "configurations:Debug" }
        defines { "APP_MODE_DEBUG" }

    filter { "configurations:Release" }
        defines { "APP_MODE_RELEASE" }
    
    filter { "configurations:Distribution" }
        defines { "APP_MODE_DISTRIBUTION" }
    
    filter {}

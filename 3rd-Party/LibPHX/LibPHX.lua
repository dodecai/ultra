project "LibPHX"
	kind "StaticLib"
	language "C++"
    characterset "MBCS"
    conformancemode "true"
    cdialect "C11"
	cppdialect "C++11"
    staticruntime "on"
    toolset "msc"
    warnings "Off"
    defines { "_CRT_SECURE_NO_DEPRECATE", "WIN32_LEAN_AND_MEAN", "WINDOWS=1" }
    linkoptions { "/ignore:4006" }
    
	files {
        "assets/**",
        "*.lua",
		"**.h",
		"**.cpp",
	}
    
    includedirs {
	    "include",
	    "ext/include/bullet",
    }

    externalincludedirs {
        "../",
        "ext/include",
	    "%{Headers.FreeType}",
	    "%{Headers.glad}",
	    "%{Headers.stb}",
    }

    links {
        "FreeType",
        "ext/lib/win64/BulletCollision.lib",
        "ext/lib/win64/BulletDynamics.lib",
        "ext/lib/win64/fmod64_vc.lib",
        "ext/lib/win64/fmodL64_vc.lib",
        "ext/lib/win64/fmodstudio64_vc.lib",
        "ext/lib/win64/fmodstudioL64_vc.lib",
        "ext/lib/win64/liblz4.lib",
        "ext/lib/win64/liblz4s.lib",
        "ext/lib/win64/LinearMath.lib",
        "ext/lib/win64/lua51.lib",
        "ext/lib/win64/SDL2.lib",
    }

    filter { "configurations:Debug" }
        defines { "_DEBUG" }
		runtime "Debug"
        symbols "On"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
		runtime "Release"
        optimize "On"
        symbols "Off"

    filter { "configurations:Distribution" }
        defines { "NDEBUG" }
        optimize "On"
        symbols "Off"

	filter "system:windows"
		systemversion "latest"

project "LibPHX64"
    kind "None"
    --kind "SharedLib"
	language "C++"
    characterset "MBCS"
    conformancemode "true"
    cdialect "C11"
	cppdialect "C++11"
    toolset "msc"
    warnings "Off"
    defines { "_CRT_SECURE_NO_DEPRECATE", "WIN32_LEAN_AND_MEAN", "WINDOWS=1", "LIBPHX_BUILDING=1" }
    floatingpoint "Fast"
    vectorextensions "SSE2"
    --"/GL", "/EHs-c-", "/GS-", 
    buildoptions { "/MP", "/MD", "/GR-" }
    libdirs { "ext/lib/win64" }
    
	files {
        "*.lua",
		"**.h",
		"**.cpp",
	}

    includedirs {
	    "include",
	    "%{Headers.FreeType}",
	    "ext/include/bullet",
    }
    
    externalincludedirs {
        "ext/include",
	    "%{Headers.glad}",
	    "%{Headers.stb}",
    }

    links {
        "Glad",
        "FreeType",

        "BulletCollision",
        "BulletDynamics",
        "fmod64_vc",
        "fmodL64_vc",
        "fmodstudio64_vc",
        "fmodstudioL64_vc",
        "liblz4",
        "LinearMath",
        "lua51",
        "SDL2",

        "winmm",
        "Ws2_32",
    }

    postbuildcommands {
         "xcopy /q /y /i \"%{wks.location}3rd-Party\\LibPHX\\ext\\lib\\win64\\*.dll\" \"%{cfg.buildtarget.directory}\""
    }

    filter { "configurations:Distribution" }
        defines { "NDEBUG" }
        optimize "On"
        symbols "Off"

	filter "system:windows"
		systemversion "latest"

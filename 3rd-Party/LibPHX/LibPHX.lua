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
    
    externalincludedirs {
        "../",
        "ext/include",
	    "%{Headers.glad}",
	    "%{Headers.stb}",
    }

    includedirs {
	    "include",
	    "ext/include/bullet",
    }

    links {
        "ext/lib/win64/BulletCollision.lib",
        "ext/lib/win64/BulletDynamics.lib",
        "ext/lib/win64/fmod64_vc.lib",
        "ext/lib/win64/fmodL64_vc.lib",
        "ext/lib/win64/fmodstudio64_vc.lib",
        "ext/lib/win64/fmodstudioL64_vc.lib",
        "ext/lib/win64/freetype.lib",
        "ext/lib/win64/glew32.lib",
        "ext/lib/win64/glew32s.lib",
        "ext/lib/win64/liblz4.lib",
        "ext/lib/win64/liblz4s.lib",
        "ext/lib/win64/LinearMath.lib",
        "ext/lib/win64/lua51.lib",
        "ext/lib/win64/SDL2.lib",
    }

	files {
        "*.lua",
		"**.h",
		"**.cpp",
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

project "libphx64"
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
    buildoptions { "/MP", "/MD", "/EHs-c-", "/fp:fast", "/GL", "/GS-", "/GR-", "/arch:SSE2" }
    libdirs { "ext/lib/win64" }
    
    externalincludedirs {
        "ext/include",
	    "%{Headers.glad}",
	    "%{Headers.stb}",
    }

    includedirs {
	    "include",
	    "ext/include/bullet",
    }

    links {
        "Glad",

        "BulletCollision",
        "BulletDynamics",
        "fmod64_vc",
        "fmodL64_vc",
        "fmodstudio64_vc",
        "fmodstudioL64_vc",
        "freetype",
        "glew32",
        "liblz4",
        "LinearMath",
        "lua51",
        "SDL2",

        "opengl32",
        "user32",
        "winmm",
        "Ws2_32",
    }

    postbuildcommands {
         "xcopy /q /y /i \"%{wks.location}3rd-Party\\LibPHX\\ext\\lib\\win64\\*.dll\" \"%{cfg.buildtarget.directory}\""
    }

	files {
        "*.lua",
		"**.h",
		"**.cpp",
	}

    filter { "configurations:Distribution" }
        defines { "NDEBUG" }
        optimize "On"
        symbols "Off"

	filter "system:windows"
		systemversion "latest"

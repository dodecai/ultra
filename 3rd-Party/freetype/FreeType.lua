project "FreeType"
	kind "StaticLib"
	language "C"
    cdialect "C17"
    staticruntime "on"
    toolset "msc"
    warnings "Off"

    defines {
		"FT2_BUILD_LIBRARY",
		"_CRT_SECURE_NO_WARNINGS",
		"_CRT_NONSTDC_NO_WARNINGS",
	}

	files {
        "include/ft2build.h",
		"include/freetype/*.h",
		"include/freetype/config/*.h",
		"include/freetype/internal/*.h",

		"source/autofit/autofit.c",
		"source/base/ftbase.c",
		"source/base/ftbbox.c",
		"source/base/ftbdf.c",
		"source/base/ftbitmap.c",
		"source/base/ftcid.c",
		"source/base/ftdebug.c",
		"source/base/ftfstype.c",
		"source/base/ftgasp.c",
		"source/base/ftglyph.c",
		"source/base/ftgxval.c",
		"source/base/ftinit.c",
		"source/base/ftmm.c",
		"source/base/ftotval.c",
		"source/base/ftpatent.c",
		"source/base/ftpfr.c",
		"source/base/ftstroke.c",
		"source/base/ftsynth.c",
		"source/base/ftsystem.c",
		"source/base/fttype1.c",
		"source/base/ftwinfnt.c",
		"source/bdf/bdf.c",
		"source/bzip2/ftbzip2.c",
		"source/cache/ftcache.c",
		"source/cff/cff.c",
		"source/cid/type1cid.c",
		"source/gzip/ftgzip.c",
		"source/lzw/ftlzw.c",
		"source/pcf/pcf.c",
		"source/pfr/pfr.c",
		"source/psaux/psaux.c",
		"source/pshinter/pshinter.c",
		"source/psnames/psnames.c",
		"source/raster/raster.c",
		"source/sdf/sdf.c",
		"source/sfnt/sfnt.c",
		"source/svg/ftsvg.c",
		"source/smooth/smooth.c",
		"source/truetype/truetype.c",
		"source/type1/type1.c",
		"source/type42/type42.c",
		"source/winfonts/winfnt.c",
        "FreeType.lua",
	}

    includedirs {
        "include",
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

solutionitems {
--    ["Assets"] = {
--        ["Fonts"] = {
--            ["Roboto"] = {
--                "Assets/Fonts/Roboto/*",
--            },
--        },
--        ["Meshes"] = {
--            ["Cube"] = {
--                "Assets/Meshes/Cube/*",
--            },
--        },
--        ["Shaders"] = {
--            "Assets/Shaders/*",
--        },
--    },
--    ["Build"] = {
--        ["Projects"] = {
--            "Source/App/App.lua",
--            "Source/Library/Library.lua",
--            "Source/Spectra/Spectra.lua",
--            "Source/Test/Test.lua",
--        },
--        "3rd-Party/3rd-Party.lua",
--        "3rd-Party/Build/Modules.lua",
--        "Prepare.bat",
--        "Scripts/Prepare.ps1",
--        "Source/Projects.lua",
--        "Solution.lua",
--    },
--    ["Config"] = {
--        ".clang-*",
--        ".editorconfig",
--        ".git*",
--        "Source/Settings.h",
--        "vcpkg.json",
--    },
--    ["Data"] = {
--        "Data/*.*",
--    },
--    ["Manual"] = {
--        ["3rd-Party"] = {
--            "3rd-Party/*.md",
--        },
--        "Manual/*",
--    },
    "License",
    "ReadMe.md",
    "Security.md",
}
--solutionitems {
--	"./README.md",
--	["GitHub"] = {
--		"./.github/dependabot.yml",
--		["workflows"] = {
--			"./.github/workflows/**.yml",
--		}
--	},
--	["Premake"] = {
--		"./premake5.lua",
--		["Deps"] = {
--			"./deps/premake/**.lua"
--		}
--	},
--}
filter { "configurations:Debug" }
    defines { "APP_MODE_DEBUG" }

filter { "configurations:Release" }
    defines { "APP_MODE_RELEASE" }
    
filter { "configurations:Distribution" }
    defines { "APP_MODE_DISTRIBUTION" }
    
filter {}

include "App/App.lua"
include "Library/Library.lua"
include "Spectra/Spectra.lua"
include "Test/Test.lua"


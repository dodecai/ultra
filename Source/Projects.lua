solutionitems {
    ["Assets"] = {
        ["Fonts"] = {
            ["Dosis"] = {
                "Assets/Fonts/Dosis/*",
            },
            ["Exo2"] = {
                "Assets/Fonts/Exo2/*",
            },
            ["FiraMono"] = {
                "Assets/Fonts/FiraMono/*",
            },
            ["Iceland"] = {
                "Assets/Fonts/Iceland/*",
            },
            ["NovaFlat"] = {
                "Assets/Fonts/NovaFlat/*",
            },
            ["NovaSquare"] = {
                "Assets/Fonts/NovaSquare/*",
            },
            ["Oswald"] = {
                "Assets/Fonts/Oswald/*",
            },
            ["Play"] = {
                "Assets/Fonts/Play/*",
            },
            ["Rajdhani"] = {
                "Assets/Fonts/Rajdhani/*",
            },
            ["Roboto"] = {
                "Assets/Fonts/Roboto/*",
            },
            ["Share"] = {
                "Assets/Fonts/Share/*",
            },
            ["ShareTech"] = {
                "Assets/Fonts/ShareTech/*",
            },
            ["ShareTechMono"] = {
                "Assets/Fonts/ShareTechMono/*",
            },
            ["TitilliumWeb"] = {
                "Assets/Fonts/TitilliumWeb/*",
            },
            ["Ubuntu"] = {
                "Assets/Fonts/Ubuntu/*",
            },
        },
        ["Meshes"] = {
            ["Cube"] = {
                "Assets/Meshes/Cube/*",
            },
        },
        ["Shaders"] = {
            "Assets/Shaders/*",
        },
        ["Phoenix"] = {
            "Assets/Phoenix/script/*.lua",
        },
    },
    ["Build"] = {
        "3rd-Party/3rd-Party.lua",
        "Source/Library/Library.lua",
        "3rd-Party/Build/Modules.lua",
        "Prepare.bat",
        "Scripts/Prepare.ps1",
        "Source/Projects.lua",
        "Solution.lua",
    },
    ["Config"] = {
        ".clang-*",
        ".editorconfig",
        ".git*",
        "Source/Settings.h",
        "vcpkg.json",
    },
    ["Data"] = {
        "Data/*.*",
    },
    ["Manual"] = {
        ["3rd-Party"] = {
            "3rd-Party/*.md",
        },
        "Manual/*",
    },
    "License",
    "ReadMe.md",
    "Security.md",
}

filter { "configurations:Debug" }
    defines { "APP_MODE_DEBUG" }

filter { "configurations:Release" }
    defines { "APP_MODE_RELEASE" }
    
filter { "configurations:Distribution" }
    defines { "APP_MODE_DISTRIBUTION" }
    
filter {}

include "App/App.lua"
include "Game/Game.lua"
include "Library/Library.lua"
include "Spectra/Spectra.lua"
include "Test/Test.lua"


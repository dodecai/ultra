solutionitems {
    ["Assets"] = {
        ["Fonts"] = {
            ["Roboto"] = {
                "Assets/Fonts/Roboto/*",
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


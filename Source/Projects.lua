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
        ["Images"] = {
            "Assets/Images/*.ico",
            "Assets/Images/*.png",
            "Assets/Images/*.svg",
        },
        ["Meshes"] = {
            ["Cone"] = {
                "Assets/Meshes/Cone/*",
            },
            ["Cube"] = {
                "Assets/Meshes/Cube/*",
            },
            ["Cylinder"] = {
                "Assets/Meshes/Cylinder/*",
            },
            ["Monkey"] = {
                "Assets/Meshes/Monkey/*",
            },
            ["Plane"] = {
                "Assets/Meshes/Plane/*",
            },
            ["Sphere"] = {
                "Assets/Meshes/Sphere/*",
            },
            ["Torus"] = {
                "Assets/Meshes/Torus/*",
            },
        },
        ["Shaders"] = {
            "Assets/Shaders/*",
        },
        ["Sounds"] = {
            "Assets/Sounds/*.mp3",
            "Assets/Sounds/*.wav",
        },
        ["Textures"] = {
            "Assets/Textures/*.png",
            "Assets/Textures/*.tga",
        },
        ["Phoenix"] = {
            "Assets/Phoenix/script/*.lua",
        },
    },
    ["Build"] = {
        "3rd-Party/Build/Modules.lua",
        "3rd-Party/3rd-Party.lua",
        "Source/Library/Library.lua",
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
        "Data/*",
    },
    ["Manual"] = {
        ["3rd-Party"] = {
            "3rd-Party/*.md",
        },
        "Manual/*",
    },
    ["Scripts"] = {
        "Scripts/*",
        "Prepare.bat",
    },
    ["User"] = {
        "User/*",
    },
    "Contributing.md",
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

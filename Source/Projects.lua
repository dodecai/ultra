workspace_files {
    ["Build"] = {
        "Solution.lua",
        "3rd-Party/Build/Modules.lua",
        "3rd-Party/3rd-Party.lua",
        "Source/Projects.lua",
        "Source/App/App.lua",
        "Source/Library/Library.lua",
        "Source/Test/Test.lua",
    },
    ["Config"] = {
        ".editorconfig",
        ".gitignore",
        "vcpkg.json",
        "Source/Settings.h",
    },
    ["Data"] = {
        "Data/Config.yml"
    },
    ["Manual"] = {
        "License",
        "ReadMe.md",
        "Manual/Concept.md",
        "Manual/Concept.mmd",
        "Manual/Style.md",
    }
}

filter { "configurations:Debug" }
    defines { "APP_MODE_DEBUG" }

filter { "configurations:Release" }
    defines { "APP_MODE_RELEASE" }
    
filter { "configurations:Distribution" }
    defines { "APP_MODE_DISTRIBUTION" }
    
filter {}

include "App/App.lua"
include "Library/Library.lua"
include "Test/Test.lua"


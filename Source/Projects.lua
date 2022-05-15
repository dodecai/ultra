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

include "App/App.lua"
include "Library/Library.lua"
include "Test/Test.lua"


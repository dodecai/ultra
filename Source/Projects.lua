workspace_files {
    ["Build"] = {
        "Solution.lua",
        "3rd-Party/Build/Modules.lua",
        "3rd-Party/3rd-Party.lua",
        "Source/Projects.lua",
        "Source/App/App.lua",
        "Source/Library/Library.lua",
        "Source/Spectra/Spectra.lua",
        "Source/Test/Test.lua",
        "3rd-Party/imgui/DearImGui.lua",
        "3rd-Party/entt/EnTT.lua",
        "3rd-Party/glad/Glad.lua",
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
    },
    ["Shaders"] = {
        "Assets/Shaders/Basic.glsl",
        "Assets/Shaders/Sample.glsl",
        "Assets/Shaders/Texture.glsl",
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
include "Spectra/Spectra.lua"
include "Test/Test.lua"


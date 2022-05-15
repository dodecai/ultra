### Arguments
param (
    [Parameter()]   [Switch] $Clean = $false,
    [Parameter()]   [Switch] $Silent = $false
)

### Properties
    Clear

### Variables
    $Builder = "premake5"
    $ClearDirectories = @(
        "./Build",
        "./Cache"
    )
    $ClearFiles = @(
        "*.sln",
        "**.vcxproj*"
    )

#region Default
### Initialize
    function Initialize {
        $env:Path += ";" + "$($PSScriptRoot)\..\3rd-Party\Build"
     }

### Main
    function Main {
        if ($Clean) {
            Remove-Item $ClearDirectories -Recurse -Force -ErrorAction SilentlyContinue
            (Get-ChildItem -Include $ClearFiles -Recurse) | % { Remove-Item $_.FullName -Force -ErrorAction SilentlyContinue }
       } else {
            &$Builder vs2022 --file="./Solution.lua" --os="windows"
        }
    }

### Terminate
    function Terminate {
        if (!$Silent) { pause }
    }
#endregion

### Workflow
    Initialize
    Main
    Terminate

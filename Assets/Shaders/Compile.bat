### Properties
    @echo off
    cls
    cd /d "%~dp0"

::: Variables
    set "Validator="%VULKAN_SDK%\Bin\glslc.exe""

::: Main
    %Validator% Basic.vert -o "Cache/Basic.vert.spv"
    %Validator% Basic.frag -o "Cache/Basic.frag.spv"
::: Terminate
    pause

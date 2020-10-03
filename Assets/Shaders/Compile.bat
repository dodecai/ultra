### Properties
    @echo off
    cls
    cd /d "%~dp0"

::: Variables
    set "Validator="%VULKAN_SDK%\Bin\glslc.exe""

::: Main
    %Validator% Basic.vert -o Basic.vert.spv
    %Validator% Basic.frag -o Basic.frag.spv

::: Terminate
    pause

::: Properties
    @echo off
    cls
    cd /d "%~dp0"
	
::: Variables
 :: Switch between glslangvalidator and glslc
	set "Default=1"
	
::: Main
 :: Preparation
	if not defined VULKAN_SDK (
		echo.Error::Dependency: Could not determine the path to Vulkan SDK, please check your environment variables or install the SDK.
		goto :Terminate
	)
	if defined Default (
		set "Validator="%VULKAN_SDK%\Bin\glslangValidator.exe""
	) else (
		set "Validator="%VULKAN_SDK%\Bin\glslc.exe""
	)
	
 :: Compile basic shader (so we can compare with the integrated solution)
	if defined Default (
		%Validator% -S vert -e main -o "../Assets/Shaders/Cache/Basic.spirv.vert.pre" -V "../Assets/Shaders/Basic.vert"
		%Validator% -S frag -e main -o "../Assets/Shaders/Cache/Basic.spirv.frag.pre" -V "../Assets/Shaders/Basic.frag"
	) else (
		%Validator% "../Assets/Shaders/Basic.vert" -o "../Assets/Shaders/Cache/Basic.spirv.vert.pre"
		%Validator% "../Assets/Shaders/Basic.frag" -o "../Assets/Shaders/Cache/Basic.spirv.frag.pre"
	)
	
::: Terminate
:Terminate
    pause

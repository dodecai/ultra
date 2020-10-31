::: Properties
    @echo off
    cls
    cd /d "%~dp0"
	
::: Variables
 :: Switch between glslangvalidator and glslc
	set "Default=1"
    set "Prefix=static-spirv"
    set "SourceRoot=../Assets/Shaders"
    set "CacheRoot=../Data/Cache/Shaders
	
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
		%Validator% -S vert -e main -o "%CacheRoot%/Basic.%Prefix%-vert" -V "%SourceRoot%/Basic.vert"
		%Validator% -S frag -e main -o "%CacheRoot%/Basic.%Prefix%-frag" -V "%SourceRoot%/Basic.frag"
	) else (
		%Validator% "%SourceRoot%/Basic.vert" -o "%CacheRoot%/Basic.%Prefix%-vert"
		%Validator% "%SourceRoot%/Basic.frag" -o "%CacheRoot%/Basic.%Prefix%-frag"
	)
	
::: Terminate
:Terminate
    pause

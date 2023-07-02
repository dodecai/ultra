### Properties
	Clear-Host
	Set-Location $PSScriptRoot
	
### Variables
	$CacheRoot = "../Data/Cache/Shaders"
	$ShaderRoot = "../Assets/Shaders"
	$VulkanSDK = ""
	if (Get-ChildItem env:VULKAN_SDK) {
		$VulkanSDK = $env:VULKAN_SDK
	}

### Compile Shaders
	if ($VulkanSDK) {
		foreach ($shader in (Get-ChildItem "../Assets/Shaders/*.glsl")) {
			Write-Host "Compiling shader(s): './$($shader.Name)'" -ForegroundColor Blue
			& "$VulkanSDK\Bin\glslc.exe" $shader.FullName -o "../Data/Cache/Shaders/$($shader.Name).spirv"
			Write-Host ""
		}
		
	}

### Terminate
	#pause

-- Build Environment
--VulkanSdkPath = os.get('VULKANK_SDK')

-- Built-In Headers
Headers["ThirdParty"] = "%{wks.location}/3rd-Party/@Headers"

-- Project Headers
Headers["assimp"] = "%{wks.location}/3rd-Party/assimp/include"
Headers["DearImGui"] = "%{wks.location}/3rd-Party/imgui"
Headers["EnTT"] = "%{wks.location}/3rd-Party/entt/single_include"
Headers["glm"] = "%{wks.location}/3rd-Party/glm"
Headers["yamlcpp"] = "%{wks.location}/3rd-Party/yaml-cpp/include"

-- Projects
include "assimp/assimp.lua"
include "imgui/DearImGui.lua"
include "entt/EnTT.lua"
include "glad/glad.lua"
include "yaml-cpp/yaml-cpp.lua"

-- Libraries and Packages
Library["ShaderC"] = "%{wks.location}/3rd-Party/@Library/shaderc/shaderc_shared.lib"
Library["Vulkan"] = "%{wks.location}/3rd-Party//@Library/vulkan/vulkan-1.lib"
--Library["Vulkan"] = "${VulkanSdkPath}/Lib/vulkan-1.lib"
--Library["VulkanUtils"] = "${VulkanSdkPath}/Lib/VkLayer_utils.lib"

Package["ShaderC"] = "%{wks.location}3rd-Party\\@Library\\shaderc\\shaderc_shared.dll"

-- Phoenix Test
Headers["LibPHX"] = "%{wks.location}/3rd-Party/LibPHX/include"
Headers["LibPHXext"] = "%{wks.location}/3rd-Party/LibPHX/ext/include"

include "LibPHX/LibPHX.lua"

--Headers["ThirdParty"] = "%{wks.location}/3rd-Party"

-- Built-In Headers
Headers["ThirdParty"] = "%{wks.location}/3rd-Party/headers"
Headers["stb"] = "%{wks.location}/3rd-Party/stb"


Headers["assimp"] = "%{wks.location}/3rd-Party/assimp/include"
Headers["DearImGui"] = "%{wks.location}/3rd-Party/imgui"
Headers["EnTT"] = "%{wks.location}/3rd-Party/entt/single_include"
Headers["glm"] = "%{wks.location}/3rd-Party/glm"
Headers["GLSLang"] = "%{wks.location}/3rd-Party/glslang"
Headers["ShaderC"] = "%{wks.location}/3rd-Party/shaderc"
Headers["stb"] = "%{wks.location}/3rd-Party/stb"
Headers["Vulkan"] = "%{wks.location}/3rd-Party/vulkan"
Headers["VulkanVideo"] = "%{wks.location}/3rd-Party/vk_video"
Headers["LibPHX"] = "%{wks.location}/3rd-Party/LibPHX/include"
--Headers["LibPHXExt"] = "%{wks.location}/3rd-Party/LibPHX/ext/include"

include "assimp/assimp.lua"
include "imgui/DearImGui.lua"
include "entt/EnTT.lua"
include "glad/glad.lua"
include "LibPHX/LibPHX.lua"

Library["ShaderC"] = "%{wks.location}/3rd-Party/shaderc/lib/shaderc_shared.lib"
Library["Vulkan"] = "%{wks.location}/3rd-Party/vulkan/lib/vulkan-1.lib"
--Library["VulkanUtils"] = "${VulkanSdkPath}/Lib/VkLayer_utils.lib"
--Library["VulkanUtils"] = "%{wks.location}/3rd-Party/vulkan/lib/VkLayer_utils.lib"
-- VulkanSdkPath = os.get('VULKANK_SDK')

Package["ShaderC"] = "%{wks.location}3rd-Party\\shaderc\\lib\\shaderc_shared.dll"

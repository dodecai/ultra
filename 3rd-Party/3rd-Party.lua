Headers["ThirdParty"] = "%{wks.location}/3rd-Party"
Headers["DearImGui"] = "%{wks.location}/3rd-Party/imgui"
Headers["EnTT"] = "%{wks.location}/3rd-Party/EnTT/single_include"
Headers["Vulkan"] = "%{wks.location}/3rd-Party/vulkan"
Headers["VulkanVideo"] = "%{wks.location}/3rd-Party/vk_video"

include "imgui/DearImGui.lua"
include "entt/EnTT.lua"
include "glad/Glad.lua"

Library["Vulkan"] = "%{wks.location}/3rd-Party/vulkan/lib/vulkan-1.lib"
Library["VulkanUtils"] = "%{wks.location}/3rd-Party/vulkan/lib/VkLayer_utils.lib"
-- VulkanSdkPath = os.get('VULKANK_SDK')
-- Library["Vulkan"] = "${VulkanSdkPath}/Lib/vulkan-1.lib"
-- Library["VulkanUtils"] = "${VulkanSdkPath}/Lib/VkLayer_utils.lib"

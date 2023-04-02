Headers["ThirdParty"] = "%{wks.location}/3rd-Party"
Headers["DearImGui"] = "%{wks.location}/3rd-Party/imgui"
Headers["EnTT"] = "%{wks.location}/3rd-Party/EnTT/single_include"

include "imgui/DearImGui.lua"
include "entt/EnTT.lua"
include "glad/Glad.lua"

-- Internal
-- Library["Vulkan"] = "%{wks.location}/3rd-Party/vulkan/lib/vulkan-1.lib"
-- Library["VulkanUtils"] = "%{wks.location}/3rd-Party/vulkan/lib/VkLayer_utils.lib"

-- External
-- VULKAN_SDK_PATH = os.get('VK_SDK_PATH')
-- Library["Vulkan"] = "C:/Program Files/VulkanSDK/1.2.189.2/Lib/vulkan-1.lib"


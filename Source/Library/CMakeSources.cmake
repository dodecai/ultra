# Preparation
set (OMNIA_HEADERS "")
set (OMNIA_SOURCES "")

# Default Extensions
set(OMNIA_LIBRARY_DEFAULT_HEADERS
	"Source/Omnia.h"
	"Source/Omnia/Omnia.pch"
	"Source/Omnia/Core.h"
	"Source/Omnia/EntryPoint.h"
	"Source/Omnia/Platform.h"
	"Source/Omnia/Types.h"
)
set(OMNIA_LIBRARY_DEFAULT_SOURCES
	""
)
list(APPEND OMNIA_HEADERS ${OMNIA_LIBRARY_DEFAULT_HEADERS})
list(APPEND OMNIA_SOURCES ${OMNIA_LIBRARY_DEFAULT_SOURCES})

# Prime Extensions
set(OMNIA_LIBRARY_PRIME_HEADERS
	"Source/Omnia/Config.h"
	"Source/Omnia/Log.h"
)
set(OMNIA_LIBRARY_PRIME_SOURCES
	"Source/Omnia/Config.cpp"
)
list(APPEND OMNIA_HEADERS ${OMNIA_LIBRARY_PRIME_HEADERS})
list(APPEND OMNIA_SOURCES ${OMNIA_LIBRARY_PRIME_SOURCES})


# Core Extensions
set(OMNIA_LIBRARY_CORE_HEADERS
	"Source/Omnia/Core/Application.h"
	"Source/Omnia/Core/Layer.h"
	"Source/Omnia/Core/LayerStack.h"
)
set(OMNIA_LIBRARY_CORE_SOURCES
	"Source/Omnia/Core/Application.cpp"
	"Source/Omnia/Core/LayerStack.cpp"
)
list(APPEND OMNIA_HEADERS ${OMNIA_LIBRARY_CORE_HEADERS})
list(APPEND OMNIA_SOURCES ${OMNIA_LIBRARY_CORE_SOURCES})

# Debug Extensions
set(OMNIA_LIBRARY_DEBUG_HEADERS
	"Source/Omnia/Debug/Instrumentor.h"
	"Source/Omnia/Debug/Memory.h"
	"Source/Omnia/Debug/Profiler.h"
)
set(OMNIA_LIBRARY_DEBUG_SOURCES
	""
)
list(APPEND OMNIA_HEADERS ${OMNIA_LIBRARY_DEBUG_HEADERS})
list(APPEND OMNIA_SOURCES ${OMNIA_LIBRARY_DEBUG_SOURCES})

# GFX Extensions
set(OMNIA_LIBRARY_GFX_HEADERS
	"Source/Omnia/GFX/Context.h"
    
	"Source/Platform/GFX/OpenGL/OpenGL.h"
	"Source/Platform/GFX/OpenGL/GLContext.h"
	
	"Source/Platform/GFX/Vulkan/Vulkan.h"
	"Source/Platform/GFX/Vulkan/VKAllocator.h"
	"Source/Platform/GFX/Vulkan/VKContext.h"
	"Source/Platform/GFX/Vulkan/VKInstance.h"
	"Source/Platform/GFX/Vulkan/VKDevice.h"
	"Source/Platform/GFX/Vulkan/VKSwapChain.h"
)
set(OMNIA_LIBRARY_GFX_SOURCES
	"Source/Omnia/GFX/Context.cpp"

	"Source/Platform/GFX/OpenGL/GLContext.cpp"
	
	"Source/Platform/GFX/Vulkan/VKAllocator.cpp"
	"Source/Platform/GFX/Vulkan/VKContext.cpp"
	"Source/Platform/GFX/Vulkan/VKInstance.cpp"
	"Source/Platform/GFX/Vulkan/VKDevice.cpp"
	"Source/Platform/GFX/Vulkan/VKSwapChain.cpp"
)
list(APPEND OMNIA_HEADERS ${OMNIA_LIBRARY_GFX_HEADERS})
list(APPEND OMNIA_SOURCES ${OMNIA_LIBRARY_GFX_SOURCES})


# System Extensions
set(OMNIA_LIBRARY_SYSTEM_HEADERS
	"Source/Omnia/System/Cli.h"
	"Source/Omnia/System/FileSystem.h"
	"Source/Omnia/System/Input.h"

	"Source/Platform/System/WinAPI/WinInput.h"
)
set(OMNIA_LIBRARY_SYSTEM_SOURCES
	"Source/Omnia/System/Input.cpp"

	"Source/Platform/System/WinAPI/WinInput.cpp"
)
list(APPEND OMNIA_HEADERS ${OMNIA_LIBRARY_SYSTEM_HEADERS})
list(APPEND OMNIA_SOURCES ${OMNIA_LIBRARY_SYSTEM_SOURCES})


# UI Extensions
set(OMNIA_LIBRARY_UI_HEADERS
	"Source/Omnia/UI/GuiBuilder.h"
	"Source/Omnia/UI/GuiLayer.h"
    
	"Source/Omnia/UI/Dialog.h"
	"Source/Omnia/UI/EventData.h"
	"Source/Omnia/UI/Event.h"
	"Source/Omnia/UI/WindowData.h"
	"Source/Omnia/UI/Window.h"

	"Source/Platform/UI/WinAPI/WinDialog.h"
	"Source/Platform/UI/WinAPI/WinEvent.h"
	"Source/Platform/UI/WinAPI/WinWindow.h"
)
set(OMNIA_LIBRARY_UI_SOURCES
	"Source/Omnia/UI/GuiBuilder.cpp"
	"Source/Omnia/UI/GuiLayer.cpp"
    
	"Source/Omnia/UI/Dialog.cpp"
	"Source/Omnia/UI/Event.cpp"
	"Source/Omnia/UI/Window.cpp"

	"Source/Platform/UI/WinAPI/WinDialog.cpp"
	"Source/Platform/UI/WinAPI/WinEvent.cpp"
	"Source/Platform/UI/WinAPI/WinWindow.cpp"
)
list(APPEND OMNIA_HEADERS ${OMNIA_LIBRARY_UI_HEADERS})
list(APPEND OMNIA_SOURCES ${OMNIA_LIBRARY_UI_SOURCES})


# Utility Extensions
set(OMNIA_LIBRARY_UTILITY_HEADERS
	"Source/Omnia/Utility/DateTime.h"
	"Source/Omnia/Utility/Enum.h"
	"Source/Omnia/Utility/Future.h"
	"Source/Omnia/Utility/Message.h"
	"Source/Omnia/Utility/Property.h"
	"Source/Omnia/Utility/String.h"
	"Source/Omnia/Utility/ThreadPool.h"
	"Source/Omnia/Utility/Timer.h"
	"Source/Omnia/Utility/UUID.h"
)
set(OMNIA_LIBRARY_UTILITY_SOURCES
	""
)
list(APPEND OMNIA_HEADERS ${OMNIA_LIBRARY_UTILITY_HEADERS})
list(APPEND OMNIA_SOURCES ${OMNIA_LIBRARY_UTILITY_SOURCES})

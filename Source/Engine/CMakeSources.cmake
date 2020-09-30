# Preparation
set (ENGINE_HEADERS "")
set (ENGINE_SOURCES "")

# Default
set(ENGINE_DEFAULT_HEADERS "Ultra/Engine.h")
set(ENGINE_DEFAULT_SOURCES )
list(APPEND ENGINE_HEADERS ${ENGINE_DEFAULT_HEADERS})
list(APPEND ENGINE_SOURCES ${ENGINE_DEFAULT_SOURCES})

# Audio
set(ENGINE_AUDIO_HEADERS
	"Ultra/Audio/Audio.h"
)
set(ENGINE_AUDIO_SOURCES
	"Ultra/Audio/Audio.cpp"
)
list(APPEND ENGINE_HEADERS ${ENGINE_AUDIO_HEADERS})
list(APPEND ENGINE_SOURCES ${ENGINE_AUDIO_SOURCES})

# Modules
set(ENGINE_MODULE_HEADERS
	"Ultra/Module/CameraController.h"
	"Ultra/Module/ParticleSystem.h"
)
set(ENGINE_MODULE_SOURCES
	"Ultra/Module/CameraController.cpp"
	"Ultra/Module/ParticleSystem.cpp"
)
list(APPEND ENGINE_HEADERS ${ENGINE_MODULE_HEADERS})
list(APPEND ENGINE_SOURCES ${ENGINE_MODULE_SOURCES})

# Renderer
set(ENGINE_RENDERER_HEADERS
	"Ultra/Renderer/Buffer.h"
	"Ultra/Renderer/Camera.h"
	"Ultra/Renderer/Framebuffer.h"
	"Ultra/Renderer/Mesh.h"
	"Ultra/Renderer/Pipeline.h"
	"Ultra/Renderer/RenderCommand.h"
	"Ultra/Renderer/RenderPass.h"
	"Ultra/Renderer/RendererAPI.h"
	"Ultra/Renderer/RendererData.h"
	"Ultra/Renderer/Renderer.h"
	"Ultra/Renderer/Renderer2D.h"
	"Ultra/Renderer/Shader.h"
	"Ultra/Renderer/SubTexture.h"
	"Ultra/Renderer/Texture.h"

	"Ultra/Platform/OpenGL/GLBuffer.h"
	"Ultra/Platform/OpenGL/GLFramebuffer.h"
	"Ultra/Platform/OpenGL/GLPipeline.h"
	"Ultra/Platform/OpenGL/GLRendererAPI.h"
	"Ultra/Platform/OpenGL/GLRenderPass.h"
	"Ultra/Platform/OpenGL/GLShader.h"
	"Ultra/Platform/OpenGL/GLTexture.h"
    
	"Ultra/Platform/Vulkan/VKBuffer.h"
	"Ultra/Platform/Vulkan/VKFramebuffer.h"
	"Ultra/Platform/Vulkan/VKPipeline.h"
	"Ultra/Platform/Vulkan/VKRendererAPI.h"
	"Ultra/Platform/Vulkan/VKRenderPass.h"
	"Ultra/Platform/Vulkan/VKShader.h"
	"Ultra/Platform/Vulkan/VKTexture.h"

	"Ultra/Platform/Vulkan/VKTest.h"
)
set(ENGINE_RENDERER_SOURCES
	"Ultra/Renderer/Buffer.cpp"
	"Ultra/Renderer/Camera.cpp"
	"Ultra/Renderer/Framebuffer.cpp"
	"Ultra/Renderer/Mesh.cpp"
	"Ultra/Renderer/Pipeline.cpp"
	"Ultra/Renderer/RenderCommand.cpp"
	"Ultra/Renderer/RenderPass.cpp"
	"Ultra/Renderer/RendererAPI.cpp"
	"Ultra/Renderer/Renderer.cpp"
	"Ultra/Renderer/Renderer2D.cpp"
	"Ultra/Renderer/Shader.cpp"
	"Ultra/Renderer/Texture.cpp"
	"Ultra/Renderer/SubTexture.cpp"

	"Ultra/Platform/OpenGL/GLBuffer.cpp"
	"Ultra/Platform/OpenGL/GLFramebuffer.cpp"
	"Ultra/Platform/OpenGL/GLPipeline.cpp"
	"Ultra/Platform/OpenGL/GLRendererAPI.cpp"
	"Ultra/Platform/OpenGL/GLRenderPass.cpp"
	"Ultra/Platform/OpenGL/GLShader.cpp"
	"Ultra/Platform/OpenGL/GLTexture.cpp"
    
	"Ultra/Platform/Vulkan/VKBuffer.cpp"
	"Ultra/Platform/Vulkan/VKFramebuffer.cpp"
	"Ultra/Platform/Vulkan/VKPipeline.cpp"
	"Ultra/Platform/Vulkan/VKRendererAPI.cpp"
	"Ultra/Platform/Vulkan/VKRenderPass.cpp"
	"Ultra/Platform/Vulkan/VKShader.cpp"
	"Ultra/Platform/Vulkan/VKTexture.cpp"
    
	"Ultra/Platform/Vulkan/VKTest.cpp"
)
list(APPEND ENGINE_HEADERS ${ENGINE_RENDERER_HEADERS})
list(APPEND ENGINE_SOURCES ${ENGINE_RENDERER_SOURCES})

# Scene
set(ENGINE_SCENE_HEADERS
	"Ultra/Scene/Components.h"
	"Ultra/Scene/Entity.h"
	"Ultra/Scene/System.h"

	"Ultra/Scene/Scene.h"

	"Ultra/Scene/SceneCamera.h"
)
set(ENGINE_SCENE_SOURCES
	"Ultra/Scene/Scene.cpp"

	"Ultra/Scene/SceneCamera.cpp"
)
list(APPEND ENGINE_HEADERS ${ENGINE_SCENE_HEADERS})
list(APPEND ENGINE_SOURCES ${ENGINE_SCENE_SOURCES})

# Scripts
set(ENGINE_SCRIPT_HEADERS
	"Ultra/Script/CameraController.h"
)
set(ENGINE_SCRIPT_SOURCES
)
list(APPEND ENGINE_HEADERS ${ENGINE_SCRIPT_HEADERS})
list(APPEND ENGINE_SOURCES ${ENGINE_SCRIPT_SOURCES})

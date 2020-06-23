set (ENGINE_HEADERS "")
set (ENGINE_SOURCES "")

# Default
set(ENGINE_DEFAULT_HEADERS
	"Engine.h"
)
list(APPEND ENGINE_HEADERS ${ENGINE_DEFAULT_HEADERS})

set(ENGINE_DEFAULT_SOURCES
)
list(APPEND ENGINE_SOURCES ${ENGINE_DEFAULT_SOURCES})


# Renderer
set(ENGINE_RENDERER_HEADERS
	"Renderer/Buffer.h"
	"Renderer/Camera.h"
	"Renderer/CameraController.h"
	"Renderer/Shader.h"
	"Renderer/Texture.h"
	"Renderer/VertexArray.h"
	"Renderer/RendererAPI.h"
	"Renderer/RenderCommand.h"
	"Renderer/Renderer.h"

	"Platform/OpenGL/GLBuffer.h"
	"Platform/OpenGL/GLShader.h"
	"Platform/OpenGL/GLTexture.h"
	"Platform/OpenGL/GLVertexArray.h"
	"Platform/OpenGL/GLRendererAPI.h"
)
list(APPEND ENGINE_HEADERS ${ENGINE_RENDERER_HEADERS})

set(ENGINE_RENDERER_SOURCES
	"Renderer/Buffer.cpp"
	"Renderer/Camera.cpp"
	"Renderer/CameraController.cpp"
	"Renderer/Shader.cpp"
	"Renderer/Texture.cpp"
	"Renderer/VertexArray.cpp"
	"Renderer/RendererAPI.cpp"
	"Renderer/RenderCommand.cpp"
	"Renderer/Renderer.cpp"

	"Platform/OpenGL/GLBuffer.cpp"
	"Platform/OpenGL/GLShader.cpp"
	"Platform/OpenGL/GLTexture.cpp"
	"Platform/OpenGL/GLVertexArray.cpp"
	"Platform/OpenGL/GLRendererAPI.cpp"
)
list(APPEND ENGINE_SOURCES ${ENGINE_RENDERER_SOURCES})

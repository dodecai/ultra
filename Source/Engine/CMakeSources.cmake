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
	"Renderer/Renderer.h"
	"Renderer/Shader.h"

	"Platform/OpenGL/GLBuffer.h"
	"Platform/OpenGL/GLShader.h"
)
list(APPEND ENGINE_HEADERS ${ENGINE_RENDERER_HEADERS})

set(ENGINE_RENDERER_SOURCES
	"Renderer/Buffer.cpp"
	"Renderer/Renderer.cpp"
	"Renderer/Shader.cpp"

	"Platform/OpenGL/GLBuffer.cpp"
	"Platform/OpenGL/GLShader.cpp"
)
list(APPEND ENGINE_SOURCES ${ENGINE_RENDERER_SOURCES})

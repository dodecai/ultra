#include "GLRendererAPI.h"

#include <glad/glad.h>

namespace Ultra {

void OpenGLMessageCallback(
	unsigned source,
	unsigned type,
	unsigned id,
	unsigned severity,
	int length,
	const char* message,
	const void* userParam) {
	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:         APP_LOG_CRITICAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       APP_LOG_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          APP_LOG_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: APP_LOG_TRACE(message); return;
	}

	//HZ_CORE_ASSERT(false, "Unknown severity level!");
}
void GLRendererAPI::Load() {
	#define APP_MODE_DEBUG
	#ifdef APP_MODE_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	#endif

	gladLoadGL();
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	//glEnable(GL_MULTISAMPLE);	// Information

	// Information
	const uint8_t *vendor = glGetString(GL_VENDOR);
	const uint8_t *model = glGetString(GL_RENDERER);
	const uint8_t *version = glGetString(GL_VERSION);  // version as a string
	const uint8_t *slv = glGetString(GL_SHADING_LANGUAGE_VERSION);
	const uint8_t *extensions = glGetStringi(GL_EXTENSIONS, 0);
	const uint8_t *extensions2 = glGetStringi(GL_EXTENSIONS, 1);
	AppLogInfo("[Renderer] Vendor:  '", vendor, "'");
	AppLogInfo("[Renderer] Model:   '", model, "'");
	AppLogInfo("[Renderer] Version: '", version, "'");
	AppLogDebug("[Renderer] Shader Version: '", slv, "'");
	AppLogTrace("[Renderer] Extensions: '", extensions, " | ", extensions2, "'");
}

void GLRendererAPI::Clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray, uint32_t indexCount) {
	uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GLRendererAPI::SetClearColor(const glm::vec4 &color) {
	glClearColor(color.r, color.g, color.b, color.a);
}

void GLRendererAPI::SetViewport(const int32_t x, const int32_t y, const uint32_t width, const uint32_t height) {
	if (!(width == 0) || !(height == 0)) {
		// ToDo: Prevent sizes above imagination
		return;
	}
	glViewport(x, y, width, height);
}

void GLRendererAPI::SetWireframeMode(bool status) {
	if (status) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

}

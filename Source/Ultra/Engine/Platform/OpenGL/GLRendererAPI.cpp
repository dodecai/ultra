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
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glFrontFace(GL_CCW);

	glEnable(GL_MULTISAMPLE);	// Information
	//glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

	// Information
	auto &capabilities = RendererAPI::GetCapabilities();
	capabilities.Vendor =		(const char*)glGetString(GL_VENDOR);
	capabilities.Model =		(const char*)glGetString(GL_RENDERER);
	capabilities.Version =		(const char*)glGetString(GL_VERSION);
	capabilities.SLVersion =	(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	capabilities.Extensions[0]=	(const char*)glGetStringi(GL_EXTENSIONS, 0);
	capabilities.Extensions[1]=	(const char*)glGetStringi(GL_EXTENSIONS, 1);

	// Limits
	glGetIntegerv(GL_MAX_SAMPLES, &capabilities.MaxSamples);
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &capabilities.MaxAnisotropy);
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &capabilities.MaxTextureUnits);

	capabilities.Log();

	// Show Errors
	auto error = glGetError();
	if (error != GL_NO_ERROR) {
		AppLogError("[Ultra::GLRendererAPI::Load]: ", error);
	}
}

void GLRendererAPI::Unload() {

}


void GLRendererAPI::Clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray, uint32_t indexCount) {
	uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GLRendererAPI::DrawIndexed(uint32_t count, Type type, bool depthTest) {
	if (!depthTest) glDisable(GL_DEPTH_TEST);

	GLenum primitive = 0;
	switch (type) {
		case Type::Lines:		{ primitive = GL_LINES;		break; }
		case Type::Triangles:	{ primitive = GL_TRIANGLES;	break; }
	}

	glDrawElements(primitive, count, GL_UNSIGNED_INT, nullptr);

	if (!depthTest) glEnable(GL_DEPTH_TEST);
}


void GLRendererAPI::SetClearColor(const glm::vec4 &color) {
	glClearColor(color.r, color.g, color.b, color.a);
}

void GLRendererAPI::SetLineThickness(float value) {
	GLfloat range[2];
	glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, range);
	APP_ASSERT(!(value < range[0] || value > range[1]), "The specified line width exceeds the supported range!")
	glLineWidth(value);
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

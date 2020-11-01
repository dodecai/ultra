#include "GLRendererAPI.h"

#include <glad/gl.h>

#define APP_MODE_DEBUG

namespace Ultra {

// Default
void GLRendererAPI::Load() {
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

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
		AppLogError("[Ultra::RendererAPI::GL]: ", error);
	}
}

void GLRendererAPI::Unload() {

}


// 
void GLRendererAPI::Clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLRendererAPI::DrawIndexed(uint32_t count, PrimitiveType type, bool depthTest) {
	if (!depthTest) glDisable(GL_DEPTH_TEST);

	GLenum primitive = 0;
	switch (type) {
		case PrimitiveType::Line:		{ primitive = GL_LINES;		break; }
		case PrimitiveType::Triangle:	{ primitive = GL_TRIANGLES;	break; }
	}

	glDrawElements(primitive, count, GL_UNSIGNED_INT, nullptr);

	if (!depthTest) glEnable(GL_DEPTH_TEST);
}


// Mutators
void GLRendererAPI::SetClearColor(const glm::vec4 &color) {
	glClearColor(color.r, color.g, color.b, color.a);
}

void GLRendererAPI::SetLineThickness(float value) {
	GLfloat range[2];
	glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, range);
	// ToDo: Fails while closing application, the Update function should somehow finish cleanly.
	//APP_ASSERT(!(value < range[0] || value > range[1]), "The specified line width exceeds the supported range!")
	glLineWidth(value);
}

void GLRendererAPI::SetPolygonMode(PolygonMode mode) {
	switch (mode) {
		case PolygonMode::Solid:		{ glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break; }
		case PolygonMode::Wireframe:	{ glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break; }
		default: {
			APP_ASSERT(false, "The specified polygon mode isn't implemented yet!");
			break;
		}
	}
}

void GLRendererAPI::SetViewport(const int32_t x, const int32_t y, const uint32_t width, const uint32_t height) {
	if (!(width == 0) || !(height == 0)) {
		// ToDo: Prevent sizes above imagination
		return;
	}
	glViewport(x, y, width, height);
}

}

#include "GLRendererAPI.h"
#include <glad/glad.h>

namespace Ultra {

void GLRendererAPI::Load() {
	gladLoadGL();
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glEnable(GL_MULTISAMPLE);
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
	glViewport(x, y, width, height);
}

}

#include "GLRendererAPI.h"
#include <glad/glad.h>

namespace Ultra {

void GLRendererAPI::Clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray) {
	glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void GLRendererAPI::SetClearColor(const glm::vec4 &color) {
	glClearColor(color.r, color.g, color.b, color.a);
}

}

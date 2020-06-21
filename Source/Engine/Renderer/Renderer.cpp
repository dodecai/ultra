#include "Renderer.h"

#include <memory>
#include "Platform/OpenGL/GLShader.h"

namespace Ultra {

Renderer::SceneData *Renderer::mSceneData = new Renderer::SceneData;

void Renderer::Load() {
	RenderCommand::Load();
}

void Renderer::BeginScene(OrthographicCamera &camera) {
	mSceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {
}

void Renderer::Submit(const std::shared_ptr<Shader> &shader, const std::shared_ptr<VertexArray> &vertexArray, const glm::mat4 &transform) {
	std::dynamic_pointer_cast<GLShader>(shader)->Bind();
	std::dynamic_pointer_cast<GLShader>(shader)->UploadaUniformMat4("u_ViewProjection", mSceneData->ViewProjectionMatrix);
	std::dynamic_pointer_cast<GLShader>(shader)->UploadaUniformMat4("u_Transform", transform);

	vertexArray->Bind();
	RenderCommand::DrawIndexed(vertexArray);
}

}

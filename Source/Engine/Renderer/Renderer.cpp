#include "Renderer.h"

namespace Ultra {

Renderer::SceneData *Renderer::mSceneData = new Renderer::SceneData;

void Renderer::BeginScene(OrthographicCamera &camera) {
	mSceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {
}

void Renderer::Submit(const std::shared_ptr<Shader> &shader, const std::shared_ptr<VertexArray> &vertexArray, const glm::mat4 &transform) {
	shader->Bind();
	shader->UploadaUniformMat4("u_ViewProjection", mSceneData->ViewProjectionMatrix);
	shader->UploadaUniformMat4("u_Transform", transform);

	vertexArray->Bind();
	RenderCommand::DrawIndexed(vertexArray);
}

}

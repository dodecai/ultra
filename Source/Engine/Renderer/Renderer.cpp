#include "Renderer.h"

namespace Ultra {

Renderer::SceneData *Renderer::mSceneData = new Renderer::SceneData;

void Renderer::BeginScene(OrthographicCamera &camera) {
	mSceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {
}

void Renderer::Submit(const std::shared_ptr<Shader> &shader, const std::shared_ptr<VertexArray> &vertexArray) {
	shader->Bind();
	shader->UploadaUniformMat4("u_ViewProjection", mSceneData->ViewProjectionMatrix);

	vertexArray->Bind();
	RenderCommand::DrawIndexed(vertexArray);
}

}

#include "Renderer.h"
#include "Renderer2D.h"

#include "Engine/Platform/OpenGL/GLShader.h"

namespace Ultra {

Renderer::SceneData *Renderer::mSceneData = new Renderer::SceneData;

void Renderer::Load() {
	RenderCommand::Load();
	Renderer2D::Load();
}

void Renderer::BeginScene(OrthographicCamera &camera) {
	mSceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {
}

void Renderer::Resize(const uint32_t width, const uint32_t height) {
	RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::Submit(const std::shared_ptr<Shader> &shader, const std::shared_ptr<VertexArray> &vertexArray, const glm::mat4 &transform) {
	shader->Bind();
	shader->SetMat4("u_ViewProjection", mSceneData->ViewProjectionMatrix);
	shader->SetMat4("u_Transform", transform);

	vertexArray->Bind();
	RenderCommand::DrawIndexed(vertexArray);
}

}

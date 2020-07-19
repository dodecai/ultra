#pragma once

#include "RenderCommand.h"

#include "Ultra.pch"
#include "Core.h"

#include "Camera.h"
#include "Shader.h"

namespace Ultra {

class Renderer {
	struct SceneData {
		glm::mat4 ViewProjectionMatrix;
	};

	static SceneData *mSceneData;

public:
	static void Load();

	static void BeginScene(Camera &camera);
	static void BeginScene(OrthographicCamera &camera);
	static void EndScene();

	static void Resize(const uint32_t width, const uint32_t height);
	static void Submit(const std::shared_ptr<Shader> &shader, const std::shared_ptr<VertexArray> &vertexArray, const glm::mat4 &transform = glm::mat4(1.0f));

	inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
};

}

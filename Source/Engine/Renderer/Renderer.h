#pragma once

#include "RenderCommand.h"

#include "Camera.h"
#include "Shader.h"

namespace Ultra {

class Renderer {
	struct SceneData {
		glm::mat4 ViewProjectionMatrix;
	};

	static SceneData *mSceneData;

public:
	static void BeginScene(OrthographicCamera &camera);
	static void EndScene();

	static void Submit(const std::shared_ptr<Shader> &shader, const std::shared_ptr<VertexArray> &vertexArray);

	inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
};

}

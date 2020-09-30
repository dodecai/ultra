#pragma once

#include "RenderCommand.h"

#include "Ultra.pch"
#include "Ultra/Core.h"

#include "Camera.h"
#include "RenderPass.h"
#include "Shader.h"

namespace Ultra {

class Renderer {
	struct SceneData {
		glm::mat4 ViewProjectionMatrix;
	};

public:
	static void Load();

	static void BeginScene(PerspectiveCamera &camera);
    static void BeginRenderPass(Reference<RenderPass> renderPass, bool clear = true);
    static void EndRenderPass();
	static void EndScene();

	static void Resize(const uint32_t width, const uint32_t height);

	inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

private:
    static SceneData *mSceneData;
};

}

#pragma once

#include "Ultra.pch"
#include "Core.h"

#include "RendererAPI.h"

namespace Ultra {

class RenderCommand {
	static APP_API RendererAPI *s_RendererAPI;

public:
	static RendererAPI *Get() {
		return s_RendererAPI;
	}

	static void Load() {
		s_RendererAPI->Load();
	}

	static void Clear() {
		s_RendererAPI->Clear();
	};

	static void DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray, uint32_t indexCount = 0) {
		s_RendererAPI->DrawIndexed(vertexArray, indexCount);
	};

	static void SetClearColor(const glm::vec4 &color) {
		s_RendererAPI->SetClearColor(color);
	};

	static void SetViewport(const int32_t x, const int32_t y, const uint32_t width, const uint32_t height) {
		s_RendererAPI->SetViewport(x, y, width, height);
	};

	static void SetWireframeMode(bool status) {
		s_RendererAPI->SetWireframeMode(status);
	}
};
}

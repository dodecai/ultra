#pragma once

#include "Ultra.pch"
#include "Ultra/Core.h"

#include "RendererAPI.h"

namespace Ultra {

class RenderCommand {
	static APP_API RendererAPI *s_RendererAPI;

public:
	static RendererAPI *Get() { return s_RendererAPI; }

	static void Load() {
		s_RendererAPI->Load();
	}

	static void Clear() {
		s_RendererAPI->Clear();
	};

	static void DrawIndexed(uint32_t count, PrimitiveType type, bool depthTest = true) {
		s_RendererAPI->DrawIndexed(count, type, depthTest);
	};

	static void SetClearColor(const glm::vec4 &color) {
		s_RendererAPI->SetClearColor(color);
	};

	static void SetLineThickness(float value = 1.0f) {
		s_RendererAPI->SetLineThickness(value);
	};

	static void SetViewport(const int32_t x, const int32_t y, const uint32_t width, const uint32_t height) {
		s_RendererAPI->SetViewport(x, y, width, height);
	};

	static void SetPolygonMode(PolygonMode mode = PolygonMode::Solid) {
		s_RendererAPI->SetPolygonMode(mode);
	}
};
}

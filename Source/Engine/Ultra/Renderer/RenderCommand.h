#pragma once

#include "Ultra.pch"
#include "Ultra/Core.h"

#include "RendererAPI.h"
#include "Omnia/Core/Application.h"

namespace Ultra {

class RenderCommand {
	static inline APP_API RendererAPI *s_RendererAPI = nullptr;

public:
	static RendererAPI *Get() { return s_RendererAPI; }

	static void Load() {
        s_RendererAPI = RendererAPI::Create();
		s_RendererAPI->Load();
	}
	static void Clear() {
		s_RendererAPI->Clear();
	};

	static void DrawIndexed(uint32_t count, PrimitiveType type, bool depthTest = true) {
		s_RendererAPI->DrawIndexed(count, type, depthTest);
	};

    static GraphicsAPI GetAPI() { return s_RendererAPI->GetAPI(); }
    static void SetAPI(const GraphicsAPI &api) { s_RendererAPI->SetAPI(api); }
    static void Reload() {
        delete s_RendererAPI;
        s_RendererAPI = RendererAPI::Create();
        s_RendererAPI->Load();
    }

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

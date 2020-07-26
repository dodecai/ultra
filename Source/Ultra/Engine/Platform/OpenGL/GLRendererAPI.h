#pragma once

#include "Engine/Renderer/RendererAPI.h"

namespace Ultra {

class GLRendererAPI: public RendererAPI {
public:
	virtual void Load() override;
	virtual void Unload() override;

	virtual void Clear() override;
	virtual void DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray, uint32_t indexCount = 0) override;
	virtual void DrawIndexed(uint32_t count, Type type, bool depthTest = true) override;

	virtual void SetClearColor(const glm::vec4 &color) override;
	virtual void SetLineThickness(float value) override;
	virtual void SetViewport(const int32_t x, const int32_t y, const uint32_t width, const uint32_t height) override;
	virtual void SetWireframeMode(bool status) override;

};

}


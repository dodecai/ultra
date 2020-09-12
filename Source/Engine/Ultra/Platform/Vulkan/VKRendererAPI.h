﻿#pragma once

#include "Ultra/Renderer/RendererAPI.h"

namespace Ultra {

class VKRendererAPI: public RendererAPI {
public:
    // Default
    virtual void Load() override;
    virtual void Unload() override;

    // 
    virtual void Clear() override;
    virtual void DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray, uint32_t indexCount = 0) override;
    virtual void DrawIndexed(uint32_t count, Type type, bool depthTest = true) override;

    // Mutators
    virtual void SetClearColor(const glm::vec4 &color) override;
    virtual void SetLineThickness(float value) override;
    virtual void SetPolygonMode(PolygonMode mode) override;
    virtual void SetViewport(const int32_t x, const int32_t y, const uint32_t width, const uint32_t height) override;
};

}


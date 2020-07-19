#pragma once

#include <glm/glm.hpp>

#include "Ultra.pch"
#include "Core.h"

#include "Camera.h"
#include "Texture.h"
#include "SubTexture.h"

namespace Ultra {

class Renderer2D {
public:
    static void Load();
    static void Unload();

    static void BeginScene(const Camera &camera);
    static void BeginScene(const OrthographicCamera &camera);
    static void Flush();
    static void EndScene();

    // Primitives
    static void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);
    static void DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color);
    static void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Reference<Texture2D> &texture, const float tilingFactor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));
    static void DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Reference<Texture2D> &texture, const float tilingFactor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));
    static void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Reference<SubTexture2D> &subTexture, const float tilingFactor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));
    static void DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Reference<SubTexture2D> &subTexture, const float tilingFactor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));

    static void DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, const float rotation, const glm::vec4 &color);
    static void DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, const float rotation, const glm::vec4 &color);
    static void DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, const float rotation, const Reference<Texture2D> &texture, const float tilingFactor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));
    static void DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, const float rotation, const Reference<Texture2D> &texture, const float tilingFactor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));
    static void DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, const float rotation, const Reference<SubTexture2D> &subTexture, const float tilingFactor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));
    static void DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, const float rotation, const Reference<SubTexture2D> &subTexture, const float tilingFactor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));

    // Statistics
    struct Statistics
    {
        uint32_t DrawCalls = 0;
        uint32_t Triangles = 0;
        uint32_t QuadCount = 0;

        uint32_t GetTotalVertexCount() { return QuadCount * 4; }
        uint32_t GetTotalIndexCount() { return QuadCount * 6; }
    };
    static void ResetStatistics();
    static Statistics GetStatistics();

private:
    static void FlushAndReset();
};

}

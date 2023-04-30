module;

#include <glm/glm.hpp>

export module Ultra.Engine.Renderer2D;

import Ultra.Core;
import Ultra.Logger;

import Ultra.Engine.Camera;
import Ultra.Engine.DesignerCamera;
import Ultra.Engine.PerspectiveCamera;
import Ultra.Renderer.CommandBuffer;
import Ultra.Renderer.RenderDevice;
import Ultra.Renderer.Texture;

export namespace Ultra {

class Renderer2D {
public:
    Renderer2D() = default;
    ~Renderer2D() = default;

    static void Load();
    static void Unload();

    static void StartScene(const Camera &camera);
    static void StartScene(const Camera &camera, const glm::mat4 &transform);
    static void StartScene(const DesignerCamera &camera);
    static void StartScene(const PerspectiveCamera &camera);
    static void FinishScene();
    static void Flush();

    // Primitives
    //static void DrawQuad(const glm::mat4 &transform, const glm::vec4 &color = glm::vec4(1.0f), string id);
    static void DrawQuad(const glm::mat4 &transform, const glm::vec4 &color = glm::vec4(1.0f));
    static void DrawQuad(const glm::mat4 &transform, const Reference<Texture> &texture, const float tilingFactor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));
    //static void DrawQuad(const glm::mat4 &transform, const Reference<SubTexture> &subTexture, const float tilingFactor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));

    static void DrawRotatedQuad(const glm::mat4 &transform, const float rotation, const glm::vec4 &color);
    static void DrawRotatedQuad(const glm::mat4 &transform, const float rotation, const Reference<Texture> &texture, const float tilingFactor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));
    //static void DrawRotatedQuad(const glm::mat4 &transform, const float rotation, const Reference<SubTexture> &subTexture, const float tilingFactor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));

    // Primitives (including transform calculation)
    static void DrawLine(const glm::vec3 &start, const glm::vec3 &end, const glm::vec4 &color = glm::vec4(1.0f));

    static void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);
    static void DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color);
    static void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Reference<Texture> &texture, const float tilingFactor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));
    static void DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Reference<Texture> &texture, const float tilingFactor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));
    //static void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Reference<SubTexture> &subTexture, const float tilingFactor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));
    //static void DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Reference<SubTexture> &subTexture, const float tilingFactor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));

    static void DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, const float rotation, const glm::vec4 &color);
    static void DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, const float rotation, const glm::vec4 &color);
    static void DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, const float rotation, const Reference<Texture> &texture, const float tilingFactor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));
    static void DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, const float rotation, const Reference<Texture> &texture, const float tilingFactor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));
    //static void DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, const float rotation, const Reference<SubTexture> &subTexture, const float tilingFactor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));
    //static void DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, const float rotation, const Reference<SubTexture> &subTexture, const float tilingFactor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));

    // Statistics
    struct Statistics {
        uint32_t DrawCalls = 0;
        uint32_t Triangles = 0;
        uint32_t LineCount = 0;
        uint32_t QuadCount = 0;

        uint32_t GetTotalVertexCount() { return QuadCount * 4 + LineCount * 2; }
        uint32_t GetTotalIndexCount() { return QuadCount * 6 + LineCount * 2; }
    };
    static void ResetStatistics();
    static Statistics GetStatistics();

private:
    static void FlushAndReset(PrimitiveType primitive = PrimitiveType::Triangle);
};

}

module;

#include <glm/glm.hpp>

export module Ultra.Engine.Renderer;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.GFX.Context;

export import Ultra.Engine.Renderer2D;
export import Ultra.Engine.Camera;
export import Ultra.Engine.ZShader;
export import Ultra.Engine.Texture;

import Ultra.Engine.Pipeline;
import Ultra.Engine.IndexBuffer;
import Ultra.Engine.VertexBuffer;

export namespace Ultra {

class Renderer {
    struct SceneData {
        glm::mat4 ViewProjectionMatrix;
    };

public:
    Renderer() = default;
    ~Renderer() = default;

    static void Load();
    static void Unload();

    static void BeginScene();
    static void EndScene();
    static void Submit(const IndexProperties &properties, PrimitiveType type = PrimitiveType::Triangle, bool depthTest = false);

    inline static GraphicsAPI GetAPI() { return Context::API; }

    static void SetClearColor(float red, float green, float blue, float alpha);
    static void Resize(const uint32_t width, const uint32_t height);

    static void Test();

private:
    static SceneData *mSceneData;
};

}

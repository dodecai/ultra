module;

#include <glm/glm.hpp>

export module Ultra.Engine.Renderer;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.GFX.Context;

export import Ultra.Engine.Camera;
export import Ultra.Engine.Shader;
export import Ultra.Engine.Texture;


export namespace Ultra {

class Renderer {
    struct SceneData {
        glm::mat4 ViewProjectionMatrix;
    };

public:
    Renderer() = default;
    ~Renderer() = default;

    static void Load();

    static void BeginScene();
    static void EndScene();

    static void Resize(const uint32_t width, const uint32_t height);

    inline static GraphicsAPI GetAPI() { return Context::API; }

private:
    static SceneData *mSceneData;
};

}

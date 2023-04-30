export module Ultra.Renderer;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.Engine.Camera;
export import Ultra.Engine.Renderer2D;

import Ultra.GFX.Context;
import Ultra.Renderer.RenderDevice;
import Ultra.Renderer.CommandBuffer;
import Ultra.Renderer.Shader;

export namespace Ultra {

enum class RenderAPI {
    DirectX,
    OpenGL,
    Software,
    Vulkan,
};

/// 
/// @brief Agnostic Renderer
/// 
/// @example: How-To
/// auto renderer = Renderer::Create(RenderAPI::OpenGL);
/// 
/// renderer->Load();
/// while (...) { renderer->RenderFrame();
/// renderer->Unload();
///
class Renderer {
protected:
    Renderer() = default;

public:
    virtual ~Renderer() = default;

    static Scope<Renderer> Create(RenderAPI api);

    void Load();
    void RenderFrame();
    void Dispose();
    void Test();

    // Accessors
    static const RenderAPI GetAPI() { return sAPI; }

    // Mutators
    static void SetAPI(const RenderAPI &api) { sAPI = api; }

protected:
    Scope<CommandBuffer> mCommandBuffer;
    Scope<RenderDevice> mRenderDevice;
    static RenderAPI sAPI;
};

RenderAPI Renderer::sAPI = RenderAPI::OpenGL;

}

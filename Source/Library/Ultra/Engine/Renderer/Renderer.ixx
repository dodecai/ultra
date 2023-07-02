export module Ultra.Engine.Renderer;

import Ultra.GFX.Context;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.Engine.Camera;
export import Ultra.Engine.DesignerCamera;
export import Ultra.Engine.Renderer.Buffer;
export import Ultra.Engine.Renderer.CommandBuffer;
export import Ultra.Engine.Renderer.PipelineState;
export import Ultra.Engine.Renderer.RenderDevice;
export import Ultra.Engine.Renderer.Shader;
export import Ultra.Engine.Renderer.Swapchain;
export import Ultra.Engine.Renderer.Texture;
export import Ultra.Engine.Renderer2D;

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

    void DrawGrid(const DesignerCamera &camera);

    // Accessors
    static const RenderAPI GetAPI() { return sAPI; }

    // Mutators
    static void SetAPI(const RenderAPI &api) { sAPI = api; }

protected:
    Scope<CommandBuffer> mCommandBuffer;
    Scope<RenderDevice> mRenderDevice;
    static RenderAPI sAPI;
};

RenderAPI Renderer::sAPI = RenderAPI::Vulkan;

}

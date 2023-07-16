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

    static Scope<Renderer> Create();

    void Load();
    void RenderFrame();
    void Dispose();
    void Test();

    void DrawGrid(const DesignerCamera &camera);

protected:
    Scope<CommandBuffer> mCommandBuffer;
    Scope<RenderDevice> mRenderDevice;
};

}

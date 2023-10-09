export module Ultra.Renderer;

import Ultra.Graphics.Context;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.Renderer.Camera;
export import Ultra.Renderer.DesignerCamera;
export import Ultra.Renderer.Buffer;
export import Ultra.Renderer.CommandBuffer;
export import Ultra.Renderer.PipelineState;
export import Ultra.Renderer.RenderDevice;
export import Ultra.Renderer.Shader;
export import Ultra.Renderer.Swapchain;
export import Ultra.Renderer.Texture;
export import Ultra.Renderer.Viewport;
export import Ultra.Renderer2D;

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
    void Test(const DesignerCamera &camera);

    void DrawGrid(const DesignerCamera &camera);

protected:
    Scope<CommandBuffer> mCommandBuffer;
    Scope<RenderDevice> mRenderDevice;
};

}

export module Ultra.Renderer;

export import Ultra.Core;
export import Ultra.Logger;

import Ultra.Renderer.RenderDevice;
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

    virtual void Load() = 0;
    virtual void LoadShader(ShaderType type, const string &source, const string &entryPoint) = 0;
    virtual void RenderFrame() = 0;
    virtual void Dispose() = 0;

protected:
    Scope<RenderDevice> mRenderDevice;
};

}

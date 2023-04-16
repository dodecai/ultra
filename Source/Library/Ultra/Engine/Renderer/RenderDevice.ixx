export module Ultra.Renderer.RenderDevice;

export import Ultra.Core;
export import Ultra.Logger;

import Ultra.Renderer.Shader;

export namespace Ultra {

/// 
/// @brief Agnostic RenderDevice
///
class RenderDevice {
protected:
    RenderDevice() = default;

public:
    virtual ~RenderDevice() = default;

    static Scope<RenderDevice> Create();
    virtual Scope<Shader> CreateShader(ShaderType type, const string &source, const string &entryPoint) = 0;

    virtual void Load() = 0;
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;
    virtual void Dispose() = 0;
};

}

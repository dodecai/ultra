export module Ultra.Engine.Renderer.RenderDevice;

export import Ultra.Core;
export import Ultra.Logger;

export namespace Ultra {

enum class PrimitiveType {
    Null = 0x0,
    Line = 0x1,
    Circle = 0x2,
    Triangle = 0x3,
};

enum class PolygonMode {
    Null = 0x0,
    Solid = 0x1,
    Wireframe = 0x2,
};

struct RendererCapabilities {
    string Vendor;
    string Model;
    string Version;
    string SLVersion;
    vector<string> Extensions;

    float MaxAnisotropy = 0.0f;
    int MaxSamples = 0;
    int MaxTextureUnits = 0;

    void Log() {
        LogInfo("[Renderer] Vendor:  '{}'", Vendor);
        LogInfo("[Renderer] Model:   '{}'", Model);
        LogInfo("[Renderer] Version: '{}'", Version);
        LogDebug("[Renderer] Shader Version: '{}'", SLVersion);
        LogTrace("[Renderer] Max Anisotropy: '{}'", MaxAnisotropy);
        LogTrace("[Renderer] Max Samples:     '{}'", MaxSamples);
        LogTrace("[Renderer] Max TextureUnits: '{}'", MaxTextureUnits);

    }
    void ShowExtensions() {
        LogTrace("[Renderer] Extensions: ...");
        for (const auto &extension : Extensions) {
            LogTrace("... : '", extension);
        }
    }

};


/// 
/// @brief Agnostic RenderDevice
///
class RenderDevice {
protected:
    RenderDevice() = default;

public:
    virtual ~RenderDevice() = default;

    static Scope<RenderDevice> Create();

    virtual void Load() = 0;
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;
    virtual void Dispose() = 0;

    static RendererCapabilities &GetCapabilities() {
        static RendererCapabilities capabilities;
        return capabilities;
    }

    virtual void SetLineThickness(float value = 1.0f) = 0;
    virtual void SetPolygonMode(PolygonMode mode = PolygonMode::Solid) = 0;
};

}

module;

#include <glm/glm.hpp>

export module Ultra.Engine.RendererAPI;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.GFX.Context;

export namespace Ultra {

enum class PrimitiveType {
    Null        = 0x0,
    Line        = 0x1,
    Circle      = 0x2,
    Triangle    = 0x3,
};

enum class PolygonMode {
    Null        = 0x0,
    Solid       = 0x1,
    Wireframe   = 0x2,
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
		LogInfo("[Renderer] Vendor:  '", Vendor, "'");
		LogInfo("[Renderer] Model:   '", Model, "'");
		LogInfo("[Renderer] Version: '", Version, "'");
		LogDebug("[Renderer] Shader Version: '", SLVersion, "'");
		LogTrace("[Renderer] Max Anisotropy: '", MaxAnisotropy, "'");
		LogTrace("[Renderer] Max Samples: '", MaxSamples, "'");
		LogTrace("[Renderer] Max TextureUnits: '", MaxTextureUnits, "'");

	}
    void ShowExtensions() {
        LogTrace("[Renderer] Extensions: ...");
        for (const auto &extension : Extensions) {
            LogTrace("... : '", extension);
        }
    }

};

class RendererAPI {
public:
    RendererAPI() = default;
    ~RendererAPI() = default;

	static RendererAPI *Create();
	virtual void Load() = 0;
	virtual void Unload() = 0;

	// Commands
	virtual void Clear() = 0;
	virtual void DrawIndexed(uint32_t count, PrimitiveType type, bool depthTest = true) = 0;

	// Accessors
    static const GraphicsAPI GetAPI();
	static RendererCapabilities &GetCapabilities() {
		static RendererCapabilities capabilities;
		return capabilities;
	}

	// Mutators
    static void SetAPI(const GraphicsAPI api);
	virtual void SetClearColor(const glm::vec4 &color) = 0;
	virtual void SetLineThickness(float value = 1.0f) = 0;
	virtual void SetViewport(const int32_t x, const int32_t y, const uint32_t width, const uint32_t height) = 0;
	virtual void SetPolygonMode(PolygonMode mode = PolygonMode::Solid) = 0;
};

}

module;

#include <glm/glm.hpp>

export module Ultra.Engine.RenderCommand;

import Ultra.Core;
import Ultra.Logger;
import Ultra.Engine.RendererAPI;

export namespace Ultra {

class RenderCommand {
public:
    RenderCommand() = default;
    ~RenderCommand() = default;

    static void Load() {
        sRendererAPI = RendererAPI::Create();
        sRendererAPI->Load();
    }
    static void Reload() {
        delete sRendererAPI;
        sRendererAPI = RendererAPI::Create();
        sRendererAPI->Load();
    }

    // Methods
    static void Clear() {
        sRendererAPI->Clear();
    }
    static void DrawIndexed(uint32_t count, PrimitiveType type, bool depthTest = true) {
        sRendererAPI->DrawIndexed(count, type, depthTest);
    }
    
    static GraphicsAPI GetAPI() {
        sRendererAPI->GetAPI();
    }
    static void SetAPI(const GraphicsAPI &api) {
        sRendererAPI->SetAPI(api);
    }
    static void SetClearColor(const glm::vec4 &color) {
        sRendererAPI->SetClearColor(color);
    };
    static void SetLineThickness(float value = 1.0f) {
        sRendererAPI->SetLineThickness(value);
    };
    static void SetViewport(const int32_t x, const int32_t y, const uint32_t width, const uint32_t height) {
        sRendererAPI->SetViewport(x, y, width, height);
    };
    static void SetPolygonMode(PolygonMode mode = PolygonMode::Solid) {
        sRendererAPI->SetPolygonMode(mode);
    }
private:
    static inline RendererAPI *sRendererAPI = nullptr;
};

}

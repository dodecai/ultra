export module Ultra.Graphics.Context;

import Ultra.Core;

export namespace Ultra {

enum class GraphicsAPI {
    DirectX,
    OpenGL,
    Software,
    Vulkan,
};

// The data structure is platform specific, so we only forward declare it here.
struct ContextData;

class Context {
public:
    Context() = default;
    virtual ~Context() = default;

    static Reference<Context> Create(void *window);
    static void Destroy();

    virtual void Load() = 0;

    virtual void Attach() = 0;
    virtual void Detach() = 0;

    // Accessors
    virtual void *GetNativeContext() = 0;
    virtual bool const IsCurrentContext() = 0;

    // Conversion
    template<typename T>
    T *As() {
        return reinterpret_cast<T *>(this);
    }

    // Accessors
    static const GraphicsAPI GetAPI() { return API; }

    // Mutators
    virtual void Clear() = 0;
    static void SetAPI(const GraphicsAPI &api) { API = api; }
    virtual void SetViewport(uint32_t width, uint32_t height, int32_t x = 0, int32_t y = 0) = 0;
    virtual void SwapBuffers() = 0;

    // Settings
    virtual void SetVSync(bool activate) = 0;

    static GraphicsAPI API;
};

GraphicsAPI Context::API = GraphicsAPI::OpenGL;

}

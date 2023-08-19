#include <Settings.h>
#include <Ultra/EntryPoint.h>

import Ultra;

#define NATIVE_RENDERER1

#ifdef NATIVE_RENDERER
    import Ultra.Module.Phoenix;
#else
    import Ultra.Math;
    import Ultra.Engine.Resource;
    import Ultra.Engine.Renderer.Viewport;
    import Ultra.Engine.UIRenderer;
    import Ultra.Utility.String;
    import Ultra.UI.HmGui;
#endif

namespace Ultra {

using namespace Ultra::UI;

// Application
class App: public Application {
public:
    // Constructors and Destructor
#ifdef NATIVE_RENDERER
    App(const ApplicationProperties &properties): Application(ApplicationProperties(true)) {}
#else
    App(const ApplicationProperties &properties): Application(properties) {}
#endif
    ~App() = default;

    // Methods
    void Create() {
    #ifdef NATIVE_RENDERER
        Engine_Init(2, 1);
        mLua = Lua_Create();

        char const *entryPoint = "./Script/Main.lua";

        if (!File_Exists(entryPoint)) {
            Directory_Change("../../Assets/Phoenix/");
            if (!File_Exists(entryPoint))
                Fatal("can't find script entrypoint <%s>", entryPoint);
        }

        Lua_SetBool(mLua, "__debug__", false); //DEBUG > 0
        Lua_SetBool(mLua, "__embedded__", true);
        Lua_SetNumber(mLua, "__checklevel__", 0); // CHECK_LEVEL [=0]
        Lua_SetStr(mLua, "__app__", "TestHmGui");
        Lua_DoFile(mLua, "./Script/Main.lua");
    #else
        mRenderer = Renderer::Create();
        mViewport = Viewport::Create({ 0, 0, 1280, 1024, true });

        Resource::Instance();
    #endif
    }

    void Destroy() {
    #ifdef NATIVE_RENDERER
        Lua_Free(mLua);
        Engine_Free();
    #endif
    }

    void Update([[maybe_unused]] Timestamp deltaTime) {
    #ifdef NATIVE_RENDERER
        Engine_Update();
    #else#
        HmGui::ShowDemo(deltaTime);
        mRenderer->RenderFrame();
        HmGui::Draw(mViewport);
    #endif
    }

private:
#ifdef NATIVE_RENDERER
    Lua *mLua = nullptr;
#else
    Scope<Renderer> mRenderer;
    Scope<Viewport> mViewport;
#endif
};

// Application Entry-Point
Application *CreateApplication() {
    return new App({ "Game", "1280x1024", GraphicsAPI::OpenGL });
}

}

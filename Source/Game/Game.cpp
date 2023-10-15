import <Settings.h>;
import <Ultra/EntryPoint.h>;
import Ultra;
import Ultra.Module.Phoenix;

namespace Ultra {

// Application
class App: public Application {
public:
    // Constructors and Destructor
    App(const ApplicationProperties &properties): Application(ApplicationProperties(true)) {}
    ~App() = default;

    // Methods
    void Create() {
        Engine_Init(2, 1);
        mLua = Lua_Create();

        char const *entryPoint = "./Script/Main.lua";

        if (!File_Exists(entryPoint)) {
            Directory_Change("../../3rd-Party/LibPHX/assets/");
            if (!File_Exists(entryPoint))
                Fatal("can't find script entrypoint <%s>", entryPoint);
        }

        Lua_SetBool(mLua, "__debug__", false); //DEBUG > 0
        Lua_SetBool(mLua, "__embedded__", true);
        Lua_SetNumber(mLua, "__checklevel__", 0); // CHECK_LEVEL [=0]
        Lua_SetStr(mLua, "__app__", "LTheory");
        Lua_DoFile(mLua, "./Script/Main.lua");
    }

    void Destroy() {
        Lua_Free(mLua);
        Engine_Free();
    }

    void Update([[maybe_unused]] Timestamp deltaTime) {
        Engine_Update();
    }

private:
    Lua *mLua = nullptr;
    Scope<Renderer> mRenderer;
};

// Application Entry-Point
Application *CreateApplication() {
    return new App({ "Game", "1280x1024", GraphicsAPI::OpenGL });
}

}

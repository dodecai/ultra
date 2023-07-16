#include <Settings.h>
#include <Ultra/EntryPoint.h>

import Ultra.Module.Phoenix;

namespace Ultra {

// Application
class App: public Application {
public:
    // Constructors and Destructor
    App() = default;
    ~App() = default;

    // Methods
    void Create() {
        Engine_Init(2, 1);
        Lua *lua = Lua_Create();
        char const *entryPoint = "./Script/Main.lua";

        if (!File_Exists(entryPoint)) {
            Directory_Change("../../Assets/Phoenix/");
            if (!File_Exists(entryPoint))
                Fatal("can't find script entrypoint <%s>", entryPoint);
        }

        Lua_SetBool(lua, "__debug__", false); //DEBUG > 0
        Lua_SetBool(lua, "__embedded__", true);
        Lua_SetNumber(lua, "__checklevel__", 0); // CHECK_LEVEL [=0]
        Lua_SetStr(lua, "__app__", "LTheory");
        Lua_DoFile(lua, "./Script/Main.lua");
        Lua_Free(lua);
        Engine_Free();
    }

    void Destroy() {}
    void Update([[maybe_unused]] Timestamp deltaTime) {
        Exit();
    }
};

// Application Entry-Point
Application *CreateApplication() {
    return new App();
}

}

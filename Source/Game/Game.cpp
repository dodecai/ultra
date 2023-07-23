#include <Settings.h>
#include <Ultra/EntryPoint.h>

import Ultra;

#define NATIVE_RENDERER1

#ifdef NATIVE_RENDERER
    import Ultra.Module.Phoenix;
#else
    import Ultra.Engine.Phoenix;
    import Ultra.UI.HmGui;
#endif
import Ultra.Utility.String;

namespace Ultra {

using namespace Ultra::UI;

using CheckListEntry = std::map<std::string, bool>;
using CheckList = std::map<std::string, CheckListEntry>;

CheckList gCheckList = {
    { "Basic Widgets", {
        { "Text", true },
        { "Buttons", true },
        { "Checkboxes", true },
        { "Sliders", false },
        { "Radio Groups", false },
        { "Selectable", false },
        { "Tooltips", false },
    }},
    { "Layout", {
        { "Deferred Layout Pass", true },
        { "Horizontal Groups", true },
        { "Vertical Groups", true },
        { "Stacked Groups", true },
        { "Group Padding", true },
        { "Group Margins", false },
        { "Child Spacing ", true },
        { "ScrollViews", true },
        { "Windows", true },
    }},
    { "Input", {
        { "Clip Groups", true },
        { "Input Clipping", true },
        { "Sticky Drag", false },
        { "Keyboard Navigation", false },
        { "Gamepad Navigation", false },
    }},
    { "Draw", {
        { "Draw Layers", true },
        { "Shader-Based Render", true },
        { "Glyph Render", false },
        }},
    { "Technical", {
        { "Hash Storage", true },
        { "Hash Storage Invalidation", false },
        { "Deferred Metrics", true },
    }},
};

constexpr const char * gCodeExample = R"(
#include <print>

class Foo {
public:
    Foo() = default;
    virtual ~Foo() = default;
};

class Bar: Foo {
public:
    Bar() = default;
    virtual ~Bar() = default;
};


int main() {
    print("{}", "Hello World!");
}
)";


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
        RenderState::PushAllDefaults();

        Resource::Instance();

        mFontExo2Bold = Font::Load("Exo2Bold", 30);
        mFontFiraMono = Font::Load("FiraMono", 10);
        mFontRajdhani = Font::Load("Rajdhani", 18);
    #endif
    }

    void Destroy() {
    #ifdef NATIVE_RENDERER
        Lua_Free(mLua);
        Engine_Free();
    #else
        Font::Free(mFontExo2Bold);
        Font::Free(mFontFiraMono);
        Font::Free(mFontRajdhani);
    #endif
    }

    void Update([[maybe_unused]] Timestamp deltaTime) {
        #ifdef NATIVE_RENDERER
            Engine_Update();
        #else

            HmGui::Begin(1280, 1024);
            ShowSimple();
            ShowMetrics(deltaTime);
            ShowToDoWindow();
            HmGui::End();

            Viewport::Push(0, 0, 1280, 1024, true);
            Draw::Clear(0, 0, 0, 0);
            HmGui::Draw();
            Viewport::Pop();

            Metric::Reset();
        #endif
    }


#ifdef NATIVE_RENDERER
#else
    void ShowMetrics(float deltaTime) {
        HmGui::BeginWindow("Metrics");
            HmGui::Text(std::format("fps: {:.2}", 1.0 / deltaTime).c_str());
        HmGui::EndWindow();
    }

    void ShowSimple() {
        HmGui::BeginWindow("Test");

        HmGui::BeginGroupX();
            HmGui::Button(" < "); HmGui::SetStretch(0, 1);
            HmGui::Button("Tab1");
            HmGui::Button("Tab2");
            HmGui::Button("Tab3");
            HmGui::Button(" > "); HmGui::SetStretch(0, 1);
        HmGui::EndGroup();
        HmGui::SetStretch(1, 1);

        HmGui::BeginGroupX();
            HmGui::BeginGroupY();
                HmGui::SetPadding(4, 4);
                HmGui::Text("Welcome to...");
                HmGui::SetAlign(0.5, 0.5);

                HmGui::PushTextColor(1.0, 0.0, 0.3, 1.0);
                HmGui::PushFont(mFontExo2Bold);
                HmGui::Text("~ Hybrid Mode ~");
                HmGui::PopStyle(2);
                HmGui::SetAlign(0.5, 0.5);

                HmGui::Text("GUI!");
                HmGui::SetAlign(0.5, 0.5);
                HmGui::Button("Not-So-Stretchy");
                HmGui::SetStretch(1, 0);
                HmGui::Button("Stretchy");
                HmGui::SetStretch(1, 1);

                HmGui::BeginGroupX();
                for (auto i = 1; i < 3; i++) {
                    HmGui::BeginGroupY();
                    for (auto j = 1; j < 3; j++) {
                        HmGui::Button(":)");
                    }
                    HmGui::EndGroup();
                    HmGui::SetStretch(1, 1);
                }
                HmGui::EndGroup();
                HmGui::SetStretch(1, 1);
            HmGui::EndGroup();
            HmGui::SetAlign(0, 0.0);
            HmGui::SetStretch(1, 1);


            HmGui::BeginGroupY();
                HmGui::SetPadding(4, 4);
                if (HmGui::Button("-- OPT 1 --")) {
                    logger << "Opt 1!\n";
                }
                HmGui::Button("-- OPT 2 --");
                HmGui::Checkbox("Yas", true);
                HmGui::Checkbox("Nope", false);
                HmGui::Checkbox("Possibly?", false);
                HmGui::Button("DONE");
            HmGui::EndGroup();
            HmGui::SetAlign(0, 1.0);
            HmGui::SetStretch(1, 1);

            HmGui::BeginGroupY();
                HmGui::SetPadding(4, 4);
                for (auto i = 1; i < 9; i++) {
                    HmGui::BeginGroupX();
                    for (auto j = 1; j < i; j++) {
                        auto result = std::format("{}.{}", i, j);
                        HmGui::Button(result.c_str());
                    }
                    HmGui::EndGroup();
                    HmGui::SetAlign(0.5, 0.5);

                }
            HmGui::EndGroup();
            ShowToDo();
        HmGui::EndGroup();
        HmGui::SetStretch(1, 0);

        HmGui::Text("Behold, the codez! \\o/");
        HmGui::BeginGroupX();
        for (auto i = 0; i < 2; i++) {
            HmGui::BeginScroll(200);
                HmGui::PushTextColor(0.1, 0.5, 1.0, 1.0);
                HmGui::PushFont(mFontFiraMono);
                auto lines = String::Split(gCodeExample, '\n');
                for (auto &line : lines) {
                    HmGui::Text(line.c_str());
                }
                HmGui::PopStyle(2);
            HmGui::EndScroll();
        }
        HmGui::EndGroup();

        HmGui::EndWindow();
        HmGui::SetAlign(0.5, 0.5);

    }

    void ShowToDo() {
        HmGui::BeginScroll(256);
        HmGui::SetSpacing(8);
        for (auto &[entry, list] : gCheckList) {
            HmGui::TextEx(mFontRajdhani, entry.c_str(), 1, 1, 1, 1);
            HmGui::BeginGroupY();
            HmGui::SetSpacing(2);
            HmGui::SetPaddingLeft(12);
            for (auto &[key, value] : list) {
                value = HmGui::Checkbox(key.c_str(), value);
            }
            HmGui::EndGroup();
        }
        HmGui::EndScroll();
    }

    void ShowToDoWindow() {
        HmGui::BeginWindow("ToDo");
        ShowToDo();
        HmGui::EndWindow();
    }
#endif

private:
#ifdef NATIVE_RENDERER
    Lua *mLua = nullptr;
#else
    Scope<Renderer> mRenderer;
    FontData *mFontExo2Bold = nullptr;
    FontData *mFontFiraMono = nullptr;
    FontData *mFontRajdhani = nullptr;
#endif
};

// Application Entry-Point
Application *CreateApplication() {
    return new App({ "Game", "1280x1024", GraphicsAPI::OpenGL });
}

}

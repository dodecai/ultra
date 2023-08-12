﻿#include <Settings.h>
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

// The amazing and well known foo class, which doesn't need to be explained.
class Foo {
public:
    Foo() = default;
    virtual ~Foo() = default;
};

// As amazing as foo can be, it is nothing without Bar.
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
        mViewport = Viewport::Create({ 0, 0, 1280, 1024, true });

        Resource::Instance();

        mFontExo2Bold = CreateScope<Font>("Exo2Bold", 30);
        mFontFiraMono = CreateScope<Font>("FiraMono", 10);
        mFontRajdhani = CreateScope<Font>("Rajdhani", 18);
    #endif
    }

    void Destroy() {
    #ifdef NATIVE_RENDERER
        Lua_Free(mLua);
        Engine_Free();
    #else
    #endif
    }

    void Update([[maybe_unused]] Timestamp deltaTime) {
        mFrames++;
        mDeltaDelay += deltaTime;
        if (mDeltaDelay > 1.0f) {
            mFPS = mFrames;
            mMSPF = 1000.0 / mFrames;
            mFrames = 0;
            mDeltaDelay = 0;
        }

    #ifdef NATIVE_RENDERER
        Engine_Update();
    #else#
        HmGui::Begin({ 1280, 1024 });
        HmGui::ShowDemo();

        ShowSimple();
        ShowMetrics();
        ShowToDoWindow();
        HmGui::End();

        mRenderer->RenderFrame();
        HmGui::Draw(mViewport);
    #endif
    }


#ifdef NATIVE_RENDERER
#else
    void ShowMetrics() {
        HmGui::BeginWindow("Metrics");
        HmGui::BeginGroup(UI::Layout::Vertical);
        HmGui::Text("Applicaiton Statistics");
            HmGui::Text(std::format("frames/s: {:.2f}", mFPS));
            HmGui::Text(std::format("ms/Frame: {:.2f}", mMSPF));
        HmGui::EndGroup();
        HmGui::EndWindow();
        HmGui::SetAlign(1.0f, 1.0f);
    }

    void ShowSimple() {
        HmGui::BeginWindow("Test");

        HmGui::BeginGroup(UI::Layout::Horizontal);
            HmGui::Button(" < "); HmGui::SetStretch(0, 1);
            HmGui::Button("Tab1");
            HmGui::Button("Tab2");
            HmGui::Button("Tab3");
            HmGui::Button(" > "); HmGui::SetStretch(0, 1);
        HmGui::EndGroup();
        
        HmGui::SetStretch(1, 1);

        HmGui::BeginGroup(UI::Layout::Horizontal);
            HmGui::BeginGroup(UI::Layout::Vertical);
                HmGui::SetPadding(4, 4);
                HmGui::Text("Welcome to...");
                HmGui::SetAlign(0.5, 0.5);

                HmGui::PushTextColor({ 1.0f, 0.0f, 0.3f, 1.0f });
                HmGui::PushFont(mFontExo2Bold.get());
                HmGui::Text("~ Hybrid Mode ~");
                HmGui::PopStyle(2);
                HmGui::SetAlign(0.5, 0.5);

                HmGui::Text("GUI!");
                HmGui::SetAlign(0.5, 0.5);
                HmGui::Button("Not-So-Stretchy");
                HmGui::SetStretch(1, 0);
                HmGui::Button("Stretchy");
                HmGui::SetStretch(1, 1);

                HmGui::BeginGroup(UI::Layout::Horizontal);
                for (auto i = 1; i < 3; i++) {
                    HmGui::BeginGroup(UI::Layout::Vertical);
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


            HmGui::BeginGroup(UI::Layout::Vertical);
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

            HmGui::BeginGroup(UI::Layout::Vertical);
                HmGui::SetPadding(4, 4);
                for (auto i = 1; i < 9; i++) {
                    HmGui::BeginGroup(UI::Layout::Horizontal);
                    for (auto j = 1; j < i; j++) {
                        auto result = std::format("{}.{}", i, j);
                        HmGui::Button(result);
                    }
                    HmGui::EndGroup();
                    HmGui::SetAlign(0.5, 0.5);

                }
            HmGui::EndGroup();
            ShowToDo();
        HmGui::EndGroup();
        HmGui::SetStretch(1, 0);


        HmGui::Text("水 Behold, the codez! \\o/");
        HmGui::BeginGroup(UI::Layout::Horizontal);
        for (auto i = 0; i < 2; i++) {
            HmGui::BeginScroll(200);
                HmGui::PushTextColor({0.1f, 0.5f, 1.0f, 1.0f});
                HmGui::PushFont(mFontFiraMono.get());
                auto lines = String::Split(gCodeExample, '\n');
                for (auto &line : lines) {
                    HmGui::Text(line);
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
            HmGui::TextExtended(entry, { 1.0f, 1.0f, 1.0f, 1.0f }, mFontRajdhani.get());
            HmGui::BeginGroup(Layout::Vertical);
            HmGui::SetSpacing(2);
            HmGui::SetPaddingLeft(12);
            for (auto &[key, value] : list) {
                value = HmGui::Checkbox(key, value);
            }
            HmGui::EndGroup();
        }
        HmGui::EndScroll();
    }

    void ShowToDoWindow() {
        HmGui::BeginWindow("ToDo");
        ShowToDo();
        HmGui::EndWindow();
        HmGui::SetAlign(1.0f, 0.0f);
    }
#endif

private:
#ifdef NATIVE_RENDERER
    Lua *mLua = nullptr;
#else
    double mFrames {};
    double mFPS {};
    double mMSPF {};
    double mDeltaDelay {};

    Scope<Renderer> mRenderer;
    Scope<Viewport> mViewport;

    Scope<Font> mFontExo2Bold = nullptr;
    Scope<Font> mFontFiraMono = nullptr;
    Scope<Font> mFontRajdhani = nullptr;
#endif
};

// Application Entry-Point
Application *CreateApplication() {
    return new App({ "Game", "1280x1024", GraphicsAPI::OpenGL });
}

}

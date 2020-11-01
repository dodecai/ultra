#pragma once

namespace Ultra {
class Designer;
}

// Designer Menu
namespace Menu {

// Components
class MenuFile {
public:
    MenuFile(Ultra::Designer *designer) { mContext = designer; }
    ~MenuFile() = default;

    void Render() {
        if (ImGui::BeginMenu("File")) {

            ImGui::MenuItem("Load & Save", NULL, false, false);
            if (ImGui::MenuItem("New", "Ctrl+N"))   mContext->NewScene();
            if (ImGui::MenuItem("Open", "Ctrl+O"))  mContext->OpenScene();
            if (ImGui::BeginMenu("Open Recent")) {
                ImGui::MenuItem("-");
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Save", "Ctrl+S"))              mContext->SaveScene();
            if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S"))   mContext->SaveScene();
            ImGui::Separator();
            if (ImGui::MenuItem("Close", "Ctrl+C"))             mContext->CloseScene();
            if (ImGui::MenuItem("Exit", "Alt+F4"))              Ultra::Application::Get().Exit();

            ImGui::EndMenu();
        }
    }

private:
    Ultra::Designer *mContext = nullptr;
};

class MenuEdit {
public:
    MenuEdit(Ultra::Designer *designer) { mContext = designer; }
    ~MenuEdit() = default;

    void Render() {
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}

            ImGui::EndMenu();
        }
    }

private:
    Ultra::Designer *mContext = nullptr;
};

class MenuView {
public:
    MenuView(Ultra::Designer *designer) { mContext = designer; }
    ~MenuView() = default;

    void Render() {
        if (ImGui::BeginMenu("View")) {
            bool ViewportRender;
            ImGui::Checkbox("Viewport", &ViewportRender);
            ImGui::EndMenu();
        }
    }

private:
    Ultra::Designer *mContext = nullptr;
};

class MenuTools {
public:
    MenuTools(Ultra::Designer *designer) { mContext = designer; }
    ~MenuTools() = default;

    void Render() {
        if (ImGui::BeginMenu("Tools")) {
            ImGui::Text("Style");

            static ColorThemes themeSelected = ColorThemes::Dark;
            static ColorPalette primarySelected = ColorPalette::Cyan;
            static ColorPalette warnSelected = ColorPalette::Red;

            static int theme = 0;
            if (ImGui::Combo("Theme", &theme, "Dark\0Light\0\0")) {
                switch (theme) {
                    case 0:		{ themeSelected = ColorThemes::Dark;	break; }
                    case 1:		{ themeSelected = ColorThemes::Light;	break; }
                    default:	{ SetRayTeakStyle();					break; }
                }
                SetStyle(themeSelected,	primarySelected, warnSelected);
            }
            static int color = 0;
            if (ImGui::Combo("Color", &color, "Default\0Red\0Pink\0Purple\0DeepPurple\0Indigo\0Blue\0LightBlue\0Cyan\0Teal\0Green\0LightGreen\0Lime\0Yellow\0Orange\0DeepOrange\0Brown\0Gray\0Amber\0\0")) {
                switch (color) {
                    case 1:		{ primarySelected =  ColorPalette::Red;			warnSelected = ColorPalette::Yellow;	break; }
                    case 2:		{ primarySelected =  ColorPalette::Pink;		warnSelected = ColorPalette::Yellow;	break; }
                    case 3:		{ primarySelected =  ColorPalette::Purple;		warnSelected = ColorPalette::Yellow;	break; }
                    case 4:		{ primarySelected =  ColorPalette::DeepPurple;	warnSelected = ColorPalette::Yellow;	break; }
                    case 5:		{ primarySelected =  ColorPalette::Indigo;		warnSelected = ColorPalette::Red;		break; }
                    case 6:		{ primarySelected =  ColorPalette::Blue;		warnSelected = ColorPalette::Red;		break; }
                    case 7:		{ primarySelected =  ColorPalette::LightBlue;	warnSelected = ColorPalette::Red;		break; }
                    case 8:		{ primarySelected =  ColorPalette::Cyan;		warnSelected = ColorPalette::Red;		break; }
                    case 9:		{ primarySelected =  ColorPalette::Teal;		warnSelected = ColorPalette::Red;		break; }
                    case 10:	{ primarySelected =  ColorPalette::Green;		warnSelected = ColorPalette::Red;		break; }
                    case 11:	{ primarySelected =  ColorPalette::LightGreen;	warnSelected = ColorPalette::Red;		break; }
                    case 12:	{ primarySelected =  ColorPalette::Lime;		warnSelected = ColorPalette::Red;		break; }
                    case 13:	{ primarySelected =  ColorPalette::Yellow;		warnSelected = ColorPalette::Red;		break; }
                    case 14:	{ primarySelected =  ColorPalette::Orange;		warnSelected = ColorPalette::Red;		break; }
                    case 15:	{ primarySelected =  ColorPalette::DeepOrange;	warnSelected = ColorPalette::Red;		break; }
                    case 16:	{ primarySelected =  ColorPalette::Brown;		warnSelected = ColorPalette::Red;		break; }
                    case 17:	{ primarySelected =  ColorPalette::Gray;		warnSelected = ColorPalette::Red;		break; }
                    case 18:	{ primarySelected =  ColorPalette::Amber;		warnSelected = ColorPalette::Red;		break; }
                    default:	{ SetRayTeakStyle();																	break; }
                }
                SetStyle(themeSelected,	primarySelected, warnSelected);
            }

            if (ImGui::BeginMenu("Color Overview")) {
                float sz = ImGui::GetTextLineHeight();
                for (int i = 0; i < ImGuiCol_COUNT; i++) {
                    const char *name = ImGui::GetStyleColorName((ImGuiCol)i);
                    ImVec2 p = ImGui::GetCursorScreenPos();
                    ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
                    ImGui::Dummy(ImVec2(sz, sz));
                    ImGui::SameLine();
                    ImGui::MenuItem(name);
                }
                ImGui::EndMenu();
            }
            ImGui::Separator();

            ImGui::Separator();
            if (ImGui::BeginMenu("Renderer Settings")) {
                static int api = 0;
                if (ImGui::Combo("API", &api, "OpenGL\0Vulkan\0\0")) {
                    switch (api) {
                        case 0:     { break; }
                        case 1:     { break; }
                        default:    { break; }
                    }
                }

                static bool wireframeMode = false;
                static bool verticalSync = false;
                if (ImGui::Checkbox("Wireframe", &wireframeMode)) {
                    Ultra::RenderCommand::SetPolygonMode(wireframeMode ? Ultra::PolygonMode::Wireframe : Ultra::PolygonMode::Solid);
                }
                if (ImGui::Checkbox("VSync", &verticalSync)) {
                    Ultra::Application::GetContext().SetVSync(verticalSync);
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
    }

private:
    Ultra::Designer *mContext = nullptr;
};

class MenuHelp {
public:
    MenuHelp(Ultra::Designer *designer) { mContext = designer; }
    ~MenuHelp() = default;

    void Render() {
        static bool showAbout = false;

        if (ImGui::BeginMenu("Help")) {
            ImGui::Separator();
            ImGui::MenuItem("About", NULL, &showAbout);
            ImGui::EndMenu();
        }

        if (showAbout) { ShowAboutWindow(&showAbout); };
    }

    static void ShowAboutWindow(bool *show) {
        if (ImGui::Begin("About Ultra", show, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::TextColored(ImTextColorHighlight, "Ultra Spectra");
            ImGui::Text("- License: %s", "MIT");
            ImGui::Text("- Version: v%s", Ultra::AppVersion);
            ImGui::Text("This project was created for educational purposes, use it at your own risk!");

            ImGui::Separator();
            if (ImGui::CollapsingHeader("Information")) {
                bool copyToCliipboard = ImGui::Button("Copy to clipboard");
                if (copyToCliipboard) {
                    ImGui::LogToClipboard();
                    ImGui::LogText("```\n");
                }

                ImVec2 childSize = ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 8);
                ImGui::BeginChildFrame(ImGui::GetID("cfg_infos"), childSize, ImGuiWindowFlags_NoMove);
                ImGui::TextColored(ImTextColorHighlight, "Build Information");
                ImGui::Text("Compiler %s [%s]", APP_COMPILER, APP_COMPILER_VERSION);
                ImGui::Text("- DebugMode: %d", (int)Ultra::DebugMode);
                ImGui::Separator();
                ImGui::Text("Language Version %d", (int)__cplusplus);

                ImGui::Separator();
                ImGui::EndChildFrame();

                if (copyToCliipboard) {
                    ImGui::LogText("\n```\n");
                    ImGui::LogFinish();
                }
            }

            ImGui::Separator();
            if (ImGui::CollapsingHeader("3rd-Party Notes")) {
                ImVec2 childSize = ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 8);
                ImGui::BeginChildFrame(ImGui::GetID("cfg_infos2"), childSize, ImGuiWindowFlags_NoMove);
                ImGui::TextColored(ImTextColorHighlight, "Dear ImGui");
                ImGui::Text("- Information: %s" "By Omar Cornut and all Dear ImGui contributors");
                ImGui::Text("- License: %s", "MIT");
                ImGui::Text("- Version: %s", ImGui::GetVersion());
                ImGui::Separator();
                ImGui::EndChildFrame();
            }

            ImGui::End();
        }
    }

private:
    Ultra::Designer *mContext = nullptr;
};

}

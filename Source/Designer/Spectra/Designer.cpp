#pragma once

#include <glm/gtx/matrix_decompose.hpp>

#include <Ultra.h>
#include <Ultra/Audio/Audio.h>
#include <Ultra/Script/CameraController.h>

#include "Menu/Menues.h"
#include "Panel/Panels.h"

//#define RAW_RENDERER_TEST

namespace Ultra {

Reference<Scene> sCurrentScene = nullptr;

void test(const string &value) {
    AppLog(value);
}

enum class DesignerState {
    Edit,
    Play,
    Pause
};

class Designer: public Layer {
public:
    Designer(): Layer("Core") {
        ClearColor =  { 0.0f, 0.0f, 0.0f, 0.72f };
        GridColor = { 0.8f, 0.8f, 0.2f, 0.72f };
        SetStyle();
    }

    void Create() override {
        CurrentScene = CreateReference<Scene>();
        Serializer serializer(CurrentScene);

        Debug();


        Browser.SetContext(CurrentScene);
        ScriptEditor.SetText("#include <Ultra.h>\r\n\r\nnamespace Ultra {\r\n\r\nclass Player {\r\n};\r\n\r\n}\r\n");

        auto &dimension = Application::GetWindow().GetContexttSize();
        CurrentScene->Resize(dimension.Width, dimension.Height);

        sCurrentScene = CurrentScene;
    }

    void Debug() {
        Serializer serializer(CurrentScene);
        serializer.Deserialize("./Data/Scene.yaml");

        Ultra::Audio::Player::Initialize();
        BackgroundMusic = CurrentScene->GetEntity("Background Music");

        BackgroundMusicPlayer = Ultra::Audio::Source::LoadFromFile(BackgroundMusic.GetComponent<Component::Sound>());
        BackgroundMusicPlayer.SetRepeat(true);

        CameraEntity = CurrentScene->GetEntity("Camera");
        CameraEntity.AddComponent<Component::NativeScript>().Bind<CameraSystem>();
    }

    void GuiUpdate() override {
        // ToDo: Disable UI Elements while scene is playing
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        // ToDo: Put disabled Items here
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
        Browser.GuiUpdate();

        static bool alwaysOpen = true;
        if (Menu::ViewportRender) {
            mViewport.Enable();
        } else {
            mViewport.Disable();
        }

        /**
         * @brief Menus
        */
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open", "Ctrl+O")) {
                    CurrentScene->Clear();
                    Serializer serializer(CurrentScene);
                    auto result = Omnia::Application::GetDialog().OpenFile();
                    serializer.Deserialize(result);
                    CameraEntity = CurrentScene->GetEntity("Camera");
                    CameraEntity.AddComponent<Component::NativeScript>().Bind<CameraSystem>();
                    auto &dimension = Application::GetWindow().GetContexttSize();
                    CurrentScene->Resize(dimension.Width, dimension.Height);
                }
                if (ImGui::MenuItem("Close Test", "Ctrl+O")) {
                    CurrentScene->Clear();
                }
                if (ImGui::MenuItem("Save Test", "Ctrl+N")) {
                    Serializer serializer(CurrentScene);
                    auto result = Omnia::Application::GetDialog().SaveFile();
                    serializer.Serialize(result);
                }
                ImGui::EndMenu();
            }
            Menu::ShowMenuFile();
            Menu::ShowMenuEdit();
            Menu::ShowMenuView();
            Menu::ShowMenuTools();
            Menu::ShowMenuHelp();
            ImGui::EndMenuBar();
        }

        /**
         * @brief Panels
        */
        // Browser

        // Console
        static bool consoleOpen = true;
        if (consoleOpen) View::ShowExampleAppConsole(&consoleOpen);

        // Scene
        mViewport.Update();

        // Properties
        auto &selectedNode = Browser.GetSelectedNode();
        SceneProperties.SetContext(selectedNode);
        SceneProperties.GuiUpdate();

        ImGui::Begin("Script Editor");
        ScriptEditor.Render("Script Editor");
        ImGui::End();


        // Test
        ImGui::Begin("Test");
        #ifdef StepI
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Grid")) {
            UI::Property("Quad Size:", QuadSize);
            UI::Property("Grid Steps:", GridSteps, 0.075f, 1.0f);
            UI::Property("Grid X:", GridX, 1.0f, 25.0f);
            UI::Property("Grid Y:", GridY, 1.0f, 25.0f);
            UI::Property("Rotation Speed:", RoatationSpeed, 0.0f, 120.0f);

        }
        #endif
        ImGui::End();

        // Statistics
        ImGui::Begin("Statistics");
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Application")) {
            // Some fun stuff
            if (Application::GetStatistics().fps < 30) {
                ImGui::PushStyleColor(ImGuiCol_PlotLines, ImTextColorHighlightWarn);
                ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImTextColorHighlightWarn);
            }

            // Visualization
                // frames per second
            static float fpsValues[90] = {};
            static int fpsOffset = 0;
            static double fpsRefresh = 0.0;
            if (fpsRefresh == 0.0) fpsRefresh = ImGui::GetTime();
            while (fpsRefresh < ImGui::GetTime()) {
                static float phase = 0.1f;
                fpsValues[fpsOffset] = Application::GetStatistics().fps;
                fpsOffset = (fpsOffset + 1) % IM_ARRAYSIZE(fpsValues);
                phase += 0.1f * fpsOffset;
                fpsRefresh += 1.0f / 30.0f;
            }
            float fpsAverage = 0.0f;
            for (int n = 0; n < IM_ARRAYSIZE(fpsValues); n++) fpsAverage += fpsValues[n];
            fpsAverage /= (float)IM_ARRAYSIZE(fpsValues);
            char fpsOverlay[32];
            sprintf(fpsOverlay, "avg %.2f", fpsAverage);
            ImGui::PlotLines("fps", fpsValues, IM_ARRAYSIZE(fpsValues), fpsOffset, fpsOverlay, 0.0f, 120.0f, { 0.f, 64.0f });

            // milliseconds per frame
            static float mspfValues[120] = {};
            static int mspfOffset = 0;
            static double valueRefresh = 0.0;

            if (valueRefresh == 0.0) valueRefresh = ImGui::GetTime();
            while (valueRefresh < ImGui::GetTime()) {
                static float phase = 0.1f;
                mspfValues[mspfOffset] = Application::GetStatistics().msPF;
                mspfOffset = (mspfOffset + 1) % IM_ARRAYSIZE(mspfValues);
                phase += 0.1f * mspfOffset;
                valueRefresh += 1.0f / 60.0f;
            }

            float average = 0.0f;
            for (int n = 0; n < IM_ARRAYSIZE(mspfValues); n++) average += mspfValues[n];
            average /= (float)IM_ARRAYSIZE(mspfValues);
            char overlay[32];
            sprintf(overlay, "avg %.3f", average);
            ImGui::PlotHistogram("mspf", mspfValues, IM_ARRAYSIZE(mspfValues), mspfOffset, overlay, 0.0f, 33.4f, { 0.f, 64.0f });
            if (Application::GetStatistics().fps < 30) {
                ImGui::PopStyleColor(2);
            }
        }

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Renderer")) {
            ImGui::Text("2D Renderer");
            ImGui::Separator();
            UI::Property("DrawCalls", "%d", Renderer2D::GetStatistics().DrawCalls);
            UI::Property("Triangles", "%d", Renderer2D::GetStatistics().Triangles);
            UI::Property("Vertices", "%d", Renderer2D::GetStatistics().GetTotalVertexCount());
            UI::Property("Indices", "%d", Renderer2D::GetStatistics().GetTotalIndexCount());
            ImGui::Separator();
            UI::Property("... thereof Lines", "%d", Renderer2D::GetStatistics().LineCount);
            UI::Property("... thereof Quads", "%d", Renderer2D::GetStatistics().QuadCount);
        }
        ImGui::End();

    }

    void Update(Timestamp deltaTime) override {
        // Preparation
        Renderer2D::ResetStatistics();
        if (mViewport.IsEnabled()) {
            mViewport.BindBuffer();
        }

        // Renderer
        RenderCommand::SetClearColor(ClearColor);
        RenderCommand::Clear();

        CurrentScene->Update(deltaTime);
        #ifdef StepI
            // Draw
            Renderer2D::StartScene(CurrentScene->GetCamera());

            static float rotation = 0.0f;
            rotation += RoatationSpeed * deltaTime;

            Renderer2D::DrawLine({ -1.5f, -1.5f, 1.0f }, { -1.5f, 1.5f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
            Renderer2D::DrawLine({ -1.6f, -1.6f, 1.0f }, { -1.6f, 1.6f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
            Renderer2D::DrawLine({ -1.7f, -1.7f, 1.0f }, { -1.7f, 1.7f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
            Renderer2D::DrawLine({ -1.8f, -1.8f, 1.0f }, { -1.8f, 1.8f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
            Renderer2D::DrawLine({ -1.9f, -1.9f, 1.0f }, { -1.9f, 1.9f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

            // Checkerboard Texture
            Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 2.0f, 2.0f }, GridTexture);


            // Low performance Grid
            for (float y = -GridY; y < +GridY; y += GridSteps) {
                for (float x = -GridX; x < +GridX; x += GridSteps) {
                    glm::vec4 color = { (x + 5.0f) / 10.f, 0.2f, (y + 5.0f) / 10.0f, 0.72f };
                    Renderer2D::DrawRotatedQuad({ x, y, -0.05f }, { GridSteps * 0.72f, GridSteps * 0.72f }, glm::radians(rotation * -1.0f), color);
                }
            }

            // High performance Grid

            // Sample Quads
            Renderer2D::DrawQuad({ -0.2f, -0.2f }, { QuadSize, QuadSize }, { 1.0f, 0.0f, 0.0f, 1.0f });
            Renderer2D::DrawQuad({ 0.2f, 0.2f }, { QuadSize, QuadSize }, { 0.0f, 0.0f, 1.0f, 1.0f });
            Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, 1.0f }, { 0.2f, 0.2f }, glm::radians(rotation), { 0.0f, 1.0f, 0.0f, 0.72f });

            Renderer2D::FinishScene();
        #endif

        if (mViewport.IsEnabled()) { mViewport.UnbindBuffer(); }
    }

    void KeyboardEvent(KeyboardEventData data) override {
        static bool playing = false;
        if (data.State == KeyState::Release) {
            if (data.Key == KeyCode::F2) {
                APP_DEBUGBREAK();
            }
            if (data.Key == KeyCode::KeyP) {
                if (playing) {
                    Audio::Player::Pause(BackgroundMusicPlayer);
                } else {
                    Audio::Player::Play(BackgroundMusicPlayer);
                }
                playing = !playing;
            }
        }
    }

    void WindowEvent(WindowEventData data) override {
        if (data.Action == WindowAction::Resize) {
            auto &dimension = Application::GetWindow().GetContexttSize();
            Renderer::Resize(dimension.Width, dimension.Height);
            mViewport.Resize(dimension.Width, dimension.Height);
            CurrentScene->Resize(dimension.Width, dimension.Height);
        }
    }

    static Reference<Scene> GetCurrentScene() { return sCurrentScene; }

private:
    // Panels
    SceneBrowser Browser;
    ViewportPanel mViewport;
    PropertiesPanel SceneProperties;
    TextEditor ScriptEditor;

    // Scene
    Reference<Scene> CurrentScene;
    Entity CameraEntity;
    Entity BackgroundMusic;

    Ultra::Audio::Source BackgroundMusicPlayer = {};

    glm::vec4 ClearColor;
    glm::vec4 GridColor;
    Reference<Texture2D> GridTexture;
    Reference<Shader> GridShader;

    float QuadSize = 0.5f;
    float GridSteps = 0.27f;
    float GridX = 5.0f;
    float GridY = 5.0f;
    float RoatationSpeed = 5.0f;
};

}

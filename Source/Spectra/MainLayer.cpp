#pragma once

#include "Ultra.h"

#include "Menu/Menues.h"
#include "View/Views.h"

#include "Engine/Audio/Audio.h"
#include "Scripts/CameraController.h"

namespace Ultra {

class MainLayer: public Layer {
public:
    MainLayer(): Layer("Core"),
        ClearColor(1.0f, 1.01f, 1.0f, 0.0f),
        GridColor(0.8f, 0.8f, 0.2f, 0.72f),
        SceneCamera(1.33f) {
    }

    void Create() override {
        SetStyle();
        Ultra::Audio::Player::Initialize();
        BackgroundMusic = Ultra::Audio::Source::LoadFromFile("Assets/Audio/Intergalactic Odyssey.ogg");
        BackgroundMusic.SetRepeat(true);

        FramebufferProperties fbProperties;
        auto [width, height] = Application::GetWindow().GetDisplaySize();
        fbProperties.Width = width;
        fbProperties.Height = height;
        ViewportBuffer = Framebuffer::Create(fbProperties);

        Viewport = Texture2D::Create("./Assets/Textures/Checkerboard.png");
        GridTexture = Texture2D::Create("./Assets/Textures/Checkerboard.png");

        GridShader = Shader::Create("Assets/Shaders/Grid.glsl");
        GridShader->SetFloat("uResolution", 1000.0f);
        GridShader->SetFloat("uScaling", 24.0f);

        CurrentScene = CreateReference<Scene>();

        SquareEntity = CurrentScene->CreateEntity("My square");
        SquareEntity.AddComponent<Component::Sprite>(glm::vec4{ 1.0f, 1.0f, 0.0f, 1.0f });

        CameraEntity = CurrentScene->CreateEntity("Camera");
        CameraEntity.AddComponent<Component::Camera>(); //glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f)

        CameraEntity2 = CurrentScene->CreateEntity("Camera2");
        CameraEntity2.AddComponent<Component::Camera>(); //glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f)

        CameraEntity.AddComponent<Component::NativeScript>().Bind<CameraSystem>();
    }

    void GuiUpdate() override {
        static bool alwaysOpen = true;
        UseFrameBuffer = Menu::ViewportRender;

        /**
         * @brief Menus
        */
        if (ImGui::BeginMenuBar()) {
            Menu::ShowMenuFile();
            Menu::ShowMenuEdit();
            Menu::ShowMenuView();
            Menu::ShowMenuTools();
            Menu::ShowMenuHelp();
            ImGui::EndMenuBar();
        }



        /**
         * @brief Views
        */
        // Browser
        ImGui::Begin("Browser");
        ImGui::End();

        // Console
        static bool consoleOpen = true;
        if (consoleOpen) View::ShowExampleAppConsole(&consoleOpen);

        // Scene
        static bool viewportOpen = true;
        if (viewportOpen && UseFrameBuffer) {
            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
            uint32_t rendererID = ViewportBuffer->GetColorAttachmentRendererID();
            ViewportActive = View::ShowScene(&viewportOpen, rendererID);
        } else {
            UseFrameBuffer = false;
        }

        // Properties
        ImGui::Begin("Properties");
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Grid")) {
            UI::Property("Quad Size:", QuadSize);
            UI::Property("Grid Steps:", GridSteps, 0.075f, 1.0f);
            UI::Property("Grid X:", GridX, 1.0f, 25.0f);
            UI::Property("Grid Y:", GridY, 1.0f, 25.0f);
            UI::Property("Rotation Speed:", RoatationSpeed, 0.0f, 120.0f);
        }

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Test")) {
            if (SquareEntity) {
                const char *tag = SquareEntity.GetComponent<Component::Tag>();
                ImGui::Text("%s", tag);
                ImGui::Separator();
            }

            ImGui::DragFloat3("Camera Transform", glm::value_ptr(CameraEntity.GetComponent<Component::Transform>().mTransform[3]));

            static bool myCheck = true;
            UI::Property("2nd Camera", myCheck);
            CameraEntity.GetComponent<Component::Camera>().Primary = myCheck;

            {
                auto &sc = CameraEntity2.GetComponent<Component::Camera>().mCamera;
                float os = sc.GetOrthographicSize();
                if (ImGui::DragFloat("2nd Camera Size", &os)) {
                    sc.SetOrthographicSize(os);
                }
            }

            UI::Property("State", "%s", "Test");

            UI::Label("Label: %d | %d | %d", 0, 1, 2);
            UI::LabelX("Label: %d", 0, 2);
        }
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
        if (UseFrameBuffer) ViewportBuffer->Bind();

        // Camera
        if (ViewportActive) SceneCamera.Update(deltaTime);

        // Renderer
        RenderCommand::SetClearColor(ClearColor);
        RenderCommand::Clear();

        // Draw
        if (FramebufferProperties fbp = ViewportBuffer->GetProperties();
            ViewportSize.x > 0.0f && ViewportSize.y > 0.0f &&
            (fbp.Width != ViewportSize.x || fbp.Height != ViewportSize.y)) {

            auto &width = Application::GetWindow().GetContexttSize().Width;
            auto &height = Application::GetWindow().GetContexttSize().Height;

            Renderer::Resize(width, height);
            SceneCamera.Resize(height, height);
            CurrentScene->Resize((uint32_t)ViewportSize.x, (uint32_t)ViewportSize.y);
            ViewportBuffer->Resize((uint32_t)ViewportSize.x, (uint32_t)ViewportSize.y);
        }
        CurrentScene->Update(deltaTime);

        //Renderer2D::StartScene(SceneCamera.GetCamera());

        //static float rotation = 0.0f;
        //rotation += RoatationSpeed * deltaTime;

        //Renderer2D::DrawLine({ -1.5f, -1.5f, 1.0f }, { -1.5f, 1.5f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
        //Renderer2D::DrawLine({ -1.6f, -1.6f, 1.0f }, { -1.6f, 1.6f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
        //Renderer2D::DrawLine({ -1.7f, -1.7f, 1.0f }, { -1.7f, 1.7f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
        //Renderer2D::DrawLine({ -1.8f, -1.8f, 1.0f }, { -1.8f, 1.8f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
        //Renderer2D::DrawLine({ -1.9f, -1.9f, 1.0f }, { -1.9f, 1.9f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

        //// Checkerboard Texture
        //Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 2.0f, 2.0f }, GridTexture);


        //// Low performance Grid
        //for (float y = -GridY; y < +GridY; y += GridSteps) {
        //    for (float x = -GridX; x < +GridX; x += GridSteps) {
        //        glm::vec4 color = { (x + 5.0f) / 10.f, 0.2f, (y + 5.0f) / 10.0f, 0.72f };
        //        Renderer2D::DrawRotatedQuad({ x, y, -0.05f }, { GridSteps * 0.72f, GridSteps * 0.72f }, glm::radians(rotation * -1.0f), color);
        //    }
        //}

        //// High performance Grid

        //// Sample Quads
        //Renderer2D::DrawQuad({ -0.2f, -0.2f }, { QuadSize, QuadSize }, { 1.0f, 0.0f, 0.0f, 1.0f });
        //Renderer2D::DrawQuad({ 0.2f, 0.2f }, { QuadSize, QuadSize }, { 0.0f, 0.0f, 1.0f, 1.0f });
        //Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, 1.0f }, { 0.2f, 0.2f }, glm::radians(rotation), { 0.0f, 1.0f, 0.0f, 0.72f });

        //Renderer2D::FinishScene();
        if(UseFrameBuffer) ViewportBuffer->Unbind();
    }

    void KeyboardEvent(KeyboardEventData data) override {
        static bool playing = false;
        if (data.State == KeyState::Release) {
            if (data.Key == KeyCode::KeyP) {
                if (playing) {
                    Ultra::Audio::Player::Pause(BackgroundMusic);
                } else {
                    Ultra::Audio::Player::Play(BackgroundMusic);
                }
                playing = !playing;
            }
        }
    }

    void MouseEvent(MouseEventData data) override {
        if (data.Action == MouseAction::Wheel) {
            SceneCamera.MouseEvent(data);
        }
    }

    void WindowEvent(WindowEventData data) override {
        if (data.Action == WindowAction::Resize) {
            Renderer::Resize(data.Width, data.Height);
            SceneCamera.Resize(data.Width, data.Height);
            CurrentScene->Resize(Application::GetWindow().GetContexttSize().Width, Application::GetWindow().GetContexttSize().Height);
        }
    }

private:
    Reference<Texture2D> Viewport;
    Reference<Framebuffer> ViewportBuffer;

    CameraController SceneCamera;
    //OrthographicCameraController SceneCamera;

    Reference<Scene> CurrentScene;
    Entity CameraEntity;
    Entity CameraEntity2;
    Entity SquareEntity;

    glm::vec4 ClearColor;
    glm::vec4 GridColor;

    Reference<Texture2D> GridTexture;

    Reference<Shader> GridShader;

    bool UseFrameBuffer = true;
    bool ViewportActive = true;

    float QuadSize = 0.5f;

    float GridSteps = 0.27f;
    float GridX = 5.0f;
    float GridY = 5.0f;

    float RoatationSpeed = 5.0f;

    Ultra::Audio::Source BackgroundMusic = {};

    glm::vec2 ViewportSize = { 0.0f, 0.0f };
};

}

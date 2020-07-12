#pragma once

#include "Ultra.h"

#include "Menu/Menues.h"
#include "View/Views.h"

namespace Ultra {

class MainLayer: public Layer {
    Reference<Texture2D> Viewport;
    Reference<Framebuffer> ViewportBuffer;

    OrthographicCameraController SceneCamera;
    glm::vec4 ClearColor;
    glm::vec4 GridColor;

    Reference<Texture2D> GridTexture;

    bool UseFrameBuffer = true;
    bool ViewportActive = true;

    float QuadSize = 0.5f;

    float GridSteps = 0.27f;
    float GridX = 5.0f;
    float GridY = 5.0f;

    float RoatationSpeed = 5.0f;

public:
    MainLayer():
        Layer("Core"),
        ClearColor(1.0f, 1.01f, 1.0f, 0.0f),
        GridColor(0.8f, 0.8f, 0.2f, 0.72f),
        SceneCamera(1.33f, true) {
    }

    void Create() override {
        FramebufferProperties fbProperties;
        auto [width, height] = Application::GetWindow().GetDisplaySize();
        fbProperties.Width = width;
        fbProperties.Height = height;
        ViewportBuffer = Framebuffer::Create(fbProperties);

        Viewport = Texture2D::Create("./Assets/Textures/Checkerboard.png");
        GridTexture = Texture2D::Create("./Assets/Textures/Checkerboard.png");

        //SetDefaultStyle();
        //SetStyle(ColorPalette::BlueGray, ColorPalette::Blue, ColorPalette::Red);
        //SetDefaultStyle();
        //SetRayTeakStyle();
        SetStyle();
    }

    void GuiUpdate() override {
        ImVec4 defaultHighlightColor = ImTextColorHighlight;
        ImVec4 defaultHighlightWarnColor = ImTextColorHighlightWarn;

        UseFrameBuffer = Menu::ViewportRender;

        static bool alwaysOpen = true;
        static bool DockSpace = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

        // Disabling fullscreen would allow the window to be moved to the front of other windows,  which we can't undo at the moment without finer window depth/z control.
        //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,  because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        ImGuiWindowFlags flagsViewport = ImGuiWindowFlags_NoScrollbar;
        if (opt_fullscreen) {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->GetWorkPos());
            ImGui::SetNextWindowSize(viewport->GetWorkSize());
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) window_flags |= ImGuiWindowFlags_NoBackground;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        // DockSpace
        ImGui::Begin("DockSpace", &DockSpace, window_flags);
        ImGui::PopStyleVar();
        if (opt_fullscreen) ImGui::PopStyleVar(2);

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("DockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

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
        View::ShowExampleAppConsole(&alwaysOpen);

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
        if (ImGui::CollapsingHeader("Test")) {
            static bool myCheck = true;
            UI::Property("Boolean", myCheck);
            UI::Property("State", "%s", "Test");

            UI::Label("Label: %d | %d | %d", 0, 1, 2);
            UI::LabelX("Label: %d", 0, 2);
        }
        ImGui::End();

        // Viewport
        if (UseFrameBuffer) {
            View::ShowViewport(&alwaysOpen);
            ImGui::Begin("Viewport", &alwaysOpen, flagsViewport);
            ViewportActive = ImGui::IsWindowFocused() ? true : false;
            uint32_t rendererID = ViewportBuffer->GetColorAttachmentRendererID();
            // ToDo: GetContentRegionMax returns to high y value
            ImVec2 contentRegion = ImGui::GetContentRegionMax();
            contentRegion.y -= 32;
            ImGui::Image((void*)rendererID, contentRegion, ImVec2(0,1), ImVec2(1,0));
            ImGui::End();
        }

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
            UI::Property("... thereof Quads", "%d", Renderer2D::GetStatistics().QuadCount);
        }

        ImGui::End();

        ImGui::End();
    }

    void Update(Timestamp deltaTime) override {

        Renderer2D::ResetStatistics();
        if(UseFrameBuffer) ViewportBuffer->Bind();

        // Preparation
        // Camera
        if (ViewportActive) SceneCamera.Update(deltaTime);
        // Renderer
        RenderCommand::SetClearColor(ClearColor);
        RenderCommand::Clear();

        static float rotation = 0.0f;
        rotation += RoatationSpeed * deltaTime;

        // Draw
        Renderer2D::BeginScene(SceneCamera.GetCamera());

        Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 32.0f, 32.0f }, GridTexture);

        // Low performance Grid
        for (float y = -GridY; y < +GridY; y += GridSteps) {
            for (float x = -GridX; x < +GridX; x += GridSteps) {
                glm::vec4 color = { (x + 5.0f) / 10.f, 0.2f, (y +5.0f) / 10.0f, 0.72f };
                Renderer2D::DrawRotatedQuad({ x, y, -0.05f }, { GridSteps * 0.72f, GridSteps * 0.72f }, glm::radians(rotation), color);
            }
        }

        Renderer2D::DrawQuad({ -0.2f, -0.2f }, { QuadSize, QuadSize }, { 1.0f, 0.0f, 0.0f, 1.0f });
        Renderer2D::DrawQuad({ 0.2f, 0.2f }, { QuadSize, QuadSize }, { 0.0f, 0.0f, 1.0f, 1.0f });
        Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, 1.0f }, { 0.2f, 0.2f }, glm::radians(rotation), { 0.0f, 1.0f, 0.0f, 0.72f });
        Renderer2D::EndScene();
        if(UseFrameBuffer) ViewportBuffer->Unbind();
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
        }
    }
};

}

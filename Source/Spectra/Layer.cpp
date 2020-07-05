#pragma once

#include "Ultra.h"

#include "Style/Styles.h"
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
public:
    MainLayer():
        Layer("Core"),
        ClearColor(0.1f, 0.1f, 0.1f, 1.0f),
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
        static bool alwaysOpen = true;
        static bool DockSpace = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

        static ImVec4 accentTextColor = { 1.0f, 1.0f, 0.0f, 1.0f };

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
        ImGui::End();

        // Viewport
        View::ShowViewport(&alwaysOpen);
        ImGui::Begin("Viewport", &alwaysOpen, flagsViewport);
        ViewportActive = ImGui::IsWindowFocused() ? true : false;
        uint32_t rendererID = ViewportBuffer->GetColorAttachmentRendererID();
        // ToDo: GetContentRegionMax returns to high y value
        ImVec2 contentRegion = ImGui::GetContentRegionMax();
        contentRegion.y -= 32;
        ImGui::Image((void*)rendererID, contentRegion, ImVec2(0,1), ImVec2(1,0));
        ImGui::End();

        // Statistics
        ImGui::Begin("Statistics");
        ImGui::Text("2D Renderer");
        ImGui::Separator();
        ImGui::Text("DrawCalls: ");
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, accentTextColor);
        ImGui::Text("%d", Renderer2D::GetStatistics().DrawCalls);
        ImGui::PopStyleColor();
        ImGui::Text("Triangles: ");
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, accentTextColor);
        ImGui::Text("%d", Renderer2D::GetStatistics().Triangles);
        ImGui::PopStyleColor();
        ImGui::Text("Vertices:  ");
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, accentTextColor);
        ImGui::Text("%d", Renderer2D::GetStatistics().GetTotalVertexCount());
        ImGui::PopStyleColor();
        ImGui::Text("Indices:   ");
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, accentTextColor);
        ImGui::Text("%d", Renderer2D::GetStatistics().GetTotalIndexCount());
        ImGui::PopStyleColor();
        ImGui::Separator();
        ImGui::Text("... thereof Quads: ");
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, accentTextColor);
        ImGui::Text("%d", Renderer2D::GetStatistics().QuadCount);
        ImGui::PopStyleColor();

        ImGui::PlotLines("Sin", [](void *data, int idx) { return idx*0.2f; }, NULL, 100);

        UI::Label("Label: %d", 0);
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
        rotation += 1.0f;

        // Draw
        Renderer2D::BeginScene(SceneCamera.GetCamera());

        Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 32.0f, 32.0f }, GridTexture);

        // Low performance Grid
        float steps = 0.27f;
        for (float y = -5; y < +5; y += steps) {
            for (float x = -5; x < +5; x += steps) {
                glm::vec4 color = { (x + 5.0f) / 10.f, 0.2f, (y +5.0f) / 10.0f, 0.72f };
                Renderer2D::DrawQuad({ x, y, -0.05f }, { steps * 0.72f, steps * 0.72f }, color);
            }
        }

        Renderer2D::DrawQuad({ -0.2f, -0.2f }, { 0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f });
        Renderer2D::DrawQuad({ 0.2f, 0.2f }, { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f });
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

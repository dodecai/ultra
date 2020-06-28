#include <Ultra.h>

namespace Ultra {

class CoreLayer: public Layer {
    Reference<Texture2D> Viewport;
    Reference<Framebuffer> ViewportBuffer;

    OrthographicCameraController SceneCamera;
    glm::vec4 ClearColor;
    glm::vec4 GridColor;

    Reference<Texture2D> GridTexture;

    bool UseFrameBuffer = true;
public:
	CoreLayer():
		Layer("Core"),
        ClearColor(0.1f, 0.1f, 0.1f, 1.0f),
        GridColor(0.8f, 0.8f, 0.2f, 0.72f),
        SceneCamera(1.33f, true) {
	}

	void Create() override {
        FramebufferProperties fbProperties;
        auto [width, height] = Application::Get().GetWindow().GetDisplaySize();
        fbProperties.Width = width;
        fbProperties.Height = height;
        ViewportBuffer = Framebuffer::Create(fbProperties);

        Viewport = Texture2D::Create("./Assets/Textures/Checkerboard.png");
        GridTexture = Texture2D::Create("./Assets/Textures/Checkerboard.png");
    }

	void GuiUpdate() override {
    
        static bool dockspaceShow = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
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

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceShow, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("DockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Exit")) Application::Get().Exit();

                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

                //if (ImGui::MenuItem("Flag: NoSplit",                "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
                //if (ImGui::MenuItem("Flag: NoResize",               "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
                //if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
                //if (ImGui::MenuItem("Flag: PassthruCentralNode",    "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))     dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
                //if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
                //ImGui::Separator();
                //if (ImGui::MenuItem("Close DockSpace", NULL, false, dockspaceShow != NULL))
                //    dockspaceShow = false;
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::Begin("Viewport");
        uint32_t rendererID = ViewportBuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)rendererID, ImGui::GetWindowContentRegionMax(), ImVec2(0,1), ImVec2(1,0));
        ImGui::End();

        ImGui::Begin("Statistics");
        ImGui::Text("2D Renderer");
        ImGui::Text("DrawCalls: %d", Renderer2D::GetStatistics().DrawCalls);
        ImGui::Text("Triangles: %d", Renderer2D::GetStatistics().QuadCount * 2);
        ImGui::Text("Vertices: %d", Renderer2D::GetStatistics().GetTotalVertexCount());
        ImGui::Text("Indices: %d", Renderer2D::GetStatistics().GetTotalIndexCount());
        ImGui::End();
        ImGui::End();
	}

	void Update(Timestamp deltaTime) override {

        Renderer2D::ResetStatistics();
        if(UseFrameBuffer) ViewportBuffer->Bind();

        // Preparation
        // Camera
        SceneCamera.Update(deltaTime);
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

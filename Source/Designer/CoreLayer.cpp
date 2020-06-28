#include <Ultra.h>

#include "Menu/Menues.h"
#include "Style/Styles.h"
#include "View/Views.h"

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

        SetDefaultStyle();
    }

	void GuiUpdate() override {
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



        //    ImGuiWindowFlags_NoTitleBar             = 1 << 0,   // Disable title-bar
        //    ImGuiWindowFlags_NoResize               = 1 << 1,   // Disable user resizing with the lower-right grip
        //    ImGuiWindowFlags_NoMove                 = 1 << 2,   // Disable user moving the window
        //    ImGuiWindowFlags_NoScrollbar            = 1 << 3,   // Disable scrollbars (window can still scroll with mouse or programmatically)
        //    ImGuiWindowFlags_NoScrollWithMouse      = 1 << 4,   // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
        //    ImGuiWindowFlags_NoCollapse             = 1 << 5,   // Disable user collapsing window by double-clicking on it. Also referred to as "window menu button" within a docking node.
        //    ImGuiWindowFlags_AlwaysAutoResize       = 1 << 6,   // Resize every window to its content every frame
        //    ImGuiWindowFlags_NoBackground           = 1 << 7,   // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
        //    ImGuiWindowFlags_NoSavedSettings        = 1 << 8,   // Never load/save settings in .ini file
        //    ImGuiWindowFlags_NoMouseInputs          = 1 << 9,   // Disable catching mouse, hovering test with pass through.
        //    ImGuiWindowFlags_MenuBar                = 1 << 10,  // Has a menu-bar
        //    ImGuiWindowFlags_HorizontalScrollbar    = 1 << 11,  // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
        //    ImGuiWindowFlags_NoFocusOnAppearing     = 1 << 12,  // Disable taking focus when transitioning from hidden to visible state
        //    ImGuiWindowFlags_NoBringToFrontOnFocus  = 1 << 13,  // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
        //    ImGuiWindowFlags_AlwaysVerticalScrollbar= 1 << 14,  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
        //    ImGuiWindowFlags_AlwaysHorizontalScrollbar=1<< 15,  // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
        //    ImGuiWindowFlags_AlwaysUseWindowPadding = 1 << 16,  // Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)
        //    ImGuiWindowFlags_NoNavInputs            = 1 << 18,  // No gamepad/keyboard navigation within the window
        //    ImGuiWindowFlags_NoNavFocus             = 1 << 19,  // No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)
        //    ImGuiWindowFlags_UnsavedDocument        = 1 << 20,  // Append '*' to title without affecting the ID, as a convenience to avoid using the ### operator. When used in a tab/docking context, tab is selected on closure and closure is deferred by one frame to allow code to cancel the closure (with a confirmation popup, etc.) without flicker.
        //    ImGuiWindowFlags_NoDocking              = 1 << 21,  // Disable docking of this window

        //    ImGuiWindowFlags_NoNav                  = ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,
        //    ImGuiWindowFlags_NoDecoration           = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse,
        //    ImGuiWindowFlags_NoInputs               = ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,

        //    // [Internal]
        //    ImGuiWindowFlags_NavFlattened           = 1 << 23,  // [BETA] Allow gamepad/keyboard navigation to cross over parent border to this child (only use on child that have no scrolling!)
        //    ImGuiWindowFlags_ChildWindow            = 1 << 24,  // Don't use! For internal use by BeginChild()
        //    ImGuiWindowFlags_Tooltip                = 1 << 25,  // Don't use! For internal use by BeginTooltip()
        //    ImGuiWindowFlags_Popup                  = 1 << 26,  // Don't use! For internal use by BeginPopup()
        //    ImGuiWindowFlags_Modal                  = 1 << 27,  // Don't use! For internal use by BeginPopupModal()
        //    ImGuiWindowFlags_ChildMenu              = 1 << 28,  // Don't use! For internal use by BeginMenu()
        //    ImGuiWindowFlags_DockNodeHost           = 1 << 29   // Don't use! For internal use by Begin()/NewFrame()


        // DockSpace
        ImGui::Begin("DockSpace", &DockSpace, window_flags);
        ImGui::PopStyleVar();
        if (opt_fullscreen) ImGui::PopStyleVar(2);

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("DockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        // Menus
        if (ImGui::BeginMenuBar()) {
            Menu::ShowMenuFile();
            Menu::ShowMenuEdit();
            Menu::ShowMenuView();
            Menu::ShowMenuTools();
            Menu::ShowMenuHelp();
            ImGui::EndMenuBar();
        }

        // Views
        View::ShowExampleAppConsole(&alwaysOpen);
        View::ShowViewport(&alwaysOpen);
        ImGui::Begin("Viewport", &alwaysOpen, flagsViewport);
        uint32_t rendererID = ViewportBuffer->GetColorAttachmentRendererID();
        // ToDo: GetContentRegionMax returns to high y value
        ImVec2 contentRegion = ImGui::GetContentRegionMax();
        contentRegion.y -= 32;
        ImGui::Image((void*)rendererID, contentRegion, ImVec2(0,1), ImVec2(1,0));
        ImGui::End();

        // Statistics
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

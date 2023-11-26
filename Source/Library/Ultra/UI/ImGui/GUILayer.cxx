module;

#pragma warning(push, 0)
    #include <imgui/imgui.h>
    #include <imgui/imgui_internal.h>
#pragma warning(pop)

module Ultra.UI.GUILayer;

import Ultra.Core.Application;
import Ultra.Logger;
import Ultra.UI.GUIBuilder;

import Ultra.Platform.Graphics.VKContext;

#pragma warning(push)
#pragma warning(disable: 4100)

namespace Ultra {

const float FontSize = 16.0f;
static bool ShowDemoWindow = false;

static int ImGui_ImplWin32_CreateVkSurface(ImGuiViewport *viewport, ImU64 instance, const void *allocator, ImU64 *surface) {
    auto context = Application::GetContext().As<VKContext>();
    auto result = (int)!context->CreateSurface(viewport->PlatformHandleRaw, (vk::SurfaceKHR *)surface);
    return result;
}

GuiLayer::GuiLayer(): Layer("GuiLayer") {}
GuiLayer::~GuiLayer() {}

void GuiLayer::Attach() {
	// Decide GL+GLSL versions
    auto &app = Application::Instance();
	//ImGui_ImplWin32_EnableDpiAwareness();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	auto &io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    //io.ConfigDockingWithShift = true;

	// ToDo: Works only like this, the question is why (otherwise ImGui uses an old pointer where the data is gone) ...
	string *dataTarget = new string("./Data/" + Application::GetProperties().Title + ".ini");
	string *logTarget = new string("./Log/" + Application::GetProperties().Title + ".log");
	io.IniFilename = dataTarget->c_str();
	io.LogFilename = logTarget->c_str();
	//io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	//io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    auto size = app.GetWindow().GetContexttSize();
	io.DisplaySize = ImVec2((float)size.Width, (float)size.Height);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

    // Load Fonts
    io.Fonts->AddFontDefault();
    io.Fonts->AddFontFromFileTTF("./Assets/Fonts/Roboto/Roboto-Medium.ttf", FontSize, NULL, io.Fonts->GetGlyphRangesDefault());
    io.Fonts->AddFontFromFileTTF("./Assets/Fonts/Roboto/Roboto-Light.ttf", FontSize, NULL, io.Fonts->GetGlyphRangesDefault());
    io.Fonts->AddFontFromFileTTF("./Assets/Fonts/Roboto/Roboto-Bold.ttf", FontSize, NULL, io.Fonts->GetGlyphRangesDefault());
    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/consola.ttf", FontSize, NULL, io.Fonts->GetGlyphRangesDefault());
    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/segoeui.ttf", FontSize, NULL, io.Fonts->GetGlyphRangesDefault());

    if (Context::API == GraphicsAPI::OpenGL) {
        const char *glsl_version = "#version 450";
        //ImGui_ImplWin32_Init(app.GetWindow().GetNativeWindow(), app.GetContext().GetNativeContext());
        ImGui_ImplWin32_InitForOpenGL(app.GetWindow().GetNativeWindow(), app.GetContext().GetNativeContext());
        ImGui_ImplOpenGL3_Init(glsl_version);
    }
    if (Context::API == GraphicsAPI::Vulkan) {
        // Extend Dear ImGui WinAPI
        ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
        platform_io.Platform_CreateVkSurface = ImGui_ImplWin32_CreateVkSurface;

        auto context = Application::GetContext().As<VKContext>();
        ImGui_ImplWin32_Init(app.GetWindow().GetNativeWindow());

        // Create Descriptor Pool
        // UI Descriptor Pool
        VkDescriptorPool descriptorPool;
        VkDescriptorPoolSize pool_sizes[] = {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
        pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        VkResult err = vkCreateDescriptorPool(context->GetDevice()->Call(), &pool_info, nullptr, &descriptorPool);
        if (err == VK_SUCCESS) { /* ToDo: Check Result */ }

        // UI Command Buffer/Pool, Framebuffers, RenderPass
        ImGui_ImplVulkan_InitInfo vkInfo = {};
        vkInfo.Instance = context->GetInstance()->Call();
        vkInfo.PhysicalDevice = context->GetPhyiscalDevice()->Call();
        vkInfo.Device = context->GetDevice()->Call();
        vkInfo.Queue = context->GetDevice()->GetQueue();
        vkInfo.DescriptorPool = descriptorPool;
        vkInfo.MinImageCount = context->GetSwapChain()->GetImageCount();
        vkInfo.ImageCount = context->GetSwapChain()->GetImageCount();
        ImGui_ImplVulkan_Init(&vkInfo, context->GetSwapChain()->GetRenderPass());

        // Upload Fonts
        {
            ImGui_ImplVulkan_CreateFontsTexture();
            vkDeviceWaitIdle(context->GetDevice()->Call());
        }
     }
}

void GuiLayer::Detach() {
    if (Context::API == GraphicsAPI::OpenGL) ImGui_ImplOpenGL3_Shutdown();
    if (Context::API == GraphicsAPI::Vulkan) ImGui_ImplVulkan_Shutdown();

	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void GuiLayer::GuiUpdate() {
	if (ShowDemoWindow) ImGui::ShowDemoWindow(&ShowDemoWindow);
}

void GuiLayer::Update(Timestamp deltaTime) {
	ImGuiIO &io = ImGui::GetIO();
	io.DeltaTime = static_cast<float>(deltaTime);
	return;
}


void GuiLayer::Prepare() {
	// Start new 'Dear ImGui' frame
    if (Context::API == GraphicsAPI::OpenGL) ImGui_ImplOpenGL3_NewFrame();
    if (Context::API == GraphicsAPI::Vulkan) ImGui_ImplVulkan_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Properties
	ImGuiIO& io = ImGui::GetIO();
	static bool DockSpace = true;
	static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;
	static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;;
    if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode) windowFlags |= ImGuiWindowFlags_NoBackground;
    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	// Viewport
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos, 0);
    ImGui::SetNextWindowSize(viewport->Size, 0);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	//// DockSpace
    ImGui::Begin("DockSpace", &DockSpace, windowFlags);
    ImGuiStyle &style = ImGui::GetStyle();
    // ToDo: Window minimum size while docked
    style.WindowMinSize.x = 256.0f;
	ImGui::PopStyleVar(3);

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGuiID dockspaceId = ImGui::GetID("DockSpace");
		ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
	}
}

void GuiLayer::Finish() {
	// ~DockSpace
	ImGui::End();

	// Properties
    ImGuiIO& io = ImGui::GetIO();
	auto [width, height] = Application::GetWindow().GetContexttSize();
	
	// Rendering
    ImGui::Render();
    io.DisplaySize = ImVec2((float)width, (float)height);
    
    if (Context::API == GraphicsAPI::OpenGL) {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    if (Context::API == GraphicsAPI::Vulkan) {
        auto context = Application::GetContext().As<VKContext>();
        auto buffer = context->GetSwapChain()->GetCurrentDrawCommandBuffer();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), (VkCommandBuffer)buffer);
    }

	// Update and Render additional Platform Windows
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}


void GuiLayer::OnControllerEvent(ControllerEventData &data, const EventListener::EventEmitter &emitter) {}

void GuiLayer::OnKeyboardEvent(KeyboardEventData &data, const EventListener::EventEmitter &emitter) {
	if (ImGui::GetCurrentContext() == NULL) return;

	ImGuiIO &io = ImGui::GetIO();

    data.Handled = io.WantCaptureKeyboard;
    
	io.KeyAlt = data.Modifier.Alt;
	io.KeyCtrl = data.Modifier.Control;
	io.KeyShift = data.Modifier.Shift;
	io.KeySuper = data.Modifier.Super;

	switch (data.Action) {
		case KeyboardAction::Input: {
			io.AddInputCharacterUTF16((uint32_t)data.Key);
			break;
		}
        case KeyboardAction::Null: {
			switch (data.State) {
				case KeyState::Press: {
                    logger << data.Key << '\n';
                    if (data.Key == KeyCode::F1) ShowDemoWindow = !ShowDemoWindow;
                    if (data.Key == KeyCode::Shift) {
                        io.AddKeyEvent(ImGuiKey_LeftShift, true);
                        //io.KeysDown[ImGuiKey_LeftShift] = true;
                        io.KeyShift = true;
                    }
                    break;
				}
				case KeyState::Release: {
                    if (data.Key == KeyCode::Shift) {
                        io.AddKeyEvent(ImGuiKey_LeftShift, false);
                        io.KeyShift = false;
                    }
                    //io.AddKeyEvent((ImGuiKey)data.Key, false);
                    //io.KeysDown[(ImGuiKey)data.Key] = false;
					break;
				}

				default: {
					break;
				}
			}
		}
		default: {
			break;
		}
	}
}

void GuiLayer::OnMouseEvent(MouseEventData &data, const EventListener::EventEmitter &emitter) {
	if (ImGui::GetCurrentContext() == NULL) return;
	ImGuiIO &io = ImGui::GetIO();
    data.Handled = io.WantCaptureMouse;
    

	io.KeyAlt = data.Modifier.Alt;
	io.KeyCtrl = data.Modifier.Control;
	io.KeyShift = data.Modifier.Shift;
	io.KeySuper = data.Modifier.Super;

	switch (data.Action) {
		case MouseAction::Move:	{
            io.AddMousePosEvent(static_cast<float>(data.X), static_cast<float>(data.Y));
            //io.MousePos = ImVec2(static_cast<float>(data.X), static_cast<float>(data.Y));
            break;
        }

        case MouseAction::Wheel: {
            io.AddMouseWheelEvent(data.DeltaWheelX, data.DeltaWheelY);
            //io.MouseWheel += data.DeltaWheelY;
            //io.MouseWheelH += data.DeltaWheelX;
            break;
        }

        default: {
            switch (data.State) {
                case ButtonState::Press: {
					if (data.Button == MouseButton::Left)	io.AddMouseButtonEvent(ImGuiMouseButton_Left, true);
					if (data.Button == MouseButton::Right)	io.AddMouseButtonEvent(ImGuiMouseButton_Right, true);
					if (data.Button == MouseButton::Middle)	io.AddMouseButtonEvent(ImGuiMouseButton_Middle, true);
					if (data.Button == MouseButton::X1)		io.AddMouseButtonEvent(3, true);
					if (data.Button == MouseButton::X2)		io.AddMouseButtonEvent(4, true);
					break;
				}

				case ButtonState::Release: {
					if (data.Button == MouseButton::Left)	io.AddMouseButtonEvent(ImGuiMouseButton_Left, false);
					if (data.Button == MouseButton::Right)	io.AddMouseButtonEvent(ImGuiMouseButton_Right, false);
					if (data.Button == MouseButton::Middle) io.AddMouseButtonEvent(ImGuiMouseButton_Middle, false);
					if (data.Button == MouseButton::X1)		io.AddMouseButtonEvent(3, false);
					if (data.Button == MouseButton::X2)		io.AddMouseButtonEvent(4, false);
					break;
				}

				default: {
					break;
				}
			}
		}
	}
}

void GuiLayer::OnTouchEvent(TouchEventData &data, const EventListener::EventEmitter &emitter) {}
void GuiLayer::OnWindowEvent(WindowEventData &data, const EventListener::EventEmitter &emitter) {}

}

#pragma warning(pop)

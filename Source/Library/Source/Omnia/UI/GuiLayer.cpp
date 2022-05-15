#include "Omnia/Omnia.pch"
#include "GuiLayer.h"

#include "Omnia/Core/Application.h"
#include "Omnia/Log.h"

#include "Platform/GFX/Vulkan/VKContext.h"

#include "Omnia/UI/GuiBuilder.h"

namespace Omnia {

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
	Application &app = Application::Get();
	//ImGui_ImplWin32_EnableDpiAwareness();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    // ToDo: [Vulkan] Windows creation now workes, but the swapchains are flickering
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    io.ConfigDockingWithShift = false;

	// ToDo: Works only as an memory leak, the question is why (otherwise ImGui uses old pointer where the data is gone) ...
	string *dataTarget = new string("./Data/"s + Application::GetProperties().Title + ".ini"s);
	string *logTarget = new string("./Log/"s + Application::GetProperties().Title + ".log"s);
	io.IniFilename = dataTarget->c_str();
	io.LogFilename = logTarget->c_str();
	//io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	//io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
	io.DisplaySize = ImVec2((float)app.GetWindow().GetContexttSize().Width, (float)app.GetWindow().GetContexttSize().Height);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

    // Load Fonts
    io.Fonts->AddFontFromFileTTF("./Assets/Fonts/Roboto/Roboto-Medium.ttf", FontSize, NULL, io.Fonts->GetGlyphRangesDefault());
    io.Fonts->AddFontFromFileTTF("./Assets/Fonts/Roboto/Roboto-Light.ttf", FontSize, NULL, io.Fonts->GetGlyphRangesDefault());
    io.Fonts->AddFontFromFileTTF("./Assets/Fonts/Roboto/Roboto-Bold.ttf", FontSize, NULL, io.Fonts->GetGlyphRangesDefault());
    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/consola.ttf", FontSize, NULL, io.Fonts->GetGlyphRangesDefault());
    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/segoeui.ttf", FontSize, NULL, io.Fonts->GetGlyphRangesDefault());
    io.Fonts->AddFontDefault();

    if (Context::API == GraphicsAPI::OpenGL) {
        const char *glsl_version = "#version 450";
        ImGui_ImplWin32_Init(app.GetWindow().GetNativeWindow(), app.GetContext().GetNativeContext());
        ImGui_ImplOpenGL3_Init(glsl_version);
    }
    if (Context::API == GraphicsAPI::Vulkan) {
        // Extend Dear ImGui WinAPI
        ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
        platform_io.Platform_CreateVkSurface = ImGui_ImplWin32_CreateVkSurface;

        Application &app = Application::Get();
        auto context = Application::GetContext().As<VKContext>();
        ImGui_ImplWin32_Init(app.GetWindow().GetNativeWindow(), nullptr);

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
            vk::CommandBuffer commandBuffer =  context->GetDevice()->GetCommandBuffer(true);
            ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
            context->GetDevice()->FlushCommandBuffer(commandBuffer);

            vkDeviceWaitIdle(context->GetDevice()->Call());
            ImGui_ImplVulkan_DestroyFontUploadObjects();
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
	io.DeltaTime = deltaTime;
	return;
}


void GuiLayer::Prepare() {
	// Start new 'Dear ImGui' frame
    if (Context::API == GraphicsAPI::OpenGL) ImGui_ImplOpenGL3_NewFrame();
    if (Context::API == GraphicsAPI::Vulkan) ImGui_ImplVulkan_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
    ImGuizmo::BeginFrame();

	// Properties
	ImGuiIO& io = ImGui::GetIO();
	static bool DockSpace = true;
	static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;
	static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;;

	// Viewport
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->GetWorkPos());
	ImGui::SetNextWindowSize(viewport->GetWorkSize());
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode) windowFlags |= ImGuiWindowFlags_NoBackground;

	// DockSpace
	ImGui::Begin("DockSpace", &DockSpace, windowFlags);
    ImGuiStyle &style = ImGui::GetStyle();
    // ToDo: Window minimum size while docked
    //style.WindowMinSize.x = 256.0f;
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


void GuiLayer::Event(void *event) {}
void GuiLayer::ControllerEvent(ControllerEventData data) {}

void GuiLayer::KeyboardEvent(KeyboardEventData data) {
	if (ImGui::GetCurrentContext() == NULL) return;

	ImGuiIO &io = ImGui::GetIO();

	io.KeyAlt = data.Modifier.Alt;
	io.KeyCtrl = data.Modifier.Control;
	io.KeyShift = data.Modifier.Shift;
	io.KeySuper = data.Modifier.Super;

	//case KeyCode::F1: {
	//	//Menu::show_demo_window = !Menu::show_demo_window;
	//	break;
	//}
	switch (data.Action) {
		case KeyboardAction::Input: {
			io.AddInputCharacterUTF16((uint32_t)data.Key);
			break;
		}
		case KeyboardAction::Default: {
			switch (data.State) {
				case KeyState::Press: {
					io.KeysDown[(uint32_t)data.Key] = true;

					if (data.Key == KeyCode::F1) ShowDemoWindow = true;

					break;
				}
				case KeyState::Release: {
					io.KeysDown[(uint32_t)data.Key] = false;
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

void GuiLayer::MouseEvent(MouseEventData data) {
	if (ImGui::GetCurrentContext() == NULL) return;
	ImGuiIO &io = ImGui::GetIO();

	io.KeyAlt = data.Modifier.Alt;
	io.KeyCtrl = data.Modifier.Control;
	io.KeyShift = data.Modifier.Shift;
	io.KeySuper = data.Modifier.Super;

	switch (data.Action) {
		case MouseAction::Move:	{
			io.MousePos = ImVec2(static_cast<float>(data.X), static_cast<float>(data.Y));
		}

		case MouseAction::Wheel: {
			io.MouseWheel += data.DeltaWheelY;
			io.MouseWheelH += data.DeltaWheelX;
			break;
		}

		default: {
			switch (data.State) {
				case ButtonState::Press: {
					if (data.Button == MouseButton::Left)	io.MouseDown[0] = true;
					if (data.Button == MouseButton::Right)	io.MouseDown[1] = true;
					if (data.Button == MouseButton::Middle)	io.MouseDown[2] = true;
					if (data.Button == MouseButton::X1)		io.MouseDown[3] = true;
					if (data.Button == MouseButton::X2)		io.MouseDown[4] = true;
					break;
				}

				case ButtonState::Release: {
					if (data.Button == MouseButton::Left)	io.MouseDown[0] = false;
					if (data.Button == MouseButton::Right)	io.MouseDown[1] = false;
					if (data.Button == MouseButton::Middle) io.MouseDown[2] = false;
					if (data.Button == MouseButton::X1)		io.MouseDown[3] = false;
					if (data.Button == MouseButton::X2)		io.MouseDown[4] = false;
					break;
				}

				default: {
					break;
				}
			}
		}
	}
}

void GuiLayer::TouchEvent(TouchEventData data) {}
void GuiLayer::WindowEvent(WindowEventData data) {}

}


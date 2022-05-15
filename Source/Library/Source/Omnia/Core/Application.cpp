#include "Application.h"

#include "Omnia/Omnia.pch"
#include "Omnia/Log.h"

#include "Omnia/Debug/Instrumentor.h"

namespace Omnia {

Application *Application::pAppInstance = nullptr;

Application::Application(const ApplicationProperties &properties):
    Reloaded(false),
    Running(true),
    Paused(false) {
    // Preparation
    applog << "Application started..."s << "\n";
    applog << "... on: " << apptime.GetIsoDate() << "\n";
    applog << "... at: " << apptime.GetIsoTime() << "\n";
    pAppInstance = this;
    mProperties = properties;

    // Initialization
    applog << Log::Caption << "Initialization" << "\n";

    // Load Configuration
    mConfig = CreateReference<Config>();

    // Load Window, Context and Events
    mWindow = Window::Create(WindowProperties(properties.Title, properties.Width, properties.Height));
    pListener = EventListener::Create();
    mDialog = Dialog::Create();
    AppLogDebug("[Application] ", "Created window '", properties.Title, "' with size '", properties.Width, "x", properties.Height, "'");

    Context::API = properties.GfxApi;
    mContext = Context::Create(mWindow->GetNativeWindow());
    mContext->Attach();
    mContext->Load();
    mContext->SetViewport(mWindow->GetContexttSize().Width, mWindow->GetContexttSize().Height);

    // Load Core Layer
    pCoreLayer = new GuiLayer();
    PushOverlay(pCoreLayer);
}

Application::~Application() {
    applog << "\nApplication finished ..."s << "\n";
    applog << "... on: " << apptime.GetIsoDate() << "\n";
    applog << "... at: " << apptime.GetIsoTime() << "\n";
}

void Application::Run() {
    // Preparation
    APP_PROFILE_BEGIN_SESSION("Application", "AppProfile.json");
    Timer timer = {};
    double delay = {};
    size_t frames = {};
    string statistics;
    string title = mWindow->GetTitle();

    // Subscribe to all events (internal)
    auto oDispatcher = mWindow->EventCallback.Subscribe([&](bool &result, void *event) { pListener->Callback(result, event); });

    auto oAutoDeviceEvent = pListener->DeviceEvent.Subscribe([&](DeviceEventData data) { this->AutoDeviceEvent(data); });
    auto oAutoPowerEvent = pListener->PowerEvent.Subscribe([&](PowerEventData data) { this->AutoPowerEvent(data); });

    auto oAutoControllerEvent = pListener->ControllerEvent.Subscribe([&](ControllerEventData data) { this->AutoControllerEvent(data); });
    auto oAutoKeyboardEvent = pListener->KeyboardEvent.Subscribe([&](KeyboardEventData data) { this->AutoKeyboardEvent(data); });
    auto oAutoMouseEvent = pListener->MouseEvent.Subscribe([&](MouseEventData data) { this->AutoMouseEvent(data); });
    auto oAutoTouchEvent = pListener->TouchEvent.Subscribe([&](TouchEventData data) { this->AutoTouchEvent(data); });
    auto oAutoWindowEvent = pListener->WindowEvent.Subscribe([&](WindowEventData data) { this->AutoWindowEvent(data); });

    auto oAutoContextEvent = pListener->ContextEvent.Subscribe([&](ContextEventData data) { this->AutoContextEvent(data); });

    // Subscribe to all Events (external)
    auto oControllerEvent = pListener->ControllerEvent.Subscribe([&](ControllerEventData data) { this->ControllerEvent(data); });
    auto oKeyboardEvent = pListener->KeyboardEvent.Subscribe([&](KeyboardEventData data) { this->KeyboardEvent(data); });
    auto oMouseEvent = pListener->MouseEvent.Subscribe([&](MouseEventData data) { this->MouseEvent(data); });
    auto oTouchEvent = pListener->TouchEvent.Subscribe([&](TouchEventData data) { this->TouchEvent(data); });
    auto oWindowEvent = pListener->WindowEvent.Subscribe([&](WindowEventData data) { this->WindowEvent(data); });

    // Main Logic
    Create();
    for (Layer *layer : mLayers) layer->Create();
    applog << Log::Caption << "Main Loop"s << "\n";

    while (Running) {
        // Update events and check if application is paused
        pListener->Update();
        if (Paused) continue;
        if (Reloaded) {
            Reloaded = false;
            continue;
        }

        // Calcualte 
        Timestamp deltaTime = timer.GetDeltaTime();
        frames++;
        delay += deltaTime;
        this->statistics.msPF = deltaTime.GetMilliseconds();

        if (delay >= 1.0f) {
            this->statistics.fps = frames;
            float msPF = 1000.0f / (float)frames;

            statistics = title + " [FPS:" + std::to_string(frames) + " | msPF:" + std::to_string(msPF) + "]";
            mWindow->SetTitle(statistics);

            frames = 0;
            delay = 0.0f;
        }

        // Update
        mContext->Attach();
        for (Layer *layer : mLayers) layer->Update(deltaTime);
        Update(deltaTime);
        if (mWindow->GetState(WindowState::Alive)) {
            pCoreLayer->Prepare();
            for (Layer *layer : mLayers) layer->GuiUpdate();
            pCoreLayer->Finish();
        }
        mContext->SwapBuffers();
        mContext->Detach();
    }

    // Termination
    applog << Log::Caption << "Termination" << "\n";
    for (Layer *layer : mLayers) layer->Destroy();
    Destroy();
    APP_PROFILE_END_SESSION();
}

void Application::Exit() {
    Running = false;
}

// Workflow
void Application::Create() {}
void Application::Destroy() {}
void Application::Update(Timestamp deltaTime) {}

// Danger-Zone
void Application::Reload() {
    auto &context = Get().mContext;
    auto &layers = Get().mLayers;
    auto &reloaded = Get().Reloaded;

    reloaded = true;
    Get().mProperties.GfxApi = Context::API;

    context = Context::Create(GetWindow().GetNativeWindow());
    context->Attach();
    context->Load();

    auto &[width, height] = GetWindow().GetContexttSize();
    context->SetViewport(width, height);

    for (auto *layer : layers) layer->Detach();
    for (auto *layer : layers) layer->Attach();
}


// Event System
void Application::ControllerEvent(ControllerEventData &data) {}
void Application::KeyboardEvent(KeyboardEventData &data) {}
void Application::MouseEvent(MouseEventData &data) {}
void Application::TouchEvent(TouchEventData &data) {}
void Application::WindowEvent(WindowEventData &data) {}


// Layer System
void Application::PushLayer(Layer *layer) {
    mLayers.PushLayer(layer);
    layer->Attach();
}
void Application::PushOverlay(Layer *overlay) {
    mLayers.PushOverlay(overlay);
    overlay->Attach();
}


// Event System (internal)
void Application::AutoDeviceEvent(DeviceEventData &data) {
    AppLog("[Application::AutoDeviceEvent]:");
}
void Application::AutoPowerEvent(PowerEventData &data) {
    AppLog("[Application::AutoPowerEvent]:");
}

void Application::AutoControllerEvent(ControllerEventData &data) {
    for (auto layer : mLayers) { layer->ControllerEvent(data); }
}
void Application::AutoKeyboardEvent(KeyboardEventData &data) {
    for (auto layer : mLayers) { layer->KeyboardEvent(data); }

    //// Left for debugging purposes
    //if (data.Action == KeyboardAction::Input) {
    //	applog << data.Source << ": [" <<
    //		"Action:"		<< data.Action		<< " | "	<<
    //		"State:"		<< data.State		<< " | "	<<
    //		"Key:'"			<< data.Character	<< "' | "	<<
    //		"Modifiers:"	<< data.Modifier	<< " | "	<<
    //		"] \n";
    //} else {
    //	applog << data.Source << ": [" <<
    //		"Action:"		<< data.Action		<< " | "	<<
    //		"State:"		<< data.State		<< " | "	<<
    //		"Key:"			<< data.Key			<< " | "	<<
    //		"Modifiers:"	<< data.Modifier	<< " | "	<<
    //		"-] \n";
    //}
}
void Application::AutoMouseEvent(MouseEventData &data) {
    for (auto layer : mLayers) { layer->MouseEvent(data); }

    // Left for debugging purposes
    //applog << data.Source << ": [" << 
    //	"Action:"		<< data.Action		<< " | " <<
    //	"State:"		<< data.State		<< " | " <<
    //	"Button:"		<< data.Button		<< " | " <<
    //	"X/Y:"			<< data.X			<< "/"	<< data.Y			<< " | " <<
    //	"DeltaX/Y:"		<< data.DeltaX		<< "/"	<< data.DeltaY		<< " | " <<
    //	"LastX/Y:"		<< data.LastX		<< "/"	<< data.LastY		<< " | " <<
    //	"DeltaW X/Y:"	<< data.DeltaWheelX	<< "/"	<< data.DeltaWheelY <<
    //	"Modifiers:"	<< data.Modifier	<< " | " <<
    //	"-] \n";
}
void Application::AutoTouchEvent(TouchEventData &data) {
    for (auto layer : mLayers) { layer->TouchEvent(data); }
}
void Application::AutoWindowEvent(WindowEventData &data) {
    for (auto layer : mLayers) { layer->WindowEvent(data); }

    //if (data.Action == WindowAction::Draw) return;
    //if (data.Action == WindowAction::Focus) return;
    //if (data.Action == WindowAction::Activate) return;
    //if (data.Action == WindowAction::Move) return;

    // Left for debugging purposes
    //applog << data.Source << ": [" << 
    //	"Action:"		<< data.Action		<< " | " <<
    //	"A/F/V:"		<< data.Active		<< "/"	<< data.Focus		<< "/"	<< data.Visible		<< " | " <<
    //	"W/H:"			<< data.Width		<< "/"	<< data.Height		<< " | " <<
    //	"X/Y:"			<< data.X			<< "/"	<< data.Y			<< " | " <<
    //	"DeltaX/Y:"		<< data.DeltaX		<< "/"	<< data.DeltaY		<< " | " <<
    //	"LastX/Y:"		<< data.LastX		<< "/"	<< data.LastY		<< " | " <<
    //	"-] \n";

    switch (data.Action) {
        case WindowAction::Destroy: {
            Running = false;
            break;
        }

        case WindowAction::Resize: {
            mContext->SetViewport(mWindow->GetContexttSize().Width, mWindow->GetContexttSize().Height);
            break;
        }

        default: {
            break;
        }
    }
}

void Application::AutoContextEvent(ContextEventData &data) {
    AppLog("[Application::AutoContextEvent]:");
}

}

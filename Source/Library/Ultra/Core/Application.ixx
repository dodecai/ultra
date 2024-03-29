﻿export module Ultra.Core.Application;

import Ultra.Core;
import Ultra.Core.DateTime;
import Ultra.Core.Layer;
//import Ultra.Config;
import Ultra.Logger;
import Ultra.Core.Timer;
import Ultra.Graphics.Context;
import Ultra.System.Event;
import Ultra.UI.Dialog;
import Ultra.UI.GUILayer;
import Ultra.UI.Window;

int main(int, char **);

export namespace Ultra {

// The title, resolution and graphics API can be passed as structure.
struct ApplicationProperties {
    ApplicationProperties(): Title("Ultra"), Resolution("800x600") { CalculateResolution(); }
    ApplicationProperties(bool external): External(external) {}
    ApplicationProperties(string title, string resolution): Title(title), Resolution(resolution) { CalculateResolution(); }
    ApplicationProperties(string title, string resolution, Ultra::LogLevel level): Title(title), Resolution(resolution), LogLevel(level) { CalculateResolution(); }
    ApplicationProperties(string title, string resolution, Ultra::GraphicsAPI api): Title(title), Resolution(resolution), GfxApi(api) { CalculateResolution(); }

    // Properties
    string Title;
    string Resolution;
    LogLevel LogLevel = LogLevel::Trace;
    GraphicsAPI GfxApi = GraphicsAPI::OpenGL;
    uint32_t Width;
    uint32_t Height;
    bool External = false;

private:
    void CalculateResolution() {
        std::string delimiter = "x";
        std::string width = Resolution.substr(0, Resolution.find(delimiter));
        std::string height = Resolution.substr(Resolution.find(delimiter) + 1, Resolution.size());

        bool rwidth = !width.empty() && std::find_if(width.begin(), width.end(), [](unsigned char c) { return !std::isdigit(c); }) == width.end();
        bool rheight = !height.empty() && std::find_if(height.begin(), height.end(), [](unsigned char c) { return !std::isdigit(c); }) == height.end();

        if (!rwidth || !rheight) {
            Resolution = "1024x768";
            width = "1024";
            height = "768";
            return;
        }

        Width = std::stoi(width);
        Height = std::stoi(height);
    }
};

///
/// @brief This is the main class which controls the workflow during runtime.
///
class Application {
    // Friends
    friend int ::main(int, char**);

    // Types
    struct Statistics {
        std::queue<float> fpsData;
        std::queue<float> msPFData;

        double fps = {};
        double msPF = {};
    };

public:
    // Instance
    static Application *pAppInstance;

    // Constructors and Destructor
    Application(const ApplicationProperties &properties = {}):
        mRunning(true) {
        pAppInstance = this;
        mProperties = properties;
        logger.SetLevel(mProperties.LogLevel);
        logger.Attach(CreateScope<ConsoleLogger>());
        logger.Attach(CreateScope<FileLogger>("Test.log"));
        logger.Attach(CreateScope<MemoryLogger>());

        LogCaption("{}", AsciiLogo());
        Log("{} started ...\n  on: '{}'\n  at: '{}'", mProperties.Title, apptime.GetDate(), apptime.GetTime());

        // Initialization
        if (mProperties.External) return;
        LogCaption("Initialization");

        // Load Configuration
        //mConfig = CreateReference<Config>();
        //mConfig->Load("Data/Config.yml");

        // Load Window, Context and Events
        mDialog = Dialog::Create();
        mWindow = Window::Create(WindowProperties(mProperties.Title, mProperties.Width, mProperties.Height));
        mListener = EventListener::Create();
        mWindow->mExternalInputEventListener = [&](auto value) -> bool { return mListener->Callback(value); };
        mListener->Emitter.on<KeyboardEventData>([&](auto &data, const auto &emitter) { OnKeyboardEvent(data, emitter); });
        mListener->Emitter.on<MouseEventData>([&](auto &data, const auto &emitter) { OnMouseEvent(data, emitter); });
        mListener->Emitter.on<WindowEventData>([&](auto &data, const auto &emitter) { OnWindowEvent(data, emitter); });
        LogDebug("Created window '{}' with size '{}x{}'.", mProperties.Title, mProperties.Width, mProperties.Height);

        Context::API = mProperties.GfxApi;
        mContext = Context::Create(mWindow->GetNativeWindow());
        mContext->Attach();
        mContext->Load();
        mContext->SetViewport(mWindow->GetContextSize().Width, mWindow->GetContextSize().Height);
        mContext->SetVSync(true);
        mContext->Clear();

        // Load Core Layer
        pCoreLayer = new GuiLayer();
        PushOverlay(pCoreLayer);
    };
    virtual ~Application() {
        Log("{} finished ...\n  on: '{}'\n  at: '{}'\n", mProperties.Title, apptime.GetDate(), apptime.GetTime());
    };
    static Application &Instance() { return *pAppInstance; }

    // With this method, everything begins.
    void Run() {
        if (mProperties.External) {
            RunExternal();
            return;
        }

        // Runtime Properties
        Timer timer = {};
        double delay = {};
        double frames = {};
        string title(64, ' ');

        // Creation
        Create();
        for (Layer *layer : mLayers) layer->Create();

        // Logic Loop
        LogCaption("Main Loop");
        while (mRunning) {
            // Update events and check if application is paused
            if (mPaused) continue;
            if (mReloaded) {
                mReloaded = false;
                continue;
            }

            // Calculate 
            Timestamp deltaTime = timer.GetDeltaTime();
            frames++;
            delay += deltaTime;
            mStatistics.msPF = deltaTime.GetMilliseconds();
            if (delay >= 1.0f) {
                mStatistics.fps = frames;
                mStatistics.msPF = 1000.0f / frames;

                title = mProperties.Title + std::format(" [fps: {:.0f} | msPF: {:.3f}]", mStatistics.fps, mStatistics.msPF);

                mWindow->SetTitle(title);
                frames = 0;
                delay = 0.0f;
            }

            // Update
            mContext->Attach();
            for (Layer *layer : mLayers) layer->Update(deltaTime);
            Update(deltaTime);
            if (mWindow->GetState(WindowState::Alive)) {
                mListener->Update();
                pCoreLayer->Prepare();
                for (Layer *layer : mLayers) layer->GuiUpdate();
                pCoreLayer->Finish();
            }
            mContext->SwapBuffers();
            mContext->Detach();
        }

        // Termination
        LogCaption("Termination");
        for (Layer *layer : mLayers) layer->Destroy();
        Destroy();
    }
    void RunExternal() {
        // Runtime Properties
        Timer timer = {};
        double delay = {};
        double frames = {};

        // Creation
        Create();

        while (mRunning) {
            // Update events and check if application is paused
            if (mPaused) continue;
            if (mReloaded) {
                mReloaded = false;
                continue;
            }

            // Calculate 
            Timestamp deltaTime = timer.GetDeltaTime();
            frames++;
            delay += deltaTime;
            mStatistics.msPF = deltaTime.GetMilliseconds();
            if (delay >= 1.0f) {
                mStatistics.fps = frames;
                mStatistics.msPF = 1000.0f / frames;

                frames = 0;
                delay = 0.0f;
            }

            // Update
            Update(deltaTime);
        }

        // Termination
        Destroy();
    }

    // Accessors
    static ApplicationProperties &GetProperties() { return Instance().mProperties; }
    //static Config &GetConfig() { return *Instance().mConfig; }
    static Context &GetContext() { return *Instance().mContext; }
    static Dialog &GetDialog() { return *Instance().mDialog; };
    static Statistics GetStatistics() { return Instance().mStatistics; }
    static Window &GetWindow() { return *Instance().mWindow.get(); };

    ///
    /// Methods
    /// 
    // This method executes your initialization code.
    virtual void Create() {}
    // This method executes your termination code.
    virtual void Destroy() {}
    // This method executes your main logic code.
    virtual void Update([[maybe_unused]] Timestamp deltaTime) {}
    // Add an Ascii-Art Logo to logging headers.
    virtual string AsciiLogo() { return mProperties.Title; };

    // With this method, everything ends.
    void Exit() {
        mRunning = false;
    }

    // This method pushes a layer to the application.
    void PushLayer(Layer *layer) {
        mLayers.PushLayer(layer);
    }

    // This method pushes an overlay on top of the application.
    void PushOverlay(Layer *overlay) {
        mLayers.PushOverlay(overlay);
    }

protected:
    // Attention: This method is used reload/switch the graphics context.
    static void Reload() {
        auto &context = Instance().mContext;
        auto &layers = Instance().mLayers;
        auto &reloaded = Instance().mReloaded;

        reloaded = true;
        Instance().mProperties.GfxApi = Context::API;

        context->Detach();
        context = Context::Create(GetWindow().GetNativeWindow());
        context->Attach();
        context->Load();

        auto &[width, height] = GetWindow().GetContextSize();
        context->SetViewport(width, height);

        for (auto *layer : layers) layer->Detach();
        for (auto *layer : layers) layer->Attach();
    }

    // Events
    virtual void OnControllerEvent(ControllerEventData &data, const EventListener::EventEmitter &emitter) {
        for (auto layer : mLayers) {
            if (data.Handled) break;
            layer->OnControllerEvent(data, emitter);
        }
    }
    
    virtual void OnKeyboardEvent(KeyboardEventData &data, const EventListener::EventEmitter &emitter) {
        for (auto layer : mLayers) {
            if (data.Handled) break;
            layer->OnKeyboardEvent(data, emitter);
        }
    }
    
    virtual void OnMouseEvent(MouseEventData &data, const EventListener::EventEmitter &emitter) {
        for (auto layer : mLayers) {
            if (data.Handled) break;
            layer->OnMouseEvent(data, emitter);
        }
    }
    
    virtual void OnTouchEvent(TouchEventData &data, const EventListener::EventEmitter &emitter) {
        for (auto layer : mLayers) {
            if (data.Handled) break;
            layer->OnTouchEvent(data, emitter);
        }
    }

    virtual void OnWindowEvent(WindowEventData &data, const EventListener::EventEmitter &emitter) {
        for (auto layer : mLayers) {
            if (data.Handled) break;
            layer->OnWindowEvent(data, emitter);
        }
        
        switch (data.Action) {
            case WindowAction::Destroy: {
                Exit();
                break;
            }

            case WindowAction::Resize: {
                mContext->SetViewport(mWindow->GetContextSize().Width, mWindow->GetContextSize().Height);
                break;
            }

            default: {
                break;
            }
        }
    }

    virtual void OnDeviceEvent(DeviceEventData &data, const EventListener::EventEmitter &emitter) {
        for (auto layer : mLayers) {
            if (data.Handled) break;
            layer->OnDeviceEvent(data, emitter);
        }
    }

    virtual void OnPowerEvent(PowerEventData &data, const EventListener::EventEmitter &emitter) {
        for (auto layer : mLayers) {
            if (data.Handled) break;
            layer->OnPowerEvent(data, emitter);
        }
    }

    // This method sets the arguments passed during startup.
    void SetArguments(const Arguments &arguments) {
        mArguments = arguments;
    }

private:
    // Properties
    ApplicationProperties mProperties;
    Arguments mArguments;
    Statistics mStatistics;

    // Objects
    GuiLayer *pCoreLayer;
    LayerStack mLayers;
    //Reference<Config> mConfig;
    Reference<Context> mContext;
    Reference<Dialog> mDialog;
    Scope<EventListener> mListener;
    Scope<Window> mWindow;

    // States
    bool mPaused = false;
    bool mReloaded = false;
    bool mRunning = false;
};

// Application Instance
Application *Application::pAppInstance = nullptr;

// Symbol for Application Entry-Point
Application *CreateApplication();

}

// Module
export module Ultra.Core.Application;

import Ultra.Config;
import Ultra.Core;
import Ultra.Core.Layer;
import Ultra.Core.LayerStack;
import Ultra.Logger;

import Ultra.GFX.Context;
import Ultra.System.Event;
import Ultra.UI.Dialog;
import Ultra.UI.GUILayer;
import Ultra.UI.Window;

import Ultra.Utility.DateTime;
import Ultra.Utility.Timer;

int main(int, char **);

export namespace Ultra {

// List of passed command line arguments
struct Arguments {
    Arguments() = default;
    Arguments(const vector<string> &arguments): mArgumentList(arguments) {}

private:
    vector<string> mArgumentList = {};
};

// The title, resolution and graphics API can be passed as structure.
struct ApplicationProperties {
    ApplicationProperties(): Title("Ultra"), Resolution("800x600") { CalculateResolution(); }
    ApplicationProperties(string title, string resolution): Title(title), Resolution(resolution) { CalculateResolution(); }
    ApplicationProperties(string title, string resolution, Ultra::LogLevel level): Title(title), Resolution(resolution), LogLevel(level) { CalculateResolution(); }

    // Properties
    string Title;
    string Resolution;
    uint32_t Width;
    uint32_t Height;

    Ultra::LogLevel LogLevel = Ultra::LogLevel::Trace;
    GraphicsAPI GfxApi = GraphicsAPI::OpenGL;

private:
    void CalculateResolution() {
        std::string delimiter = "x";
        std::string width = Resolution.substr(0, Resolution.find(delimiter));
        std::string height = Resolution.substr(Resolution.find(delimiter) + 1, Resolution.size());

        bool rwidth = !width.empty() && std::find_if(width.begin(), width.end(), [](unsigned char c) { return !std::isdigit(c); }) == width.end();
        bool rheight = !height.empty() && std::find_if(height.begin(), height.end(), [](unsigned char c) { return !std::isdigit(c); }) == height.end();

        if (!rwidth || !rheight) {
            Resolution = "1024x768";
            Width = 1024;
            Height = 768;
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

public:
    // Instance
    static Application *pAppInstance;

    // Constructors and Destructor
    Application(const ApplicationProperties &properties = {}):
        mRunning(true) {
        pAppInstance = this;
        mProperties = properties;
        logger.SetLevel(mProperties.LogLevel);
    };
    virtual ~Application() = default;
    static Application &Instance() { return *pAppInstance; }

    // With this method, everything begins.
    void Run() {
        // Initialization
        logger << LogLevel::Caption << AsciiLogo() << "\n";
        // ToDo: mProperties.Title not defined in Release Mode...
        //logger << mProperties.Title << " started ...\n" <<
        //    "  on : '" << apptime.GetDate() << "'\n" <<
        //    "  at : '" << apptime.GetTime() << "'\n";
        logger << LogLevel::Caption << "Initialization" << "\n";

        // Load Configuration
        //mConfig = CreateReference<Config>();

        // Load Window, Context and Events
        // ToDo: Cannot be called outside this library, which doesn't make sense yet at all...
        mWindow = Window::Create(WindowProperties(mProperties.Title, mProperties.Width, mProperties.Height));
        mListener = EventListener::Create();
        mWindow->mExternalInputEventListener = [&](auto value) -> bool { return mListener->Callback(value); };
        mListener->Emitter.on<KeyboardEventData>([&]( auto &data, const auto &emitter) { OnKeyboardEvent(data, emitter); });
        mListener->Emitter.on<MouseEventData>([&]( auto &data, const auto &emitter) { OnMouseEvent(data, emitter); });
        mListener->Emitter.on<WindowEventData>([&]( auto &data, const auto &emitter) { OnWindowEvent(data, emitter); });

        LogDebug("[Application] ", "Created window '", mProperties.Title, "' with size '", mProperties.Width, "x", mProperties.Height, "'.");

        Context::API = mProperties.GfxApi;
        mContext = Context::Create(mWindow->GetNativeWindow());
        mContext->Attach();
        mContext->Load();
        mContext->SetViewport(mWindow->GetContexttSize().Width, mWindow->GetContexttSize().Height);
        mContext->Clear();

        // Load Core Layer
        pCoreLayer = new GuiLayer();
        PushOverlay(pCoreLayer);

        // Runtime Properties
        Timer timer = {};
        double delay = {};
        double frames = {};

        // Creation
        Create();
        for (Layer *layer : mLayers) layer->Create();

        // Logic Loop
        logger << LogLevel::Caption << "Main Loop" << "\n";
        while (mRunning) {
            // Update events and check if application is paused
            if (mPaused) continue;
            if (mReloaded) {
                mReloaded = false;
                continue;
            }

            // Calcualte 
            Timestamp deltaTime = timer.GetDeltaTime();
            frames++;
            delay += deltaTime;
            if (delay >= 1.0f) {
                float msPF = 1000.0f / (float)frames;

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
        logger << LogLevel::Caption << "Termination" << "\n";
        for (Layer *layer : mLayers) layer->Destroy();
        Destroy();
        // ToDo: mProperties.Title not defined in Release Mode...
        //logger << mProperties.Title << " finished ...\n" <<
        //    "  on : '" << apptime.GetDate() << "'\n" <<
        //    "  at : '" << apptime.GetTime() << "'\n";
    }

    // Accessors
    virtual string AsciiLogo() { return mProperties.Title; };
    static ApplicationProperties &GetProperties() { return Instance().mProperties; }
    static Context &GetContext() { return *Instance().mContext; }
    static Dialog &GetDialog() { return *Instance().mDialog; };
    static Window &GetWindow() { return *Instance().mWindow; };

    ///
    /// Methods
    /// 
    // This method executes your initialization code.
    virtual void Create() {}
    // This method executes your termination code.
    virtual void Destroy() {}
    // This method executes your main logic code.
    virtual void Update(Timestamp deltatime) {}

    // With this method, everything ends.
    void Exit() {
        mRunning = false;
    }

    // This method pushes a layer to the application.
    void PushLayer(Layer *layer) {
        mLayers.PushLayer(layer);
        //layer->Attach();
    }

    // This method pushes an overlay on top of the application.
    void PushOverlay(Layer *overlay) {
        mLayers.PushOverlay(overlay);
        //overlay->Attach();
    }

protected:
    // Attention: This method is used reload/switch the graphics context.
    static void Reload() {}

    // Events
    virtual void OnControllerEvent(ControllerEventData &data, const EventListener::EventEmitter &emitter) {
        for (Layer *layer : mLayers) {
            if (data.Handled) break;
            layer->OnControllerEvent(data, emitter);
        }
    }
    
    virtual void OnKeyboardEvent(KeyboardEventData &data, const EventListener::EventEmitter &emitter) {
        for (Layer *layer : mLayers) {
            if (data.Handled) break;
            layer->OnKeyboardEvent(data, emitter);
        }
    }
    
    virtual void OnMouseEvent(MouseEventData &data, const EventListener::EventEmitter &emitter) {
        for (Layer *layer : mLayers) {
            if (data.Handled) break;
            layer->OnMouseEvent(data, emitter);
        }
    }
    
    virtual void OnTouchEvent(TouchEventData &data, const EventListener::EventEmitter &emitter) {
        for (Layer *layer : mLayers) {
            if (data.Handled) break;
            layer->OnTouchEvent(data, emitter);
        }
    }

    virtual void OnWindowEvent(WindowEventData &data, const EventListener::EventEmitter &emitter) {
        if (data.Action == WindowAction::Destroy) { Exit(); }
        for (Layer *layer : mLayers) {
            if (data.Handled) break;
            layer->OnWindowEvent(data, emitter);
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

    // Objects
    GuiLayer *pCoreLayer;
    LayerStack mLayers;
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

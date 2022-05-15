#pragma once

#include "Omnia/Core.h"
#include "Omnia/Config.h"
#include "Omnia/Log.h"
#include "Omnia/Core/LayerStack.h"
#include "Omnia/GFX/Context.h"
#include "Omnia/UI/Dialog.h"
#include "Omnia/UI/Event.h"
#include "Omnia/UI/GuiLayer.h"
#include "Omnia/UI/Window.h"
#include "Omnia/Utility/Timer.h"

namespace Omnia {

// The title, resolution and graphics API can be passed as structure.
struct ApplicationProperties {
    string Title;
    string Resolution;
    GraphicsAPI GfxApi = GraphicsAPI::OpenGL;

    ApplicationProperties(): Title("Omnia"), Resolution("800x600") {
        CalculateResolution();
    }
    ApplicationProperties(string title, string resolution): Title(title), Resolution(resolution) {
        CalculateResolution();
    }
    ApplicationProperties(string title, string resolution, GraphicsAPI api): Title(title), Resolution(resolution), GfxApi(api) {
        CalculateResolution();
    }

    uint32_t Width;
    uint32_t Height;

private:
    void CalculateResolution() {
        std::string delimiter = "x";
        std::string width = Resolution.substr(0, Resolution.find(delimiter));
        std::string height = Resolution.substr(Resolution.find(delimiter) +1, Resolution.size());

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

// This class contains the main workflow of the Omnia library, it can be used only once (as singleton).
class Application {
    // Types
    struct Statistics {
        std::queue<float> fpsData;
        std::queue<float> msPFData;

        double fps = {};
        double msPF = {};
    };

public:
    // Default
    static Application *pAppInstance;
    Application(const ApplicationProperties &properties = {});
    virtual ~Application();
    static Application &Get() { return *pAppInstance; }

    // Accessors
    static ApplicationProperties &GetProperties() { return Get().mProperties; }
    static Config &GetConfig() { return *Get().mConfig; }
    static Context &GetContext() { return *Get().mContext; }
    static Dialog &GetDialog() { return *Get().mDialog; }
    static Statistics GetStatistics() { return Get().statistics; };
    static Window &GetWindow() { return *Get().mWindow; }

    // With this method, everything begins.
    void Run();

    // These methods offer an easy to use applicaiton workflow.
    /// This method executes your initialization code.
    virtual void Create();
    /// This method executes your termination code.
    virtual void Destroy();
    /// This method executes your main logic code.
    virtual void Update(Timestamp deltaTime);

    // Danger-Zone
    /// Attention: Call this method only if you want to reload/switch the graphics context.
    static void Reload();

    // With this method, everything ends.
    void Exit();

    // These methods offer an easy to use layer system.
    void PushLayer(Layer *layer);
    void PushOverlay(Layer *overlay);

    // These methods offer an easy to use event system.
    /// This method delivers controller events.
    virtual void ControllerEvent(ControllerEventData &data);
    /// This method delivers keyboard events.
    virtual void KeyboardEvent(KeyboardEventData &data);
    /// This method delivers you mouse events.
    virtual void MouseEvent(MouseEventData &data);
    /// This method delivers controller events.
    virtual void TouchEvent(TouchEventData &data);
    /// This method delivers you window events.
    virtual void WindowEvent(WindowEventData &data);

private:
    //These methods are used internally to handle critical events or pass them to the provided layers.
    /// This method dispatches device events.
    void AutoDeviceEvent(DeviceEventData &data);
    /// This method dispatches power events.
    void AutoPowerEvent(PowerEventData &data);

    /// This method dispatches controller events.
    void AutoControllerEvent(ControllerEventData &data);
    /// This method dispatches keyboard events.
    void AutoKeyboardEvent(KeyboardEventData &data);
    /// This method dispatches mouse events.
    void AutoMouseEvent(MouseEventData &data);
    /// This method dispatches controller events.
    void AutoTouchEvent(TouchEventData &data);
    /// This method dispatches window events.
    void AutoWindowEvent(WindowEventData &data);
    
    /// This method dispatches context events.
    void AutoContextEvent(ContextEventData &data);

private:
    // Properties
    ApplicationProperties mProperties;
    Statistics statistics;

    // States
    bool Paused;
    bool Reloaded;
    bool Running;
    
    // Ojbects
    LayerStack mLayers;
    GuiLayer *pCoreLayer;
    Reference<Config> mConfig;
    Reference<Dialog> mDialog;
    Reference<EventListener> pListener;
    Reference<Context> mContext;
    Reference<Window> mWindow;
};

}

export module Ultra.Core.Layer;

import Ultra.Core;
import Ultra.Core.Event.Data;
import Ultra.System.Event;
import Ultra.Utility.Timer;

export namespace Ultra {

class Layer {
public:
    Layer(const string &name = "Layer"): mName(name) {}
    virtual ~Layer() = default;

    virtual void Attach() {}
    virtual void Detach() {}

    virtual void Create() {}
    virtual void Destroy() {}
    virtual void GuiUpdate() {}
    virtual void Update([[maybe_unused]] Timestamp deltaTime) {}

    inline const string &GetName() const { return mName; }

    // EventListener
    [[maybe_unused]] virtual void OnControllerEvent([[maybe_unused]] ControllerEventData &data, [[maybe_unused]] const EventListener::EventEmitter &emitter) {}
    [[maybe_unused]] virtual void OnKeyboardEvent([[maybe_unused]] KeyboardEventData &data, [[maybe_unused]] const EventListener::EventEmitter &emitter) {}
    [[maybe_unused]] virtual void OnMouseEvent([[maybe_unused]] MouseEventData &data, [[maybe_unused]] const EventListener::EventEmitter &emitter) {}
    [[maybe_unused]] virtual void OnTouchEvent([[maybe_unused]] TouchEventData &data, [[maybe_unused]] const EventListener::EventEmitter &emitter) {}
    [[maybe_unused]] virtual void OnWindowEvent([[maybe_unused]] WindowEventData &data, [[maybe_unused]] const EventListener::EventEmitter &emitter) {}

    [[maybe_unused]] virtual void OnDeviceEvent([[maybe_unused]] DeviceEventData &data, [[maybe_unused]] const EventListener::EventEmitter &emitter) {}
    [[maybe_unused]] virtual void OnPowerEvent([[maybe_unused]] PowerEventData &data, [[maybe_unused]] const EventListener::EventEmitter &emitter) {}

protected:
    string mName;
};

}

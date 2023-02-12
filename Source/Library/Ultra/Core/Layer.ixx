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
    virtual void Update(Timestamp deltaTime) {}

    inline const string &GetName() const { return mName; }

    // EventListener
    virtual void OnControllerEvent(ControllerEventData &data, const EventListener::EventEmitter &emitter) {}
    virtual void OnKeyboardEvent(KeyboardEventData &data, const EventListener::EventEmitter &emitter) {}
    virtual void OnMouseEvent(MouseEventData &data, const EventListener::EventEmitter &emitter) {}
    virtual void OnTouchEvent(TouchEventData &data, const EventListener::EventEmitter &emitter) {}
    virtual void OnWindowEvent(WindowEventData &data, const EventListener::EventEmitter &emitter) {}

protected:
    string mName;
};

}

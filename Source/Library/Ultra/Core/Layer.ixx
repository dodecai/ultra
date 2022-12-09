export module Ultra.Core.Layer;

import Ultra.Core;
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

    // ToDo: Rewrite Event System
    //virtual void Event(void *event) {}
    //virtual void ControllerEvent(ControllerEventData data) {}
    //virtual void KeyboardEvent(KeyboardEventData data) {}
    //virtual void MouseEvent(MouseEventData data) {}
    //virtual void TouchEvent(TouchEventData data) {}
    //virtual void WindowEvent(WindowEventData data) 

protected:
    string mName;
};

}

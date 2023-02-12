export module Ultra.System.Event;

import Ultra.Core;
import Ultra.Utility.Emitter;

export import Ultra.Core.Event.Data;

export namespace Ultra {


//// System Events
//Utility::Subject<bool, DeviceEventData &> DeviceEvent;
//Utility::Subject<bool, PowerEventData &> PowerEvent;

//// Applicaiton Events
//Utility::Subject<bool, WindowEventData &> WindowEvent;
//Utility::Subject<bool, ContextEventData &> ContextEvent;

//// User Events
//Utility::Subject<bool, ControllerEventData &> ControllerEvent;
//Utility::Subject<bool, KeyboardEventData &> KeyboardEvent;
//Utility::Subject<bool, MouseEventData &> MouseEvent;
//Utility::Subject<bool, TouchEventData &> TouchEvent;
class EventListener {
public:
    // Default
    EventListener() = default;
    virtual ~EventListener() = default;
    static Scope<EventListener> Create();

    // Methods
    virtual bool Callback(void *event) = 0;
    virtual void Update() = 0;

    // Event Emitter
    struct EventEmitter: public Emitter<EventEmitter> {} Emitter;
};

/// Interfaces
// Event-Data
struct Event {
    // Constructors and Deconstructor
    Event() = delete;
    Event(std::shared_ptr<EventData> &&data): Data(std::move(data)) {};
    virtual ~Event() = default;

    // Conversions
    template<typename T>
    T *As() {
        return reinterpret_cast<T *>(this);
    }

    // Properties
    std::shared_ptr<EventData> Data = nullptr;
    bool Handled = false;
};

}

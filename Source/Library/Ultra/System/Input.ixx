export module Ultra.System.Input;

export import Ultra.Core;
export import Ultra.Core.Event.Data;

export namespace Ultra {

class Input {
    static Scope<Input> Instance;

public:
    virtual ~Input() {}
    static Scope<Input> Create();

    static bool GetKeyState(KeyCode code);
    static bool GetMouseButtonState(MouseButton button);
    static bool GetMouseButtonStateDelta(MouseButton button);
    static std::pair<float, float> GetMousePosition();
    static std::pair<float, float> GetMousePositionDelta();
    static float GetMouseWheelDelta();

protected:
    virtual bool GetKeyStatePlatform(KeyCode code) const = 0;
    virtual bool GetMouseButtonStatePlatform(MouseButton button) const = 0;
    virtual bool GetMouseButtonStateDeltaPlatform(MouseButton button) const = 0;
    virtual std::pair<float, float> GetMousePositionPlatform() const = 0;
    virtual std::pair<float, float> GetMousePositionDeltaPlatform() const = 0;
    virtual float GetMouseWheelDeltaPlatform() const = 0;

public:
    inline static float sMouseWheelDelta {};
};

}

export module Ultra.System.Input;

export import Ultra.Core;
export import Ultra.Core.Event.Data;

export namespace Ultra {

class Input {
    static Scope<Input> Instance;

public:
    static Scope<Input> Create();

    static bool GetKeyState(KeyCode code);
    static bool GetMouseButtonState(MouseButton button);
    static std::pair<float, float> GetMousePosition();

protected:
    virtual bool GetKeyStatePlatform(KeyCode code) const = 0;
    virtual bool GetMouseButtonStatePlatform(MouseButton button) const = 0;
    virtual std::pair<float, float> GetMousePositionPlatform() const = 0;
};

}

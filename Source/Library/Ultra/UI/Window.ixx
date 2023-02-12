export module Ultra.UI.Window;

import Ultra.Core;
import Ultra.UI.Window.Data;

export namespace Ultra {

class Window {
public:
    // Constructors and Deconstructor
    Window() = default;
    virtual ~Window() = default;

    // Methods
    static Scope<Window> Create(const WindowProperties &properties = WindowProperties());
    virtual void Update() = 0;

    // Accessors
    virtual void *GetNativeWindow() = 0;
    virtual const WindowProperties &GetProperties() const = 0;
    virtual const WindowSize GetContexttSize() const = 0;
    virtual const WindowPosition GetDisplayPosition() const = 0;
    virtual const WindowSize GetDisplaySize() const = 0;
    virtual const WindowSize GetScreenSize() const = 0;
    virtual const bool GetState(WindowState state) const = 0;
    virtual const string GetTitle() const = 0;

    // Modifiers
    virtual void SetProperties(const WindowProperties &properties) = 0;
    virtual void SetCursorPosition(const int32_t x, const int32_t y) = 0;
    virtual void SetDisplayPosition(const int32_t x, const int32_t y) = 0;
    virtual void SetDisplaySize(const uint32_t width, const uint32_t height) = 0;
    virtual void SetProgress(const float progress) = 0;
    virtual void SetTitle(const string_view title) = 0;

    // Properties
    std::function<bool(void *)> mExternalInputEventListener = {};
};

}

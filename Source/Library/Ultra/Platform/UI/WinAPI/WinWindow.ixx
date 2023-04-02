module;

// Hack: Workaround for problems with 'Windows.h' in combination with C++ modules (VS2022 > v17.5)!
#undef __nullnullterminated
#define __SPECSTRINGS_STRICT_LEVEL 0

#undef APIENTRY
#define NOMINMAX
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Hack: Workaround for problems with 'Windows.h' in combination with C++ modules (VS2022 > v17.5)!
#undef __nullnullterminated

export module Ultra.Platform.UI.WinAPI.Window;

import Ultra.Core;
import Ultra.UI.Window;

export namespace Ultra {

struct PlatformWindowStyle {
    uint32_t ClassStyle;
    uint32_t WindowStyle;
    uint32_t WindowStyleEx;
};

enum class ClassStyle: uint32_t {
    Application	= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
    Global		= CS_GLOBALCLASS | CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
};

enum class WinWindowStyle: uint32_t {
    // Contains: WS_SYSMENU | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_VISIBLE
    Default		= WS_OVERLAPPEDWINDOW,	
    Aero		= WS_POPUP | WS_THICKFRAME | WS_VISIBLE,
    Borderless	= WS_POPUP | WS_VISIBLE,
    Full		= WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
};

enum class WinWindowStyleX: uint32_t {
    DefaultX	= WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
    FullX		= WS_EX_APPWINDOW,
    Test		= WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE,
};


class WinWindow: public Window {
public:
    // Default
    WinWindow(const WindowProperties &properties);
    virtual ~WinWindow() override;

    // Methods
    //Subject<bool &, void *> EventCallback;
    virtual void Update() override;
    intptr_t Message(void *event);

    // Accessors
    virtual void *GetNativeWindow() override;
    virtual const WindowProperties &GetProperties() const override;
    virtual const WindowSize GetContexttSize() const override;
    virtual const WindowPosition GetDisplayPosition() const override;
    virtual const WindowSize GetDisplaySize() const override;
    virtual const WindowSize GetScreenSize() const override;
    virtual const bool GetState(WindowState state) const override;
    virtual const string GetTitle() const override;


    // Modifiers
    virtual void SetProperties(const WindowProperties &properties) override;
    virtual void SetCursorPosition(const int32_t x, const int32_t y) override;
    virtual void SetDisplayPosition(const int32_t x, const int32_t y) override;
    virtual void SetDisplaySize(const uint32_t width, const uint32_t height) override;
    virtual void SetProgress(const float progress) override;
    virtual void SetTitle(const string_view title) override;

private:
    // Callbacks
    static LRESULT CALLBACK MessageCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    // Methods
    void *LoadIconFile(const string &icon);

private:
    // Properties
    WindowData Data;
    WindowProperties Properties;

    HINSTANCE ApplicationHandle = nullptr;
    HWND WindowHandle = nullptr;
    HICON AppIcon = {};

    HWND ParentWindowHandle = nullptr;
};

}

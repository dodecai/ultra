#include "WinWindow.h"

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "uxtheme.lib")

#include <DwmApi.h>
#include <ShObjIdl.h>
#include <WindowsX.h>
#include <WinUser.h>
#include <windef.h>

typedef enum _WINDOWCOMPOSITIONATTRIB {
	WCA_UNDEFINED = 0,
	WCA_NCRENDERING_ENABLED = 1,
	WCA_NCRENDERING_POLICY = 2,
	WCA_TRANSITIONS_FORCEDISABLED = 3,
	WCA_ALLOW_NCPAINT = 4,
	WCA_CAPTION_BUTTON_BOUNDS = 5,
	WCA_NONCLIENT_RTL_LAYOUT = 6,
	WCA_FORCE_ICONIC_REPRESENTATION = 7,
	WCA_EXTENDED_FRAME_BOUNDS = 8,
	WCA_HAS_ICONIC_BITMAP = 9,
	WCA_THEME_ATTRIBUTES = 10,
	WCA_NCRENDERING_EXILED = 11,
	WCA_NCADORNMENTINFO = 12,
	WCA_EXCLUDED_FROM_LIVEPREVIEW = 13,
	WCA_VIDEO_OVERLAY_ACTIVE = 14,
	WCA_FORCE_ACTIVEWINDOW_APPEARANCE = 15,
	WCA_DISALLOW_PEEK = 16,
	WCA_CLOAK = 17,
	WCA_CLOAKED = 18,
	WCA_ACCENT_POLICY = 19,
	WCA_FREEZE_REPRESENTATION = 20,
	WCA_EVER_UNCLOAKED = 21,
	WCA_VISUAL_OWNER = 22,
	WCA_HOLOGRAPHIC = 23,
	WCA_EXCLUDED_FROM_DDA = 24,
	WCA_PASSIVEUPDATEMODE = 25,
	WCA_LAST = 26
} WINDOWCOMPOSITIONATTRIB;

typedef struct _WINDOWCOMPOSITIONATTRIBDATA {
	WINDOWCOMPOSITIONATTRIB Attrib;
	PVOID pvData;
	SIZE_T cbData;
} WINDOWCOMPOSITIONATTRIBDATA;

typedef enum _ACCENT_STATE {
	ACCENT_DISABLED = 0,
	ACCENT_ENABLE_GRADIENT = 1,
	ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
	ACCENT_ENABLE_BLURBEHIND = 3,
	ACCENT_ENABLE_ACRYLICBLURBEHIND = 4, // RS4 1803
	ACCENT_ENABLE_HOSTBACKDROP = 5, // RS5 1809
	ACCENT_INVALID_STATE = 6
} ACCENT_STATE;

typedef struct _ACCENT_POLICY {
	ACCENT_STATE AccentState;
	DWORD AccentFlags;
	DWORD GradientColor;
	DWORD AnimationId;
} ACCENT_POLICY;

typedef BOOL (WINAPI *pfnGetWindowCompositionAttribute)(HWND, WINDOWCOMPOSITIONATTRIBDATA*);
typedef BOOL (WINAPI *pfnSetWindowCompositionAttribute)(HWND, WINDOWCOMPOSITIONATTRIBDATA*);

namespace Omnia {

// Internal Properties
static HBRUSH ClearColor = (HBRUSH)GetStockObject(NULL_BRUSH);

ITaskbarList3 *TaskbarList;

// Default
WinWindow::WinWindow(const WindowProperties &properties):
	Properties{ properties } {
	// Properties
	PlatformWindowStyle windowStyle = {};

	// Get Application Information
	ApplicationHandle = GetModuleHandle(NULL);
	LPSTR lpCmdLine = GetCommandLine();
	STARTUPINFOA StartupInfo;
	GetStartupInfoA(&StartupInfo);

	// Settings
	windowStyle.ClassStyle = (DWORD)ClassStyle::Application;
	//SetThreadExecutionState(ES_DISPLAY_REQUIRED && ES_SYSTEM_REQUIRED);

	// Load Ressources
	AppIcon = (HICON)LoadIconFile(Properties.Icon);

	// Register Window Class
	WNDCLASSEX classProperties = {
		.cbSize	= sizeof(WNDCLASSEX),			// Structure Size (in bytes)
		.style = windowStyle.ClassStyle,		// Seperate device context for window and redraw on move
		.lpfnWndProc = MessageCallback,			// Message Callback (WndProc)
		.cbClsExtra = 0,						// Extra class data
		.cbWndExtra = WS_EX_TOPMOST,			// Extra window data
		.hInstance = ApplicationHandle,			// Application Intance
		.hIcon = AppIcon,						// Load Icon (Default: LoadIcon(NULL, IDI_APPLICATION);)
		.hCursor = LoadCursor(NULL, IDC_ARROW),	// Load Cursor (Default: IDC_ARROW)
		.hbrBackground = ClearColor,			// Background (Not required for GL -> NULL)
		.lpszMenuName = NULL,					// We Don't Want A Menu
		.lpszClassName = Properties.ID.c_str(),	// Class Name should be unique per window
		.hIconSm = AppIcon,						// Load Icon Symbol (Default: LoadIcon(NULL, IDI_WINLOGO);)
	};
	if (!RegisterClassEx(&classProperties)) {
		AppLogCritical("[Window]: ", "Failed to register the window class!");
		return;
	}

	// Switch to FullScreen Mode if requested
	unsigned int screenWidth = GetSystemMetricsForDpi(SM_CXSCREEN, GetDpiForSystem());
	unsigned int screenHeight = GetSystemMetricsForDpi(SM_CYSCREEN, GetDpiForSystem());
	if (Properties.Style == WindowStyle::FullScreen) {
		// Device Mode
		DEVMODE screenProperties = {
			.dmSize = sizeof(DEVMODE),				// Structure Size
			.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT,
			.dmBitsPerPel = 32,						// Selected Bits Per Pixel
			.dmPelsWidth = screenWidth,		// Selected Screen Width
			.dmPelsHeight = screenHeight,	// Selected Screen Height
		};
		
		// Try to set selected fullscreen mode
		if ((Properties.Size.Width != screenWidth) && (Properties.Size.Height != screenHeight)) {
			// If the switching fails, offer the user an option to switch to windowed mode
			if (ChangeDisplaySettings(&screenProperties, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL) {
				if (MessageBox(NULL, "The requested mode 'FullScreen' isn't supported by\nyour graphics card. Switch to windowed mode Instead?", __FUNCTION__, MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
					Properties.Style = WindowStyle::Default;
				} else {
					AppLogCritical("[Window]: ", "Switching to fullscreen mode failed!");
					return;
				}
			}
		}
	}

	// FullScreen more convinient?
	//Properties.Style = WindowStyle::FullScreen;
	//HMONITOR hmon = MonitorFromWindow(WindowHandle, MONITOR_DEFAULTTONEAREST);
	//MONITORINFO mi = { sizeof(mi) };
	//GetMonitorInfo(hmon, &mi);
	//dimension.left = mi.rcMonitor.left;
	//dimension.top = mi.rcMonitor.top;
	//dimension.right = mi.rcMonitor.right;
	//dimension.bottom = mi.rcMonitor.bottom;


	// Configure Window
	ShowCursor(!Data.Cursor);
	RECT dimension;
	switch (Properties.Style) {
		case WindowStyle::Default: {
			windowStyle.WindowStyle = (DWORD)WinWindowStyle::Default;
			windowStyle.WindowStyleEx = (DWORD)WinWindowStyleX::DefaultX;
			break;
		}
		case WindowStyle::Borderless: {
			windowStyle.WindowStyle = (DWORD)WinWindowStyle::Borderless;
			windowStyle.WindowStyleEx = (DWORD)WinWindowStyleX::DefaultX;
			break;
		}
		case WindowStyle::FullScreen: {
			windowStyle.WindowStyle = (DWORD)WinWindowStyle::Full;
			windowStyle.WindowStyleEx = (DWORD)WinWindowStyleX::FullX;
			break;
		}
	}
	dimension.left = Properties.Position.X;
	dimension.top = Properties.Position.Y;
	dimension.right = (Properties.Style == WindowStyle::FullScreen) ? (long)screenWidth : (long)Properties.Size.Width;
	dimension.bottom = (Properties.Style == WindowStyle::FullScreen) ? (long)screenHeight : (long)Properties.Size.Height;
	AdjustWindowRectEx(&dimension, windowStyle.WindowStyle, FALSE, windowStyle.WindowStyleEx);


	// Create Window
	WindowHandle = CreateWindowEx(
		windowStyle.WindowStyleEx,	// Window Style (extended)
		Properties.ID.c_str(),		// Window ClassName
		Properties.Title.c_str(),	// Window Title
		windowStyle.WindowStyle,	// Window Style

		// Window Position and Size (if not centered, system should decide where to position the window)
		CW_USEDEFAULT, CW_USEDEFAULT, dimension.right - dimension.left, dimension.bottom - dimension.top,

		ParentWindowHandle,			// Parent Window
		NULL,						// Menu
		ApplicationHandle,			// Instance Handle
		this						// Application Data
	);
	if (!WindowHandle) {
		AppLogCritical("[Window]: ", "Failed to create the window!");
		//Destroy();
		return;
	}
	if (!ParentWindowHandle) { ParentWindowHandle = WindowHandle; }

	// Center Window
	if (!(Properties.Style == WindowStyle::FullScreen) && Properties.Position.Centered) {
		unsigned int x = (GetSystemMetrics(SM_CXSCREEN) - dimension.right) / 2;
		unsigned int y = (GetSystemMetrics(SM_CYSCREEN) - dimension.bottom) / 2;
		SetWindowPos(WindowHandle, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}

	// Show window
	if (Data.Visible) {
		ShowWindow(WindowHandle, SW_SHOW);
		SetForegroundWindow(WindowHandle);
		SetFocus(WindowHandle);
	}

	// Transperency
	HMODULE hUser = GetModuleHandle("user32.dll");
	pfnSetWindowCompositionAttribute SetWindowCompositionAttribute = (pfnSetWindowCompositionAttribute)GetProcAddress(hUser, "SetWindowCompositionAttribute");
	if (SetWindowCompositionAttribute) {
		// AGBR
		ACCENT_POLICY accent = {
            ACCENT_ENABLE_HOSTBACKDROP,
            0,
            0xEA000000,
            0
        };
		WINDOWCOMPOSITIONATTRIBDATA data;
		data.Attrib = WCA_ACCENT_POLICY;
		data.pvData = &accent;
		data.cbData = sizeof(accent);
		SetWindowCompositionAttribute(WindowHandle, &data);
	}
	// Simple Transperancy (Requires: WS_EX_LAYERED)
	SetWindowLong(WindowHandle, GWL_EXSTYLE, GetWindowLong(WindowHandle, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(WindowHandle, NULL, 252, LWA_ALPHA);

	// Taskbar Settings
	RegisterWindowMessage("TaskbarButtonCreated");
	HRESULT hrf = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_ITaskbarList3, (LPVOID *)&TaskbarList);

	// Flash on Taskbar
	FlashWindow(WindowHandle, true);
}

WinWindow::~WinWindow() {
	// Switch back to Standard mode if in FullScreen mode
	if (Properties.Style == WindowStyle::FullScreen) {
		ChangeDisplaySettings(NULL, NULL);
		ShowCursor(true);
	}

	// Ensure that the Window Class gets released
	if (ApplicationHandle) {
		if (!UnregisterClass(Properties.ID.c_str(), ApplicationHandle)) {
			applog << Log::Error << "Could not unregister window class.\n";
		}
	}
}


// Events
LRESULT CALLBACK WinWindow::MessageCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	// Properties
	WinWindow *pCurrentWindow = nullptr;

	// Get/Set the current window class pointer as userdata in WinAPI window
	switch (uMsg) {
		case WM_NCCREATE: {
			pCurrentWindow = static_cast<WinWindow*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
			SetLastError(0);
			if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCurrentWindow))) {
				if (GetLastError() != 0) { return FALSE; }
			}
			break;
		}

		default: {
			pCurrentWindow = reinterpret_cast<WinWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

			// Capture Mouse while button is down
			if (uMsg == WM_LBUTTONDOWN) { SetCapture(hWnd); }
			if (uMsg == WM_LBUTTONUP) ReleaseCapture();
			break;
		}
	}
	
	// Dispatch Message or ...
	if (pCurrentWindow) {
		MSG message = {
			.hwnd    = hWnd,
			.message = uMsg,
			.wParam  = wParam,
			.lParam  = lParam,
		};
		return pCurrentWindow->Message(&message);
	}
	// ... return it as unhandeld
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

intptr_t WinWindow::Message(void *event) {
	// Properties
	LRESULT result = 1;
	MSG &msg = *(reinterpret_cast<MSG *>(event));
	HWND &hWnd = msg.hwnd;
	UINT &uMsg = msg.message;
	WPARAM &wParam = msg.wParam;
	LPARAM &lParam = msg.lParam;

	/**
	* @brief	Window Messages
	* @source	https://docs.microsoft.com/en-us/windows/win32/winmsg/window-messages
	*			https://docs.microsoft.com/en-us/windows/win32/winmsg/window-notifications
	*			https://docs.microsoft.com/en-us/windows/win32/inputdev/keyboard-input-notifications
	*			https://docs.microsoft.com/en-us/windows/win32/gdi/painting-and-drawing-messages
	*/
	// Process important window related events internally
	switch (msg.message) {
		// Pre-Check: Does this message belong to the current window?
		case WM_NULL: {
			// Note: Could be usefull in the future.
			break;
		}
		
		// Preparation
		case WM_NCCREATE: {
			// Note: Could be usefull in the future.
			break;
		}
		case WM_NCDESTROY : {
			// Note: Could be usefull in the future.
			break;
		}
		
		// Creation and Destruction
		case WM_CLOSE: {
			// Ensure that the Window gets destroyed and release the handle to it
			if (DestroyWindow(WindowHandle)) {
				WindowHandle = nullptr;
			} else {
				applog << Log::Error << "Could not release handle to window.\n";
			}

			result = 0;
			break;
		}
		case WM_CREATE: {
			Data.Alive = true;

			result = 0;
			break;
		}
		case WM_DESTROY: {
			Data.Alive = false;
			PostQuitMessage(0);

			result = 0;
			break;
		}

		 // Information
		case WM_DPICHANGED: {
			// Note: Could be usefull in the future.
			break;
		}
		case WM_GETMINMAXINFO: {
			if (Properties.Style == WindowStyle::FullScreen) { break; }

			MINMAXINFO *bounds = reinterpret_cast<MINMAXINFO *>(msg.lParam);
			if (this->Properties.MaxSize.Width > 0 && this->Properties.MaxSize.Height > 0) {
				bounds->ptMaxTrackSize.x = this->Properties.MaxSize.Width;
				bounds->ptMaxTrackSize.y = this->Properties.MaxSize.Height;
			}
			if (this->Properties.MinSize.Width > 0 && this->Properties.MinSize.Height > 0) {
				bounds->ptMinTrackSize.x = this->Properties.MinSize.Width;
				bounds->ptMinTrackSize.y = this->Properties.MinSize.Height;
			}

			result = 0;
			break;
		}

		// Drawing
		case WM_PAINT: {
			Data.Decorated = true;

			//PAINTSTRUCT ps;
			//HDC hdc = BeginPaint(msg.hwnd, &ps);
			//FillRect(hdc, &ps.rcPaint, ClearColor);
			//EndPaint(msg.hwnd, &ps);

			break;
		}
		case WM_ERASEBKGND: {
			// Prevent system clearing the window.
			result = 0;
			break;
		}
		case WM_MOVE: {
			// Note: Could be usefull in the future.
			break;
		}
		case WM_SHOWWINDOW: {
			Data.Visible = (bool)msg.wParam;

			result = 0;
			break;
		}
		case WM_SIZE: {
			switch (msg.wParam) {
				case SIZE_MAXIMIZED: {
					Data.Maximized = true;
					break;
				}
				case SIZE_MINIMIZED: {
					Data.Minimized = true;
					break;
				}
				case SIZE_RESTORED: {
					Data.Maximized = false;
					Data.Minimized = false;
				}
			}
			Properties.Size.Width = static_cast<uint32_t>((UINT64)msg.lParam & 0xFFFF);
			Properties.Size.Height = static_cast<uint32_t>((UINT64)msg.lParam >> 16);

			result = 0;
			break;
		}
		case WM_SIZING: {
			//RedrawWindow(msg.hwnd, NULL, NULL, RDW_UPDATENOW | RDW_NOERASE);
			break;
		}

		// State
		case WM_ACTIVATE: {
			Data.Active = (bool)msg.wParam;

			result = 0;
			break;
		}
		case WM_CAPTURECHANGED: {
			result = 0;
			break;
		}
		case WM_KILLFOCUS: {
			Data.Focused = false;

			result = 0;
			break;
		}
		case WM_SETFOCUS: {
			Data.Focused = true;
			
			// ToDo: Reset mouse position
			//POINT position;
			//if (GetCursorPos(&position)) {
			//	//window->prevMouseX = static_cast<unsigned int>(position.x);
			//	//window->prevMouseY = static_cast<unsigned int>(position.y);
			//}

			result = 0;
			break;
		}

		// System
		case WM_SYSCOMMAND: {
			// Disable ALT application menu
			if ((msg.wParam & 0xfff0) == SC_KEYMENU) { result = 0; break; }

			switch (msg.wParam) {
				// FulLScree-Mode: Prevent ScreenSaver or Monitor PowerSaver
				case SC_SCREENSAVE: case SC_MONITORPOWER: {
					if (Properties.Style == WindowStyle::FullScreen) { result = 0; break; }
					break;
				}

				default: {
					break;
				}
			}
		}
		
		// Default: Currently nothing of interest
		default: {
			break;
		}
	}

	// Publish events to an external event system
	if (!EventCallback.Empty()) {
		bool external = false;
		EventCallback.Publish(external, event);
		if (external && result) result = 0;
	}

	if (!result) return result;
	return DefWindowProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
}

void WinWindow::Update() {
	MSG msg = {};
	while (PeekMessage(&msg, WindowHandle, 0, 0, PM_NOREMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}


// Accessors
void *WinWindow::GetNativeWindow() {
	return reinterpret_cast<void *>(WindowHandle);
}

const WindowProperties WinWindow::GetProperties() const {
	return Properties;
}

const WindowSize WinWindow::GetContexttSize() const {
	RECT dimension;
	GetClientRect(WindowHandle, &dimension);

	return WindowSize(dimension.right - dimension.left, dimension.bottom - dimension.top);
}

const WindowPosition WinWindow::GetDisplayPosition() const {
	//WINDOWPLACEMENT position;
	//GetWindowPlacement(WindowHandle, &position);
	//WindowPosition(position.ptMinPosition.x, position.ptMinPosition.y);
	return Properties.Position;
}

const WindowSize WinWindow::GetDisplaySize() const {
	//RECT area;
	//GetWindowRect(WindowHandle, &area);
	//WindowSize(area.right - area.left, area.bottom - area.top);
	return Properties.Size;
}

const WindowSize WinWindow::GetScreenSize() const {
	int32_t width = GetSystemMetrics(SM_CXSCREEN);
	int32_t height = GetSystemMetrics(SM_CYSCREEN);

	return WindowSize(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
}

const bool WinWindow::GetState(WindowState state) const {
	switch (state) {
		case WindowState::Active:		{ return Data.Active		? true : false; }
		case WindowState::Alive:		{ return Data.Alive			? true : false; }
		case WindowState::Cursor:		{ return Data.Cursor		? true : false; }
		case WindowState::Decorated:	{ return Data.Decorated		? true : false; }
		case WindowState::Focused:		{ return Data.Focused		? true : false; }
		case WindowState::FullScreen:	{ return Data.FullScreen	? true : false; }
		case WindowState::Maximized:	{ return Data.Maximized		? true : false; }
		case WindowState::Minimized:	{ return Data.Minimized		? true : false; }
		case WindowState::Visible:		{ return Data.Visible		? true : false; }
		default: {
			AppLogWarning("[WinAPI::Window]: The specified window state isn't supported!");
			return false;
		}
	}
}

const string WinWindow::GetTitle() const {
	//char title[1024];
	//memset(title, 0, sizeof(char) * 1024);
	//GetWindowText(WindowHandle, title, sizeof(char) * 1024);
	return Properties.Title;
}


// Mutators
void WinWindow::SetProperties(const WindowProperties &properties) {
	string id = Properties.ID;
	Properties = properties;
	Properties.ID = id;

	SetDisplayPosition(properties.Position.X, properties.Position.Y);
	SetDisplaySize(properties.Size.Width, properties.Size.Height);
	SetTitle(properties.Title);
}

void WinWindow::SetCursorPosition(const int32_t x, const int32_t y) {
	if (!SetCursorPos(x, y)) {
		AppLogError("[WinAPI::Window]: Error occured while setting cursor position!");
		return;
	}
}

void WinWindow::SetDisplayPosition(const int32_t x, const int32_t y) {
	if (!SetWindowPos(WindowHandle, 0, (int)x, (int)y, 0, 0, SWP_NOSIZE | SWP_NOZORDER)) {
		AppLogError("[WinAPI::Window]: Error occured while setting display position!");
		return;
	}
	Properties.Position.X = x;
	Properties.Position.Y = y;
}

void WinWindow::SetDisplaySize(const uint32_t width, const uint32_t height) {
	WINDOWPLACEMENT position;
	GetWindowPlacement(WindowHandle, &position);

	RECT dimension;
	dimension.left = position.rcNormalPosition.left;
	dimension.top = position.rcNormalPosition.top;
	dimension.right = (long)width;
	dimension.bottom = (long)height;

	AdjustWindowRectEx(&dimension, NULL, FALSE, NULL);
	if (!SetWindowPos(WindowHandle, 0, dimension.left, dimension.top, dimension.right, dimension.bottom, SWP_NOREPOSITION | SWP_NOZORDER)) {
		AppLogError("[WinAPI::Window]: Error occured while setting display size!");
		return;
	}
	Properties.Size.Width = width;
	Properties.Size.Height = height;
}

void WinWindow::SetProgress(const float progress) {
	static uint32_t max = 100;
	uint32_t current = (uint32_t)(progress * (float)max);

	TaskbarList->SetProgressValue(WindowHandle, current, max);
}

void WinWindow::SetTitle(const string_view title) {
	if (!SetWindowText(WindowHandle, title.data())) {
		AppLogError("[WinAPI::Window]: Error occured while setting title!");
		return;
	}
	Properties.Title = title;
}


// Methods
void *WinWindow::LoadIconFile(const string &icon) {
	return LoadImage(
		NULL,				// Handle Instance must be NULL when loading from a files
		icon.c_str(),		// Icon File
		IMAGE_ICON,			// Specifies that the file is an icon
		0,					// Width of the image (we'll specify default later on)
		0,					// Height of the image
		LR_LOADFROMFILE |	// Load a file (as opposed to a resource)
		LR_DEFAULTSIZE |	// Default metrics based on the type (IMAGE_ICON, 32x32)
		LR_SHARED			// Let the system release the handle when it's no longer used
	);
}


}

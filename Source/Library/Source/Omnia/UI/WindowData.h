#pragma once

#include "Omnia/Omnia.pch"
#include "Omnia/Core.h"

#include "Omnia/Utility/Enum.h"

namespace Omnia {

/**
* @brief	Collection of platform independet window related types
*/

struct WindowPosition {
	int32_t X;	// Display position from the left
	int32_t Y;	// Display position form the top

	uint8_t Monitor; // Monitor identifier

	bool Centered;

	// Window centered on main monitor
	WindowPosition():
		X {-1},
		Y {-1},
		Monitor {0},
		Centered {true} {
	};

	// Window centered on specified monitor
	WindowPosition(uint8_t monitor):
		X {-1},
		Y {-1},
		Monitor {monitor},
		Centered {true} {
	};

	// Window with specified position on main monitor as default
	WindowPosition(int32_t x, int32_t y, uint8_t monitor = 0):
		X {x},
		Y {y},
		Monitor {monitor},
		Centered {false} {
	};
};

struct WindowSize {
	uint32_t Width;		// Display width
	uint32_t Height;	// Display height

	// Window with default resolution 640x480
	WindowSize():
		Width {640u},
		Height {480u} {
	};

	// Window with spepcified resolution
	WindowSize(uint32_t width, uint32_t height):
		Width {width},
		Height {height} {
	};
};

enum class WindowState: uint32_t {
	Active		= BitMask(0u),
	Alive		= BitMask(1u),
	Cursor		= BitMask(2u),
	Decorated	= BitMask(3u),
	Focused		= BitMask(4u),
	FullScreen	= BitMask(5u),
	Maximized	= BitMask(6u),
	Minimized	= BitMask(7u),
	Visible		= BitMask(8u),
};

enum class WindowStyle: uint8_t {
	Default		= 0x00,
	Borderless	= 0x01,
	FullScreen	= 0xFF
};

/**
* @brief	Collection of platform independet window related data and properties
*/

struct WindowData {
	// Static
	static constexpr uint32_t BorderWidth = 12;
	static constexpr uint32_t TitleBarWidth = 5;

	// States
	bool Active = false;
	bool Alive = false;
	bool Cursor = false;
	bool Decorated = false;
	bool Focused = false;
	bool FullScreen = false;
	bool Maximized = false;
	bool Minimized = false;
	bool Visible = true;
};

struct WindowProperties {
	string ID;
	string Title;
	string Icon;

	WindowPosition Position;
	WindowSize Size;
	WindowStyle Style;

	WindowSize MaxSize;
	WindowSize MinSize;

	// Window with predefined values (can be used only once for now...)
	WindowProperties():
		ID {"Window[App]"},
		Title {"App"},
		Icon {"Data/App.ico"},
		Position {},
		Size {},
		Style {WindowStyle::Default},
		MaxSize {0, 0},
		MinSize {} {
	};

	// Window with recommended values
	WindowProperties(const string &title, uint32_t width = 640u, uint32_t height = 480u, WindowStyle style = WindowStyle::Default):
		ID {"Window[" + title + "]"},
		Title {title},
		Icon {"Data/" + title + ".ico"},
		Position {},
		Size {width, height},
		Style {style},
		MaxSize {0, 0},
		MinSize {width / 2, height / 2} {
	};
};

}

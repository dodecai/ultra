#pragma once

#include "Omnia/Omnia.pch"

// Disable:
#pragma warning(push)
#pragma warning(disable: 26812)

/**
* @brief CLI Modifiers: Extends output stream with easy support for colors and styles.
*/
namespace Omnia { namespace Cli {

namespace Color {

enum Colors {
	// Foreground
	Default			= 39,
	Black			= 30,
	Red				= 31,
	Green			= 32,
	Yellow			= 33,
	Blue			= 34,
	Magenta			= 35,
	Cyan			= 36,
	LightGrey		= 37,

	DarkGrey		= 90,
	LightRed		= 91,
	LightGreen		= 92,
	LightYellow		= 93,
	LightBlue		= 94,
	LightMagenta	= 95,
	LightCyan		= 96,
	White			= 97,

	// Background
	BackgroundDefault		= 49,
	BackgroundBlack			= 40,
	BackgroundRed			= 41,
	BackgroundGreen			= 42,
	BackgroundYellow		= 43,
	BackgroundBlue			= 44,
	BackgroundMagenta		= 45,
	BackgroundCyan			= 46,
	BackgroundLightGrey		= 47,

	BackgroundGrey			= 100,
	BackgroundLightRed		= 101,
	BackgroundLightGreen	= 102,
	BackgroundLightYellow	= 103,
	BackgroundLightBlue		= 104,
	BackgroundLightMagenta	= 105,
	BackgroundLightCyan		= 106,
	BackgroundWhite			= 107,
};

inline std::ostream &operator<<(std::ostream &os, Colors color) {
	return os << "\x1b[" << static_cast<int>(color) << "m";
}

}

namespace Style {

enum Styles {
	Reset		= 0,
	BoldBright	= 1,
	Dim			= 2,
	Underline	= 4,
	Blink		= 5,
	Inverse		= 7,
	Invisible	= 8,

	RBoldBright		= 21,
	RDim			= 22,
	RUnderLineOff	= 24,
	RBlink			= 25,
	RInverseOff		= 27,
	RInvisible		= 28,
};

inline std::ostream &operator<<(std::ostream &os, Styles style) {
	return os << "\x1b[" << static_cast<int>(style) << "m";
}

}

}}

#pragma warning(pop)

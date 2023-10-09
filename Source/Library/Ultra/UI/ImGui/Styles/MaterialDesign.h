#pragma once

// Material Design
// Source: https://material.io/design/color/the-color-system.html#tools-for-picking-colors

// Color Container
struct ColorRgb {
	float r;
	float g;
	float b;
};

struct ColorRgba {
	float r;
	float g;
	float b;
	float a;
};

enum ColorPalette {
	Red,
	Pink,
	Purple,
	DeepPurple,
	Indigo,
	Blue,
	LightBlue,
	Cyan,
	Teal,
	Green,
	LightGreen,
	Lime,
	Yellow,
	Amber,
	Orange,
	DeepOrange,
	Brown,
	Gray,
	BlueGray,
	Black,
	White
};

struct ColorSwatch {
	uint32_t black	= 0x000000FF;
	uint32_t base	= 0x00000000;
	uint32_t s100	= 0x00000000;
	uint32_t s200	= 0x00000000;
	uint32_t s300	= 0x00000000;
	uint32_t s400	= 0x00000000;
	uint32_t s500	= 0x00000000;
	uint32_t s600	= 0x00000000;
	uint32_t s700	= 0x00000000;
	uint32_t s800	= 0x00000000;
	uint32_t s900	= 0x00000000;
	uint32_t sA100	= 0x00000000;
	uint32_t sA200	= 0x00000000;
	uint32_t sA400	= 0x00000000;
	uint32_t sA500	= 0x00000000;
	uint32_t sA700	= 0x00000000;
	uint32_t white	= 0xFFFFFFFF;

	ColorSwatch() = default;
	virtual ~ColorSwatch() = default;
};

// Accent Colors
struct RedSwatch: ColorSwatch {
	RedSwatch() {
		ColorSwatch::base = 0xFFEBEEFF;
		ColorSwatch::s100 = 0xFFCDD2FF;
		ColorSwatch::s200 = 0xEF9A9AFF;
		ColorSwatch::s300 = 0xE57373FF;
		ColorSwatch::s400 = 0xEF5350FF;
		ColorSwatch::s500 = 0xF44336FF;
		ColorSwatch::s600 = 0xE53935FF;
		ColorSwatch::s700 = 0xD32F2FFF;
		ColorSwatch::s800 = 0xC62828FF;
		ColorSwatch::s900 = 0xB71C1CFF;
		ColorSwatch::sA100 = 0xFF8A80FF;
		ColorSwatch::sA200 = 0xFF5252FF;
		ColorSwatch::sA400 = 0xFF1744FF;
		ColorSwatch::sA700 = 0xD50000FF;
	}
	virtual ~RedSwatch() = default;
};

struct PinkSwatch: ColorSwatch {
	PinkSwatch() {
		ColorSwatch::base = 0xFCE4ECFF;
		ColorSwatch::s100 = 0xF8BBD0FF;
		ColorSwatch::s200 = 0xF48FB1FF;
		ColorSwatch::s300 = 0xF06292FF;
		ColorSwatch::s400 = 0xEC407AFF;
		ColorSwatch::s500 = 0xE91E63FF;
		ColorSwatch::s600 = 0xD81B60FF;
		ColorSwatch::s700 = 0xC2185BFF;
		ColorSwatch::s800 = 0xAD1457FF;
		ColorSwatch::s900 = 0x880E4FFF;
		ColorSwatch::sA100 = 0xFF80ABFF;
		ColorSwatch::sA200 = 0xFF4081FF;
		ColorSwatch::sA400 = 0xF50057FF;
		ColorSwatch::sA700 = 0xC51162FF;
	}
	virtual ~PinkSwatch() = default;
};

struct PurpleSwatch: ColorSwatch {
	PurpleSwatch() {
		ColorSwatch::base = 0xF3E5F5FF;
		ColorSwatch::s100 = 0xE1BEE7FF;
		ColorSwatch::s200 = 0xCE93D8FF;
		ColorSwatch::s300 = 0xBA68C8FF;
		ColorSwatch::s400 = 0xAB47BCFF;
		ColorSwatch::s500 = 0x9C27B0FF;
		ColorSwatch::s600 = 0x8E24AAFF;
		ColorSwatch::s700 = 0x7B1FA2FF;
		ColorSwatch::s800 = 0x6A1B9AFF;
		ColorSwatch::s900 = 0x4A148CFF;
		ColorSwatch::sA100 = 0xEA80FCFF;
		ColorSwatch::sA200 = 0xE040FBFF;
		ColorSwatch::sA400 = 0xD500F9FF;
		ColorSwatch::sA700 = 0xAA00FFFF;
	}
	virtual ~PurpleSwatch() = default;
};

struct DeepPurpleSwatch: ColorSwatch {
	DeepPurpleSwatch() {
		ColorSwatch::base = 0xEDE7F6FF;
		ColorSwatch::s100 = 0xD1C4E9FF;
		ColorSwatch::s200 = 0xB39DDBFF;
		ColorSwatch::s300 = 0x9575CDFF;
		ColorSwatch::s400 = 0x7E57C2FF;
		ColorSwatch::s500 = 0x673AB7FF;
		ColorSwatch::s600 = 0x5E35B1FF;
		ColorSwatch::s700 = 0x512DA8FF;
		ColorSwatch::s800 = 0x4527A0FF;
		ColorSwatch::s900 = 0x311B92FF;
		ColorSwatch::sA100 = 0xB388FFFF;
		ColorSwatch::sA200 = 0x7C4DFFFF;
		ColorSwatch::sA400 = 0x651FFFFF;
		ColorSwatch::sA700 = 0x6200EAFF;
	}
	virtual ~DeepPurpleSwatch() = default;
};

struct IndigoSwatch: ColorSwatch {
	IndigoSwatch() {
		ColorSwatch::base = 0xE8EAF6FF;
		ColorSwatch::s100 = 0xC5CAE9FF;
		ColorSwatch::s200 = 0x9FA8DAFF;
		ColorSwatch::s300 = 0x7986CBFF;
		ColorSwatch::s400 = 0x5C6BC0FF;
		ColorSwatch::s500 = 0x3F51B5FF;
		ColorSwatch::s600 = 0x3949ABFF;
		ColorSwatch::s700 = 0x303F9FFF;
		ColorSwatch::s800 = 0x283593FF;
		ColorSwatch::s900 = 0x1A237EFF;
		ColorSwatch::sA100 = 0x8C9EFFFF;
		ColorSwatch::sA200 = 0x536DFEFF;
		ColorSwatch::sA400 = 0x3D5AFEFF;
		ColorSwatch::sA700 = 0x304FFEFF;
	}
	virtual ~IndigoSwatch() = default;
};

struct BlueSwatch: ColorSwatch {
	BlueSwatch() {
		ColorSwatch::base = 0xE3F2FDFF;
		ColorSwatch::s100 = 0xBBDEFBFF;
		ColorSwatch::s200 = 0x90CAF9FF;
		ColorSwatch::s300 = 0x64B5F6FF;
		ColorSwatch::s400 = 0x42A5F5FF;
		ColorSwatch::s500 = 0x2196F3FF;
		ColorSwatch::s600 = 0x1E88E5FF;
		ColorSwatch::s700 = 0x1976D2FF;
		ColorSwatch::s800 = 0x1565C0FF;
		ColorSwatch::s900 = 0x0D47A1FF;
		ColorSwatch::sA100 = 0x82B1FFFF;
		ColorSwatch::sA200 = 0x448AFFFF;
		ColorSwatch::sA400 = 0x2979FFFF;
		ColorSwatch::sA700 = 0x2962FFFF;
	}
	virtual ~BlueSwatch() = default;
};

struct LightBlueSwatch: ColorSwatch {
	LightBlueSwatch() {
		ColorSwatch::base = 0xE1F5FEFF;
		ColorSwatch::s100 = 0xB3E5FCFF;
		ColorSwatch::s200 = 0x81D4FAFF;
		ColorSwatch::s300 = 0x4FC3F7FF;
		ColorSwatch::s400 = 0x29B6F6FF;
		ColorSwatch::s500 = 0x03A9F4FF;
		ColorSwatch::s600 = 0x039BE5FF;
		ColorSwatch::s700 = 0x0288D1FF;
		ColorSwatch::s800 = 0x0277BDFF;
		ColorSwatch::s900 = 0x01579BFF;
		ColorSwatch::sA100 = 0x80D8FFFF;
		ColorSwatch::sA200 = 0x40C4FFFF;
		ColorSwatch::sA400 = 0x00B0FFFF;
		ColorSwatch::sA700 = 0x0091EAFF;
	}
	virtual ~LightBlueSwatch() = default;
};

struct CyanSwatch: ColorSwatch {
	CyanSwatch() {
		ColorSwatch::base = 0xE0F7FAFF;
		ColorSwatch::s100 = 0xB2EBF2FF;
		ColorSwatch::s200 = 0x80DEEAFF;
		ColorSwatch::s300 = 0x4DD0E1FF;
		ColorSwatch::s400 = 0x26C6DAFF;
		ColorSwatch::s500 = 0x00BCD4FF;
		ColorSwatch::s600 = 0x00ACC1FF;
		ColorSwatch::s700 = 0x0097A7FF;
		ColorSwatch::s800 = 0x00838FFF;
		ColorSwatch::s900 = 0x006064FF;
		ColorSwatch::sA100 = 0x84FFFFFF;
		ColorSwatch::sA200 = 0x18FFFFFF;
		ColorSwatch::sA400 = 0x00E5FFFF;
		ColorSwatch::sA700 = 0x00B8D4FF;
	}
	virtual ~CyanSwatch() = default;
};

struct TealSwatch: ColorSwatch {
	TealSwatch() {
		ColorSwatch::base = 0xE0F2F1FF;
		ColorSwatch::s100 = 0xB2DFDBFF;
		ColorSwatch::s200 = 0x80CBC4FF;
		ColorSwatch::s300 = 0x4DB6ACFF;
		ColorSwatch::s400 = 0x26A69AFF;
		ColorSwatch::s500 = 0x009688FF;
		ColorSwatch::s600 = 0x00897BFF;
		ColorSwatch::s700 = 0x00796BFF;
		ColorSwatch::s800 = 0x00695CFF;
		ColorSwatch::s900 = 0x004D40FF;
		ColorSwatch::sA100 = 0xA7FFEBFF;
		ColorSwatch::sA200 = 0x64FFDAFF;
		ColorSwatch::sA400 = 0x1DE9B6FF;
		ColorSwatch::sA700 = 0x00BFA5FF;
	}
	virtual ~TealSwatch() = default;
};

struct GreenSwatch: ColorSwatch {
	GreenSwatch() {
		ColorSwatch::base = 0xE8F5E9FF;
		ColorSwatch::s100 = 0xC8E6C9FF;
		ColorSwatch::s200 = 0xA5D6A7FF;
		ColorSwatch::s300 = 0x81C784FF;
		ColorSwatch::s400 = 0x66BB6AFF;
		ColorSwatch::s500 = 0x4CAF50FF;
		ColorSwatch::s600 = 0x43A047FF;
		ColorSwatch::s700 = 0x388E3CFF;
		ColorSwatch::s800 = 0x2E7D32FF;
		ColorSwatch::s900 = 0x1B5E20FF;
		ColorSwatch::sA100 = 0xB9F6CAFF;
		ColorSwatch::sA200 = 0x69F0AEFF;
		ColorSwatch::sA400 = 0x00E676FF;
		ColorSwatch::sA700 = 0x00C853FF;
	}
	virtual ~GreenSwatch() = default;
};

struct LightGreenSwatch: ColorSwatch {
	LightGreenSwatch() {
		ColorSwatch::base = 0xF1F8E9FF;
		ColorSwatch::s100 = 0xDCEDC8FF;
		ColorSwatch::s200 = 0xC5E1A5FF;
		ColorSwatch::s300 = 0xAED581FF;
		ColorSwatch::s400 = 0x9CCC65FF;
		ColorSwatch::s500 = 0x8BC34AFF;
		ColorSwatch::s600 = 0x7CB342FF;
		ColorSwatch::s700 = 0x689F38FF;
		ColorSwatch::s800 = 0x558B2FFF;
		ColorSwatch::s900 = 0x33691EFF;
		ColorSwatch::sA100 = 0xCCFF90FF;
		ColorSwatch::sA200 = 0xB2FF59FF;
		ColorSwatch::sA400 = 0x76FF03FF;
		ColorSwatch::sA700 = 0x64DD17FF;
	}
	virtual ~LightGreenSwatch() = default;
};

struct LimeSwatch: ColorSwatch {
	LimeSwatch() {
		ColorSwatch::base = 0xF9FBE7FF;
		ColorSwatch::s100 = 0xF0F4C3FF;
		ColorSwatch::s200 = 0xE6EE9CFF;
		ColorSwatch::s300 = 0xDCE775FF;
		ColorSwatch::s400 = 0xD4E157FF;
		ColorSwatch::s500 = 0xCDDC39FF;
		ColorSwatch::s600 = 0xC0CA33FF;
		ColorSwatch::s700 = 0xAFB42BFF;
		ColorSwatch::s800 = 0x9E9D24FF;
		ColorSwatch::s900 = 0x827717FF;
		ColorSwatch::sA100 = 0xF4FF81FF;
		ColorSwatch::sA200 = 0xEEFF41FF;
		ColorSwatch::sA400 = 0xC6FF00FF;
		ColorSwatch::sA700 = 0xAEEA00FF;
	}
	virtual ~LimeSwatch() = default;
};

struct YellowSwatch: ColorSwatch {
	YellowSwatch() {
		ColorSwatch::base = 0xFFFDE7FF;
		ColorSwatch::s100 = 0xFFF9C4FF;
		ColorSwatch::s200 = 0xFFF59DFF;
		ColorSwatch::s300 = 0xFFF176FF;
		ColorSwatch::s400 = 0xFFEE58FF;
		ColorSwatch::s500 = 0xFFEB3BFF;
		ColorSwatch::s600 = 0xFDD835FF;
		ColorSwatch::s700 = 0xFBC02DFF;
		ColorSwatch::s800 = 0xF9A825FF;
		ColorSwatch::s900 = 0xF57F17FF;
		ColorSwatch::sA100 = 0xFFFF8DFF;
		ColorSwatch::sA200 = 0xFFFF00FF;
		ColorSwatch::sA400 = 0xFFEA00FF;
		ColorSwatch::sA700 = 0xFFD600FF;
	}
	virtual ~YellowSwatch() = default;
};

struct AmberSwatch: ColorSwatch {
	AmberSwatch() {
		ColorSwatch::base = 0xFFF8E1FF;
		ColorSwatch::s100 = 0xFFECB3FF;
		ColorSwatch::s200 = 0xFFE082FF;
		ColorSwatch::s300 = 0xFFD54FFF;
		ColorSwatch::s400 = 0xFFCA28FF;
		ColorSwatch::s500 = 0xFFC107FF;
		ColorSwatch::s600 = 0xFFB300FF;
		ColorSwatch::s700 = 0xFFA000FF;
		ColorSwatch::s800 = 0xFF8F00FF;
		ColorSwatch::s900 = 0xFF6F00FF;
		ColorSwatch::sA100 = 0xFFE57FFF;
		ColorSwatch::sA200 = 0xFFD740FF;
		ColorSwatch::sA400 = 0xFFC400FF;
		ColorSwatch::sA700 = 0xFFAB00FF;
	}
	virtual ~AmberSwatch() = default;
};

struct OrangeSwatch: ColorSwatch {
	OrangeSwatch() {
		ColorSwatch::base = 0xFFF3E0FF;
		ColorSwatch::s100 = 0xFFE0B2FF;
		ColorSwatch::s200 = 0xFFCC80FF;
		ColorSwatch::s300 = 0xFFB74DFF;
		ColorSwatch::s400 = 0xFFA726FF;
		ColorSwatch::s500 = 0xFF9800FF;
		ColorSwatch::s600 = 0xFB8C00FF;
		ColorSwatch::s700 = 0xF57C00FF;
		ColorSwatch::s800 = 0xEF6C00FF;
		ColorSwatch::s900 = 0xE65100FF;
		ColorSwatch::sA100 = 0xFFD180FF;
		ColorSwatch::sA200 = 0xFFAB40FF;
		ColorSwatch::sA400 = 0xFF9100FF;
		ColorSwatch::sA700 = 0xFF6D00FF;
	}
	virtual ~OrangeSwatch() = default;
};

struct DeepOrangeSwatch: ColorSwatch {
	DeepOrangeSwatch() {
		ColorSwatch::base = 0xFBE9E7FF;
		ColorSwatch::s100 = 0xFFCCBCFF;
		ColorSwatch::s200 = 0xFFAB91FF;
		ColorSwatch::s300 = 0xFF8A65FF;
		ColorSwatch::s400 = 0xFF7043FF;
		ColorSwatch::s500 = 0xFF5722FF;
		ColorSwatch::s600 = 0xF4511EFF;
		ColorSwatch::s700 = 0xE64A19FF;
		ColorSwatch::s800 = 0xD84315FF;
		ColorSwatch::s900 = 0xBF360CFF;
		ColorSwatch::sA100 = 0xFF9E80FF;
		ColorSwatch::sA200 = 0xFF6E40FF;
		ColorSwatch::sA400 = 0xFF3D00FF;
		ColorSwatch::sA700 = 0xDD2C00FF;
	}
	virtual ~DeepOrangeSwatch() = default;
};

struct BrownSwatch: ColorSwatch {
	BrownSwatch() {
		ColorSwatch::base = 0xEFEBE9FF;
		ColorSwatch::s100 = 0xD7CCC8FF;
		ColorSwatch::s200 = 0xBCAAA4FF;
		ColorSwatch::s300 = 0xA1887FFF;
		ColorSwatch::s400 = 0x8D6E63FF;
		ColorSwatch::s500 = 0x795548FF;
		ColorSwatch::s600 = 0x6D4C41FF;
		ColorSwatch::s700 = 0x5D4037FF;
		ColorSwatch::s800 = 0x4E342EFF;
		ColorSwatch::s900 = 0x3E2723FF;
	}
	virtual ~BrownSwatch() = default;
};

struct GraySwatch: ColorSwatch {
	GraySwatch() {
		ColorSwatch::base = 0xFAFAFAFF;
		ColorSwatch::s100 = 0xF5F5F5FF;
		ColorSwatch::s200 = 0xEEEEEEFF;
		ColorSwatch::s300 = 0xE0E0E0FF;
		ColorSwatch::s400 = 0xBDBDBDFF;
		ColorSwatch::s500 = 0x9E9E9EFF;
		ColorSwatch::s600 = 0x757575FF;
		ColorSwatch::s700 = 0x616161FF;
		ColorSwatch::s800 = 0x424242FF;
		ColorSwatch::s900 = 0x212121FF;
	}
	virtual ~GraySwatch() = default;
};

struct BlueGraySwatch: ColorSwatch {
	BlueGraySwatch() {
		ColorSwatch::base = 0xECEFF1FF;
		ColorSwatch::s100 = 0xCFD8DCFF;
		ColorSwatch::s200 = 0xB0BEC5FF;
		ColorSwatch::s300 = 0x90A4AEFF;
		ColorSwatch::s400 = 0x78909CFF;
		ColorSwatch::s500 = 0x607D8BFF;
		ColorSwatch::s600 = 0x546E7AFF;
		ColorSwatch::s700 = 0x455A64FF;
		ColorSwatch::s800 = 0x37474FFF;
		ColorSwatch::s900 = 0x263238FF;
	}
	virtual ~BlueGraySwatch() = default;
};

// Themes
enum ColorThemes {
	Dark,
	Light
};

struct ColorTheme {
	uint32_t background	= 0x000000FF;
	uint32_t surface	= 0x000000FF;
	uint32_t text		= 0x000000FF;
	uint32_t disabled	= 0x000000FF;
};

struct DarkTheme: public ColorTheme {
	DarkTheme() {
		//style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		ColorTheme::background	= 0x0D0F12FF;
		ColorTheme::surface		= 0x1A171EFF;
		ColorTheme::text		= 0xE0E0E8FF;
		ColorTheme::disabled	= 0x646472FF;
	}
	virtual ~DarkTheme() = default;
};

struct LightTheme: public ColorTheme {
	LightTheme() {
		ColorTheme::background	= 0xeaeaeaff;
		ColorTheme::surface		= 0xE5E8E1FF;
		ColorTheme::text		= 0x060606FF;
		ColorTheme::disabled	= 0x646472FF;
	}
	virtual ~LightTheme() = default;
};

// Themes - Console
struct ConsoleLightTheme {
	static inline uint32_t Background	= 0xeaeaeaff;
	static inline uint32_t Foreground	= 0x65e0e2ff;
};

struct ConsoleDarkTheme {
	static inline uint32_t Background	= 0x0f111aff;
	static inline uint32_t Foreground	= 0x65e0e2ff;
};

struct MaterialOcean {
	static inline uint32_t Background	= 0x0f111aff;
	static inline uint32_t Foreground	= 0x65e0e2ff;

	static inline uint32_t Red			= 0x7d2627ff;
	static inline uint32_t Green		= 0x4d9014ff;
	static inline uint32_t Blue			= 0x4747ffff;

	static inline uint32_t Cyan			= 0x00a3a3ff;
	static inline uint32_t Magenta		= 0x9531d8ff;
	static inline uint32_t Yellow		= 0xbdd534ff;
	static inline uint32_t Black		= 0x435760ff;

	static inline uint32_t Gray			= 0x546e7aff;
	static inline uint32_t LightGray	= 0x65e0e2ff;

	static inline uint32_t LightRed		= 0xd47374ff;
	static inline uint32_t LightGreen	= 0xa0e95eff;
	static inline uint32_t LightBlue	= 0xb8b8ffff;

	static inline uint32_t LightCyan	= 0x47ffffff;
	static inline uint32_t LightMagenta	= 0xc792eaff;
	static inline uint32_t LightYellow	= 0xcee067ff;
	static inline uint32_t White		= 0xe6ffffff;
};


// Utilities
static std::shared_ptr<ColorSwatch> GetColorPalette(ColorPalette color) {
	std::shared_ptr<ColorSwatch> result;
	switch (color) {
		case ColorPalette::Red:			{ result = std::make_shared<RedSwatch>();			break; }
		case ColorPalette::Pink:		{ result = std::make_shared<PinkSwatch>();			break; }
		case ColorPalette::Purple:		{ result = std::make_shared<PurpleSwatch>();		break; }
		case ColorPalette::DeepPurple:	{ result = std::make_shared<DeepPurpleSwatch>();	break; }
		case ColorPalette::Indigo:		{ result = std::make_shared<IndigoSwatch>();		break; }
		case ColorPalette::Blue:		{ result = std::make_shared<BlueSwatch>();			break; }
		case ColorPalette::LightBlue:	{ result = std::make_shared<LightBlueSwatch>();		break; }
		case ColorPalette::Cyan:		{ result = std::make_shared<CyanSwatch>();			break; }
		case ColorPalette::Teal:		{ result = std::make_shared<TealSwatch>();			break; }
		case ColorPalette::Green:		{ result = std::make_shared<GreenSwatch>();			break; }
		case ColorPalette::LightGreen:	{ result = std::make_shared<LightGreenSwatch>();	break; }
		case ColorPalette::Lime:		{ result = std::make_shared<LimeSwatch>();			break; }
		case ColorPalette::Yellow:		{ result = std::make_shared<YellowSwatch>();		break; }
		case ColorPalette::Amber:		{ result = std::make_shared<AmberSwatch>();			break; }
		case ColorPalette::Orange:		{ result = std::make_shared<OrangeSwatch>();		break; }
		case ColorPalette::DeepOrange:	{ result = std::make_shared<DeepOrangeSwatch>();	break; }
		case ColorPalette::Brown:		{ result = std::make_shared<BrownSwatch>();			break; }
		case ColorPalette::Gray:		{ result = std::make_shared<GraySwatch>();			break; }
		case ColorPalette::BlueGray:	{ result = std::make_shared<BlueGraySwatch>();		break; }
	}
	return result;
}

static std::shared_ptr<ColorTheme> GetColorTheme(ColorThemes theme) {
	std::shared_ptr<ColorTheme> result;
	switch (theme) {
		case ColorThemes::Dark:			{ result = std::make_shared<DarkTheme>();			break; }
		case ColorThemes::Light:		{ result = std::make_shared<LightTheme>();			break; }
	}
	return result;
}

static ColorRgba ConvertColorRgba(uint32_t value, float customAlpha = 1.0f) {
	ColorRgba result;
	result.r = static_cast<float>((value & 0xFF000000) >> 24) / 255.0f;
	result.g = static_cast<float>((value & 0x00FF0000) >> 16) / 255.0f;
	result.b = static_cast<float>((value & 0x0000FF00) >> 8) / 255.0f;
	result.a = customAlpha == 1.0f ? static_cast<float>((value & 0x000000FF)) / 255.0f : customAlpha;

	return result;
}

static ImVec4 ConvertColorRgbaVec(uint32_t value, float customAlpha = 1.0f) {
	ImVec4 result;
	result.x = static_cast<float>((value & 0xFF000000) >> 24) / 255.0f;
	result.y = static_cast<float>((value & 0x00FF0000) >> 16) / 255.0f;
	result.z = static_cast<float>((value & 0x0000FF00) >> 8) / 255.0f;
	result.w = customAlpha == 1.0f ? static_cast<float>((value & 0x000000FF)) / 255.0f : customAlpha;

	return result;
}

static ImVec4 operator*(ImVec4 lhs, ImVec4 rhs) {
    ImVec4 result;
    result.x = lhs.x * rhs.x;
    result.y = lhs.y * rhs.y;
    result.z = lhs.z * rhs.z;
    result.w = lhs.w * rhs.w;
    return result;
}
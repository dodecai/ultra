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

// Color Palettes
struct ColorSwatch {
	unsigned int base = 0x00000000;
	unsigned int s100 = 0x00000000;
	unsigned int s200 = 0x00000000;
	unsigned int s300 = 0x00000000;
	unsigned int s400 = 0x00000000;
	unsigned int s500 = 0x00000000;
	unsigned int s600 = 0x00000000;
	unsigned int s700 = 0x00000000;
	unsigned int s800 = 0x00000000;
	unsigned int s900 = 0x00000000;
	unsigned int sA100 = 0x00000000;
	unsigned int sA200 = 0x00000000;
	unsigned int sA400 = 0x00000000;
	unsigned int sA500 = 0x00000000;
	unsigned int sA700 = 0x00000000;
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

// Custom

// Default
struct Red50: ColorSwatch {
	unsigned int base = 0xFFEBEEFF;
	unsigned int s100 = 0xFFCDD2FF;
	unsigned int s200 = 0xEF9A9AFF;
	unsigned int s300 = 0xE57373FF;
	unsigned int s400 = 0xEF5350FF;
	unsigned int s500 = 0xF44336FF;
	unsigned int s600 = 0xE53935FF;
	unsigned int s700 = 0xD32F2FFF;
	unsigned int s800 = 0xC62828FF;
	unsigned int s900 = 0xB71C1CFF;
	unsigned int sA100 = 0xFF8A80FF;
	unsigned int sA200 = 0xFF5252FF;
	unsigned int sA400 = 0xFF1744FF;
	unsigned int sA700 = 0xD50000FF;
};

struct Pink50: ColorSwatch {
	unsigned int base = 0xFCE4ECFF;
	unsigned int s100 = 0xF8BBD0FF;
	unsigned int s200 = 0xF48FB1FF;
	unsigned int s300 = 0xF06292FF;
	unsigned int s400 = 0xEC407AFF;
	unsigned int s500 = 0xE91E63FF;
	unsigned int s600 = 0xD81B60FF;
	unsigned int s700 = 0xC2185BFF;
	unsigned int s800 = 0xAD1457FF;
	unsigned int s900 = 0x880E4FFF;
	unsigned int sA100 = 0xFF80ABFF;
	unsigned int sA200 = 0xFF4081FF;
	unsigned int sA400 = 0xF50057FF;
	unsigned int sA700 = 0xC51162FF;
};

struct Purple50: ColorSwatch {
	unsigned int base = 0xF3E5F5FF;
	unsigned int s100 = 0xE1BEE7FF;
	unsigned int s200 = 0xCE93D8FF;
	unsigned int s300 = 0xBA68C8FF;
	unsigned int s400 = 0xAB47BCFF;
	unsigned int s500 = 0x9C27B0FF;
	unsigned int s600 = 0x8E24AAFF;
	unsigned int s700 = 0x7B1FA2FF;
	unsigned int s800 = 0x6A1B9AFF;
	unsigned int s900 = 0x4A148CFF;
	unsigned int sA100 = 0xEA80FCFF;
	unsigned int sA200 = 0xE040FBFF;
	unsigned int sA400 = 0xD500F9FF;
	unsigned int sA700 = 0xAA00FFFF;
};

struct DeepPurple50: ColorSwatch {
	unsigned int base = 0xEDE7F6FF;
	unsigned int s100 = 0xD1C4E9FF;
	unsigned int s200 = 0xB39DDBFF;
	unsigned int s300 = 0x9575CDFF;
	unsigned int s400 = 0x7E57C2FF;
	unsigned int s500 = 0x673AB7FF;
	unsigned int s600 = 0x5E35B1FF;
	unsigned int s700 = 0x512DA8FF;
	unsigned int s800 = 0x4527A0FF;
	unsigned int s900 = 0x311B92FF;
	unsigned int sA100 = 0xB388FFFF;
	unsigned int sA200 = 0x7C4DFFFF;
	unsigned int sA400 = 0x651FFFFF;
	unsigned int sA700 = 0x6200EAFF;
};

struct Indigo50: ColorSwatch {
	unsigned int base = 0xE8EAF6FF;
	unsigned int s100 = 0xC5CAE9FF;
	unsigned int s200 = 0x9FA8DAFF;
	unsigned int s300 = 0x7986CBFF;
	unsigned int s400 = 0x5C6BC0FF;
	unsigned int s500 = 0x3F51B5FF;
	unsigned int s600 = 0x3949ABFF;
	unsigned int s700 = 0x303F9FFF;
	unsigned int s800 = 0x283593FF;
	unsigned int s900 = 0x1A237EFF;
	unsigned int sA100 = 0x8C9EFFFF;
	unsigned int sA200 = 0x536DFEFF;
	unsigned int sA400 = 0x3D5AFEFF;
	unsigned int sA700 = 0x304FFEFF;
};

struct Blue50: ColorSwatch {
	unsigned int base = 0xE3F2FDFF;
	unsigned int s100 = 0xBBDEFBFF;
	unsigned int s200 = 0x90CAF9FF;
	unsigned int s300 = 0x64B5F6FF;
	unsigned int s400 = 0x42A5F5FF;
	unsigned int s500 = 0x2196F3FF;
	unsigned int s600 = 0x1E88E5FF;
	unsigned int s700 = 0x1976D2FF;
	unsigned int s800 = 0x1565C0FF;
	unsigned int s900 = 0x0D47A1FF;
	unsigned int sA100 = 0x82B1FFFF;
	unsigned int sA200 = 0x448AFFFF;
	unsigned int sA400 = 0x2979FFFF;
	unsigned int sA700 = 0x2962FFFF;
};

struct LightBlue50: ColorSwatch {
	unsigned int base = 0xE1F5FEFF;
	unsigned int s100 = 0xB3E5FCFF;
	unsigned int s200 = 0x81D4FAFF;
	unsigned int s300 = 0x4FC3F7FF;
	unsigned int s400 = 0x29B6F6FF;
	unsigned int s500 = 0x03A9F4FF;
	unsigned int s600 = 0x039BE5FF;
	unsigned int s700 = 0x0288D1FF;
	unsigned int s800 = 0x0277BDFF;
	unsigned int s900 = 0x01579BFF;
	unsigned int sA100 = 0x80D8FFFF;
	unsigned int sA200 = 0x40C4FFFF;
	unsigned int sA400 = 0x00B0FFFF;
	unsigned int sA700 = 0x0091EAFF;
};

struct Cyan50: ColorSwatch {
	unsigned int base = 0xE0F7FAFF;
	unsigned int s100 = 0xB2EBF2FF;
	unsigned int s200 = 0x80DEEAFF;
	unsigned int s300 = 0x4DD0E1FF;
	unsigned int s400 = 0x26C6DAFF;
	unsigned int s500 = 0x00BCD4FF;
	unsigned int s600 = 0x00ACC1FF;
	unsigned int s700 = 0x0097A7FF;
	unsigned int s800 = 0x00838FFF;
	unsigned int s900 = 0x006064FF;
	unsigned int sA100 = 0x84FFFFFF;
	unsigned int sA200 = 0x18FFFFFF;
	unsigned int sA400 = 0x00E5FFFF;
	unsigned int sA700 = 0x00B8D4FF;
};

struct Teal50: ColorSwatch {
	unsigned int base = 0xE0F2F1FF;
	unsigned int s100 = 0xB2DFDBFF;
	unsigned int s200 = 0x80CBC4FF;
	unsigned int s300 = 0x4DB6ACFF;
	unsigned int s400 = 0x26A69AFF;
	unsigned int s500 = 0x009688FF;
	unsigned int s600 = 0x00897BFF;
	unsigned int s700 = 0x00796BFF;
	unsigned int s800 = 0x00695CFF;
	unsigned int s900 = 0x004D40FF;
	unsigned int sA100 = 0xA7FFEBFF;
	unsigned int sA200 = 0x64FFDAFF;
	unsigned int sA400 = 0x1DE9B6FF;
	unsigned int sA700 = 0x00BFA5FF;
};

struct Green50: ColorSwatch {
	unsigned int base = 0xE8F5E9FF;
	unsigned int s100 = 0xC8E6C9FF;
	unsigned int s200 = 0xA5D6A7FF;
	unsigned int s300 = 0x81C784FF;
	unsigned int s400 = 0x66BB6AFF;
	unsigned int s500 = 0x4CAF50FF;
	unsigned int s600 = 0x43A047FF;
	unsigned int s700 = 0x388E3CFF;
	unsigned int s800 = 0x2E7D32FF;
	unsigned int s900 = 0x1B5E20FF;
	unsigned int sA100 = 0xB9F6CAFF;
	unsigned int sA200 = 0x69F0AEFF;
	unsigned int sA400 = 0x00E676FF;
	unsigned int sA700 = 0x00C853FF;
};

struct LightGreen50: ColorSwatch {
	unsigned int base = 0xF1F8E9FF;
	unsigned int s100 = 0xDCEDC8FF;
	unsigned int s200 = 0xC5E1A5FF;
	unsigned int s300 = 0xAED581FF;
	unsigned int s400 = 0x9CCC65FF;
	unsigned int s500 = 0x8BC34AFF;
	unsigned int s600 = 0x7CB342FF;
	unsigned int s700 = 0x689F38FF;
	unsigned int s800 = 0x558B2FFF;
	unsigned int s900 = 0x33691EFF;
	unsigned int sA100 = 0xCCFF90FF;
	unsigned int sA200 = 0xB2FF59FF;
	unsigned int sA400 = 0x76FF03FF;
	unsigned int sA700 = 0x64DD17FF;
};

struct Lime50: ColorSwatch {
	unsigned int base = 0xF9FBE7FF;
	unsigned int s100 = 0xF0F4C3FF;
	unsigned int s200 = 0xE6EE9CFF;
	unsigned int s300 = 0xDCE775FF;
	unsigned int s400 = 0xD4E157FF;
	unsigned int s500 = 0xCDDC39FF;
	unsigned int s600 = 0xC0CA33FF;
	unsigned int s700 = 0xAFB42BFF;
	unsigned int s800 = 0x9E9D24FF;
	unsigned int s900 = 0x827717FF;
	unsigned int sA100 = 0xF4FF81FF;
	unsigned int sA200 = 0xEEFF41FF;
	unsigned int sA400 = 0xC6FF00FF;
	unsigned int sA700 = 0xAEEA00FF;
};

struct Yellow50: ColorSwatch {
	unsigned int base = 0xFFFDE7FF;
	unsigned int s100 = 0xFFF9C4FF;
	unsigned int s200 = 0xFFF59DFF;
	unsigned int s300 = 0xFFF176FF;
	unsigned int s400 = 0xFFEE58FF;
	unsigned int s500 = 0xFFEB3BFF;
	unsigned int s600 = 0xFDD835FF;
	unsigned int s700 = 0xFBC02DFF;
	unsigned int s800 = 0xF9A825FF;
	unsigned int s900 = 0xF57F17FF;
	unsigned int sA100 = 0xFFFF8DFF;
	unsigned int sA200 = 0xFFFF00FF;
	unsigned int sA400 = 0xFFEA00FF;
	unsigned int sA700 = 0xFFD600FF;
};

struct Amber50: ColorSwatch {
	unsigned int base = 0xFFF8E1FF;
	unsigned int s100 = 0xFFECB3FF;
	unsigned int s200 = 0xFFE082FF;
	unsigned int s300 = 0xFFD54FFF;
	unsigned int s400 = 0xFFCA28FF;
	unsigned int s500 = 0xFFC107FF;
	unsigned int s600 = 0xFFB300FF;
	unsigned int s700 = 0xFFA000FF;
	unsigned int s800 = 0xFF8F00FF;
	unsigned int s900 = 0xFF6F00FF;
	unsigned int sA100 = 0xFFE57FFF;
	unsigned int sA200 = 0xFFD740FF;
	unsigned int sA400 = 0xFFC400FF;
	unsigned int sA700 = 0xFFAB00FF;
};

struct Orange50: ColorSwatch {
	unsigned int base = 0xFFF3E0FF;
	unsigned int s100 = 0xFFE0B2FF;
	unsigned int s200 = 0xFFCC80FF;
	unsigned int s300 = 0xFFB74DFF;
	unsigned int s400 = 0xFFA726FF;
	unsigned int s500 = 0xFF9800FF;
	unsigned int s600 = 0xFB8C00FF;
	unsigned int s700 = 0xF57C00FF;
	unsigned int s800 = 0xEF6C00FF;
	unsigned int s900 = 0xE65100FF;
	unsigned int sA100 = 0xFFD180FF;
	unsigned int sA200 = 0xFFAB40FF;
	unsigned int sA400 = 0xFF9100FF;
	unsigned int sA700 = 0xFF6D00FF;
};

struct DeepOrange50: ColorSwatch {
	unsigned int base = 0xFBE9E7FF;
	unsigned int s100 = 0xFFCCBCFF;
	unsigned int s200 = 0xFFAB91FF;
	unsigned int s300 = 0xFF8A65FF;
	unsigned int s400 = 0xFF7043FF;
	unsigned int s500 = 0xFF5722FF;
	unsigned int s600 = 0xF4511EFF;
	unsigned int s700 = 0xE64A19FF;
	unsigned int s800 = 0xD84315FF;
	unsigned int s900 = 0xBF360CFF;
	unsigned int sA100 = 0xFF9E80FF;
	unsigned int sA200 = 0xFF6E40FF;
	unsigned int sA400 = 0xFF3D00FF;
	unsigned int sA700 = 0xDD2C00FF;
};

struct Brown50: ColorSwatch {
	unsigned int base = 0xEFEBE9FF;
	unsigned int s100 = 0xD7CCC8FF;
	unsigned int s200 = 0xBCAAA4FF;
	unsigned int s300 = 0xA1887FFF;
	unsigned int s400 = 0x8D6E63FF;
	unsigned int s500 = 0x795548FF;
	unsigned int s600 = 0x6D4C41FF;
	unsigned int s700 = 0x5D4037FF;
	unsigned int s800 = 0x4E342EFF;
	unsigned int s900 = 0x3E2723FF;
};

struct Gray50: ColorSwatch {
	unsigned int base = 0xFAFAFAFF;
	unsigned int s100 = 0xF5F5F5FF;
	unsigned int s200 = 0xEEEEEEFF;
	unsigned int s300 = 0xE0E0E0FF;
	unsigned int s400 = 0xBDBDBDFF;
	unsigned int s500 = 0x9E9E9EFF;
	unsigned int s600 = 0x757575FF;
	unsigned int s700 = 0x616161FF;
	unsigned int s800 = 0x424242FF;
	unsigned int s900 = 0x212121FF;
};

struct BlueGray50: ColorSwatch {
	unsigned int base = 0xECEFF1FF;
	unsigned int s100 = 0xCFD8DCFF;
	unsigned int s200 = 0xB0BEC5FF;
	unsigned int s300 = 0x90A4AEFF;
	unsigned int s400 = 0x78909CFF;
	unsigned int s500 = 0x607D8BFF;
	unsigned int s600 = 0x546E7AFF;
	unsigned int s700 = 0x455A64FF;
	unsigned int s800 = 0x37474FFF;
	unsigned int s900 = 0x263238FF;
};

struct Black50: ColorSwatch {
	unsigned int base = 0x000000FF;
};

struct White50: ColorSwatch {
	unsigned int base = 0xFFFFFFFF;
};

// Utilities
inline ColorRgba ConvertColorRgba(unsigned int value) {
	ColorRgba result;
	result.r = static_cast<float>((value & 0xFF000000) >> 24) / 255;
	result.g = static_cast<float>((value & 0x00FF0000) >> 16) / 255;
	result.b = static_cast<float>((value & 0x0000FF00) >> 8) / 255;
	result.a = static_cast<float>((value & 0x000000FF));

	return result;
}

inline ImVec4 ConvertColorRgbaVec(unsigned int value) {
	ImVec4 result;
	result.x = static_cast<float>((value & 0xFF000000) >> 24) / 255.0f;
	result.y = static_cast<float>((value & 0x00FF0000) >> 16) / 255.0f;
	result.z = static_cast<float>((value & 0x0000FF00) >> 8) / 255.0f;
	result.w = static_cast<float>((value & 0x000000FF)) / 255.0f;

	return result;
}

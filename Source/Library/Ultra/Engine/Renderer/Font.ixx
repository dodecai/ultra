module;

#include "ft2build.h"
#include FT_FREETYPE_H

export module Ultra.Engine.Font;

import Ultra.Core;
import Ultra.Logger;
import Ultra.Math;

import Ultra.Engine.Renderer.Texture;

///
/// @brief Internal Types
///
namespace Ultra {

struct Glyph {
    size_t index;
    int32_t advance;

    int32_t x0;
    int32_t y0;
    int32_t x1;
    int32_t y1;

    int32_t sx;
    int32_t sy;

    Reference<Texture2D> Texture;
};

struct FontData {
    FT_Face Handle;

    Glyph *AsciiGlyphs[256];
    unordered_map<uint32_t, Glyph *> Glyphs;
};

}

///
/// @brief Font
///
export namespace Ultra {

struct FontSize {
    int X {};
    int Y {};
    int Width {};
    int Height {};
};

class Font {
public:
    // Default
    Font(string_view name, uint32_t size);
    ~Font();

    // Methods
    void Draw(string_view text, float x, float y, float r, float g, float b, float a);
    
    // Accessors
    int GetLineHeight();
    FontSize GetSize(const char *text);
    FontSize GetSizeFull(const char *text);

private:
    // Helpers
    Glyph *GetGlyph(uint32_t codepoint);
    int GetKerning(int a, int b);

private:
    // Handles
    inline static FT_Library mLibrary = 0;

    // Properties
    Scope<FontData> mData;
    inline static const float kGamma = 1.8f; // @Note : Gamma of 1.8 recommended by FreeType
    inline static const float kRcpGamma = 1.0f / kGamma;
};

}

module;

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_BITMAP_H

export module Ultra.Engine.Font;

import Ultra.Core;
import Ultra.Logger;
import Ultra.Math;

import Ultra.Engine.Renderer.Texture;

///
/// @brief Font
///
export namespace Ultra {

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

class Font {
public:
    Font(string_view name, uint32_t size);
    ~Font();

    // ToDo: Legacy
    static FontData *Load(string_view name, int size);
    static void Free(FontData *);

    static void Draw(FontData *, const char *text, float x, float y, float r, float g, float b, float a);

    static int GetLineHeight(FontData *);
    static void GetSize(FontData *, Vector4Di *out, const char *text);
    static void GetSize2(FontData *, Vector2Di *out, const char *text);

private:
    static Glyph *GetGlyph(FontData *self, uint32_t codepoint);
    static int GetKerning(FontData *self, int a, int b);

private:
    inline static FT_Library mLibrary = 0;
    Scope<FontData> mData;

    // @Note : Gamma of 1.8 recommended by FreeType
    inline static const float kGamma = 1.8f;
    inline static const float kRcpGamma = 1.0f / kGamma;
};


}

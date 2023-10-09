module;

#include "ft2build.h"
#include FT_FREETYPE_H

#define MSDF_ATLAS_PUBLIC
#include <msdfgen/msdfgen.h>
#include <msdf-atlas-gen/FontGeometry.h>
#include <msdf-atlas-gen/GlyphGeometry.h>

export module Ultra.Renderer.Font;

import Ultra.Core;
import Ultra.Logger;
import Ultra.Math;
import Ultra.Renderer.Data;
import Ultra.Renderer.Texture;

///
/// @brief Font
///
export namespace Ultra {

struct FontSize {
    Position Position;
    Size Size;
};

struct Glyph {
    uint32_t UniqueID;
    int32_t Advance;

    int32_t X;
    int32_t Y;
    int32_t Width;
    int32_t Height;

    Reference<Texture2D> Texture;
};

struct FontData {
    FT_Face Handle;

    array<Scope<Glyph>, 256> AsciiGlyphs {};
    unordered_map<uint32_t, Scope<Glyph>> Glyphs;
};

struct MSDFData {
    msdf_atlas::FontGeometry FontGeometry {};
    std::vector<msdf_atlas::GlyphGeometry> Glyphs {};
};


class Font {
public:
    // Default
    Font(string_view name, uint32_t size);
    ~Font();

    static Reference<Font> GetDefault();
    Reference<Texture2D> GetTexture() const { return mAtlasTexture; }
    const MSDFData *GetMSDFData() const { return mMSDFData; }

    // Accessors
    int GetLineHeight();
    ///
    /// @note: The height returned here is the maximal *ascender* height for the string. This allows easy centering of text while still allowing descending characters to look correct.
    /// To correctly center text, first compute bounds via this function, then draw it at:
    /// pos.x - (size.x - bound.x) / 2
    /// pos.y - (size.y + bound.y) / 2
    ///
    Size GetSize(string_view text);
    FontSize GetSizeFull(string_view text);

    // Helpers
    Glyph *GetGlyph(uint32_t codepoint);
    int32_t GetKerning(uint32_t leftGlyph, uint32_t rightGlyph);

    // AtlasTextuer-Test
    void Load(string_view path, uint32_t size);

private:
    // Handles
    inline static FT_Library mLibrary = 0;

    // Properties
    Scope<FontData> mData;
    inline static const float kGamma = 1.8f; // @Note : Gamma of 1.8 recommended by FreeType
    inline static const float kRcpGamma = 1.0f / kGamma;

    // AtlasTextuer-Test
    Reference<Texture2D> mAtlasTexture;
    MSDFData *mMSDFData {};

    struct CharsetRange {
        uint32_t Begin;
        uint32_t End;
    };

    inline static constexpr auto sExpensiveColoring = false;
    inline static constexpr auto sLCGMultiplier = 6364136223846793005ull;
    inline static constexpr auto sLCGIncrement = 1442695040888963407ull;
    inline static constexpr auto sMaxCornerAngle = 3.0;

public:
    inline static constexpr auto sThreadCount = 4;

private:
    inline static constexpr CharsetRange sCharsetRanges[] = {
        { 0x0020, 0x00ff }
    };
};

}

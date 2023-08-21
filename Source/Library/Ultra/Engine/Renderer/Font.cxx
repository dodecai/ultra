module;

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_BITMAP_H

module Ultra.Engine.Font;

import Ultra.Engine.UIRenderer;
import Ultra.Engine.Resource;

// ToDo: Use Atlas instead of individual textures
namespace Ultra {

// Helpers
inline uint32_t DecodeUtf8(string_view::iterator &begin, string_view::iterator end) {
    unsigned char byte = *begin;
    uint32_t codepoint = 0;
    int additionalBytes = 0;

    // 1-Byte Character (ASCII)
    if (byte <= 0x7F) {
        codepoint = byte;
    // 2-Byte Character
    } else if ((byte & 0xE0) == 0xC0) {
        codepoint = byte & 0x1F;
        additionalBytes = 1;
    // 3-Byte Character
    } else if ((byte & 0xF0) == 0xE0) {
        codepoint = byte & 0x0F;
        additionalBytes = 2;
    // 4-Byte Character
    } else if ((byte & 0xF8) == 0xF0) {
        codepoint = byte & 0x07;
        additionalBytes = 3;
    // Invalid UTF-8 start byte
    } else {
        begin++;
        return 0xFFFD; // Unicode replacement character
    }

    begin++;

    while (additionalBytes > 0) {
        if (begin == end || (*begin & 0xC0) != 0x80) {
            // Premature end or invalid UTF-8 continuation byte
            return 0xFFFD; // Unicode replacement character
        }

        codepoint = (codepoint << 6) | (*begin & 0x3F);
        begin++;
        additionalBytes--;
    }

    return codepoint;
}


Font::Font(string_view name, uint32_t size) {
    if (!mLibrary) FT_Init_FreeType(&mLibrary);
    auto path = Resource::GetPath(PhyResourceType::Font, name.data());
    mData = CreateScope<FontData>();

    if (FT_New_Face(mLibrary, path.c_str(), 0, &mData->Handle)) {
        LogFatal("Failed to load font '{}' from {}!", name, path);
        mData.reset();
        return;
    }
    FT_Set_Pixel_Sizes(mData->Handle, 0, size);
}

Font::~Font() {
}


int Font::GetLineHeight() {
    return mData->Handle->size->metrics.height >> 6;
}

Size Font::GetSize(string_view text) {
    Size size {};

    auto begin = text.begin();
    auto end = text.end();
    uint32_t lastGlyph = 0;
    while (begin != end) {
        uint32_t codepoint = DecodeUtf8(begin, end);
        auto *glyph = GetGlyph(codepoint);
        if (!glyph) {
            lastGlyph = 0;
            continue;
        }

        if (lastGlyph) size.Width += GetKerning(lastGlyph, glyph->UniqueID);
        lastGlyph = glyph->UniqueID;

        size.Width += glyph->Advance;
        size.Height = std::max(size.Height, (float)-glyph->Y + 1.0f);
    }
    return size;
}

FontSize Font::GetSizeFull(string_view text) {
    int x = 0;
    int y = 0;
    Vector2Di lower = { INT_MAX, INT_MAX };
    Vector2Di upper = { INT_MIN, INT_MIN };

    auto begin = text.begin();
    auto end = text.end();
    uint32_t lastGlyph = 0;

    while (begin != end) {
        uint32_t codepoint = DecodeUtf8(begin, end);
        auto *glyph = GetGlyph(codepoint);
        if (!glyph) {
            lastGlyph = 0;
            continue;
        }

        if (lastGlyph) x += GetKerning(lastGlyph, glyph->UniqueID);
        lastGlyph = glyph->UniqueID;

        lower.X = std::min(lower.X, x + glyph->X);
        lower.Y = std::min(lower.Y, y + glyph->Y);
        upper.X = std::max(upper.X, x + (glyph->X + glyph->Width));
        upper.Y = std::max(upper.Y, y + (glyph->Y + glyph->Height));
        x += glyph->Advance;
    }

    return { (float)lower.X, (float)lower.Y, (float)upper.X - lower.X, (float)upper.Y - lower.Y };
}


Glyph *Font::GetGlyph(uint32_t codepoint) {
    if (codepoint < 256 && mData->AsciiGlyphs[codepoint]) return mData->AsciiGlyphs[codepoint].get();
    Glyph *hashed = mData->Glyphs[codepoint].get();
    if (hashed) return hashed;

    FT_Face face = mData->Handle;
    int glyph = FT_Get_Char_Index(face, codepoint);
    if (glyph == 0) return 0;
    if (FT_Load_Glyph(face, glyph, FT_LOAD_FORCE_AUTOHINT | FT_LOAD_RENDER)) return 0;

    FT_Bitmap const *bitmap = &face->glyph->bitmap;
    unsigned char const *pBitmap = bitmap->buffer;

    // Create a new glyph and fill out metrics
    auto g = CreateScope<Glyph>();
    g->UniqueID = glyph;
    g->Advance = face->glyph->advance.x >> 6;
    g->Width = bitmap->width;
    g->Height = bitmap->rows;
    g->X = face->glyph->bitmap_left;
    g->Y = -face->glyph->bitmap_top;
    
    vector<Vector4Df> buffer {};
    buffer.reserve(g->Width * g->Height);

    // Copy rendered bitmap into buffer
    for (uint32_t dy = 0; dy < bitmap->rows; ++dy) {
        for (uint32_t dx = 0; dx < bitmap->width; ++dx) {
            float a = std::pow((float)(pBitmap[dx]) / 255.0f, kRcpGamma);
            buffer.push_back({ 1.0f, 1.0f, 1.0f, a });
        }
        pBitmap += bitmap->pitch;
    }

    // Upload to texture
    g->Texture = Texture::Create({ (uint32_t)g->Width, (uint32_t)g->Height, TextureFormat::RGBA8, TextureDataType::Float }, buffer.data(), sizeof(buffer.data()) * sizeof(float));

    // Add to glyph cache
    auto result = g.get();
    if (codepoint < 256) {
        mData->AsciiGlyphs[codepoint] = std::move(g);
    } else {
        mData->Glyphs[codepoint] = std::move(g);
    }
    return result;
}

int32_t Font::GetKerning(uint32_t leftGlyph, uint32_t rightGlyph) {
    FT_Vector kern {};
    FT_Get_Kerning(mData->Handle, leftGlyph, rightGlyph, FT_KERNING_DEFAULT, &kern);
    return kern.x >> 6;
}

}

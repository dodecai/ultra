module;

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_BITMAP_H

module Ultra.Engine.Font;

import Ultra.Engine.UIRenderer;
import Ultra.Engine.Resource;

// ToDo: Use Atlas instead of individual textures
namespace Ultra {

// Helpers
uint32_t DecodeUtf8(string_view::iterator &begin, string_view::iterator end) {
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

void Font::Draw(string_view text, float x, float y, float r, float g, float b, float a) {
    int glyphLast = 0;
    x = std::floor(x);
    y = std::floor(y);

    if (text.find("Behold") != string_view::npos) {
        auto test = text;
    }

    auto begin = text.begin();
    auto end = text.end();

    while (begin != end) {
        uint32_t codepoint = DecodeUtf8(begin, end);
        Glyph *glyph = GetGlyph(codepoint);
        if (glyph) {
            if (glyphLast) x += GetKerning(glyphLast, glyph->index);
            float x0 = (float)(x + glyph->x0);
            float y0 = (float)(y + glyph->y0);
            float x1 = (float)(x + glyph->x1);
            float y1 = (float)(y + glyph->y1);

            UIDraw::DrawText(glyph->Texture, 0, x0, y0, x1, y1, { r, g, b, a });

            x += glyph->advance;
            glyphLast = glyph->index;
        } else {
            glyphLast = 0;
        }
    }
}

int Font::GetLineHeight() {
    return mData->Handle->size->metrics.height >> 6;
}

///
/// @note: The height returned here is the maximal *ascender* height for the string. This allows easy centering of text while still allowing descending characters to look correct.
/// To correctly center text, first compute bounds via this function, then draw it at:
/// pos.x - (size.x - bound.x) / 2
/// pos.y - (size.y + bound.y) / 2
///
FontSize Font::GetSize(const char *text) {
    FontSize size {};

    int glyphLast = 0;
    uint32_t codepoint = *text++;
    while (codepoint) {
        Glyph *glyph = GetGlyph(codepoint);
        if (glyph) {
            if (glyphLast)
                size.X += GetKerning(glyphLast, glyph->index);
            size.X += glyph->advance;
            size.Y = std::max(size.Y, -glyph->y0 + 1);
            glyphLast = glyph->index;
        } else {
            glyphLast = 0;
        }
        codepoint = *text++;
    }
    return size;
}

FontSize Font::GetSizeFull(const char *text) {
    int x = 0;
    int y = 0;
    Vector2Di lower = { INT_MAX, INT_MAX };
    Vector2Di upper = { INT_MIN, INT_MIN };

    int glyphLast = 0;
    uint32_t codepoint = *text++;
    if (!codepoint) {
        return {};
    }

    while (codepoint) {
        Glyph *glyph = GetGlyph(codepoint);
        if (glyph) {
            if (glyphLast)
                x += GetKerning(glyphLast, glyph->index);
            lower.x = std::min(lower.x, x + glyph->x0);
            lower.y = std::min(lower.y, y + glyph->y0);
            upper.x = std::max(upper.x, x + glyph->x1);
            upper.y = std::max(upper.y, y + glyph->y1);
            x += glyph->advance;
            glyphLast = glyph->index;
        } else {
            glyphLast = 0;
        }
        codepoint = *text++;
    }

    return { lower.x, lower.y, upper.x - lower.x, upper.y - lower.y };
}


Glyph *Font::GetGlyph(uint32_t character) {
    if (character < 256 && mData->AsciiGlyphs[character]) return mData->AsciiGlyphs[character];

    Glyph *g = mData->Glyphs[character];
    if (g) return g;

    FT_Face face = mData->Handle;
    int glyph = FT_Get_Char_Index(face, character);
    if (glyph == 0) return 0;
    if (FT_Load_Glyph(face, glyph, FT_LOAD_FORCE_AUTOHINT | FT_LOAD_RENDER)) return 0;

    FT_Bitmap const *bitmap = &face->glyph->bitmap;
    unsigned char const *pBitmap = bitmap->buffer;

    // Create a new glyph and fill out metrics
    {
        g = new Glyph();
        g->index = glyph;
        g->advance = face->glyph->advance.x >> 6;
        g->x0 = face->glyph->bitmap_left;
        g->y0 = -face->glyph->bitmap_top;
        g->sx = bitmap->width;
        g->sy = bitmap->rows;
        g->x1 = g->x0 + g->sx;
        g->y1 = g->y0 + g->sy;
    }

    vector<Vector4D<float>> buffer {};
    buffer.reserve(g->sx * g->sy);

    // Copy rendered bitmap into buffer
    {
        for (uint32_t dy = 0; dy < bitmap->rows; ++dy) {
            for (uint32_t dx = 0; dx < bitmap->width; ++dx) {
                float a = std::pow((float)(pBitmap[dx]) / 255.0f, kRcpGamma);
                buffer.push_back({ 1.0f, 1.0f, 1.0f, a });
            }
            pBitmap += bitmap->pitch;
        }
    }

    // Upload to texture
    {
        g->Texture = Texture::Create({ (uint32_t)g->sx, (uint32_t)g->sy, TextureFormat::RGBA8, TextureDataType::Float }, buffer.data(), sizeof(buffer.data()) * sizeof(float));
    }


    // Add to glyph cache
    if (character < 256) {
        mData->AsciiGlyphs[character] = g;
    } else {
        //HashMap_Set(self->Glyphs, &character, g);
        mData->Glyphs[character] = g;
    }
    return g;
}

int Font::GetKerning(int a, int b) {
    FT_Vector kern;
    FT_Get_Kerning(mData->Handle, a, b, FT_KERNING_DEFAULT, &kern);
    return kern.x >> 6;
}

}

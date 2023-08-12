module;

#include "Vec4.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_BITMAP_H

module Ultra.Engine.Font;

import Ultra.Engine.UIRenderer;
import Ultra.Engine.Resource;

/* TODO : Re-implement UTF-8 support */
/* TODO : Atlas instead of individual textures. */

namespace Ultra {

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



FontData *Font::Load(string_view name, int size) {
    if (!mLibrary) FT_Init_FreeType(&mLibrary);

    string path = Resource::GetPath(PhyResourceType::Font, name.data());
    FontData *data = new FontData();

    if (FT_New_Face(mLibrary, path.c_str(), 0, &data->Handle)) LogFatal("Font_Load: Failed to load font <%s> at <%s>", name, path);
    FT_Set_Pixel_Sizes(data->Handle, 0, size);

    memset(data->AsciiGlyphs, 0, sizeof(data->AsciiGlyphs));
    return data;
}

void Font::Free(FontData *self) {
    {
      /* TODO : Free Glyphs! */
        FT_Done_Face(self->Handle);
        delete self;
    }
}


void Font::Draw(FontData *self, const char * text, float x, float y, float r, float g, float b, float a) {
    int glyphLast = 0;
    uint32_t codepoint = *text++;
    x = std::floor(x);
    y = std::floor(y);

    while (codepoint) {
        Glyph *glyph = GetGlyph(self, codepoint);
        if (glyph) {
            if (glyphLast) x += GetKerning(self, glyphLast, glyph->index);
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
        codepoint = *text++;
    }
}

int Font::GetLineHeight(FontData *self) {
    return self->Handle->size->metrics.height >> 6;
}

void Font::GetSize(FontData *self, Vector4Di *out, const char * text) {
    int x = 0;
    int y = 0;
    Vector2Di lower = { INT_MAX, INT_MAX };
    Vector2Di upper = { INT_MIN, INT_MIN };

    int glyphLast = 0;
    uint32_t codepoint = *text++;
    if (!codepoint) {
        *out = { 0, 0, 0, 0 };
        return;
    }

    while (codepoint) {
        Glyph *glyph = GetGlyph(self, codepoint);
        if (glyph) {
            if (glyphLast)
                x += GetKerning(self, glyphLast, glyph->index);
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

    *out = { lower.x, lower.y, upper.x - lower.x, upper.y - lower.y };
}

///
/// @note: The height returned here is the maximal *ascender* height for the string. This allows easy centering of text while still allowing descending characters to look correct.
/// To correctly center text, first compute bounds via this function, then draw it at:
/// pos.x - (size.x - bound.x) / 2
/// pos.y - (size.y + bound.y) / 2
///
void Font::GetSize2(FontData *self, Vector2Di *out, const char * text) {
    out->x = 0;
    out->y = 0;

    int glyphLast = 0;
    uint32_t codepoint = *text++;
    while (codepoint) {
        Glyph *glyph = GetGlyph(self, codepoint);
        if (glyph) {
            if (glyphLast)
                out->x += GetKerning(self, glyphLast, glyph->index);
            out->x += glyph->advance;
            out->y = std::max(out->y, -glyph->y0 + 1);
            glyphLast = glyph->index;
        } else {
            glyphLast = 0;
        }
        codepoint = *text++;
    }
}


Glyph *Font::GetGlyph(FontData *self, uint32_t codepoint) {
    if (codepoint < 256 && self->AsciiGlyphs[codepoint])
        return self->AsciiGlyphs[codepoint];

    //Glyph *g = (Glyph *)HashMap_Get(self->Glyphs, &codepoint);
    Glyph *g = self->Glyphs[codepoint];
    if (g) return g;

    FT_Face face = self->Handle;
    int glyph = FT_Get_Char_Index(face, codepoint);
    if (glyph == 0)
        return 0;
    if (FT_Load_Glyph(face, glyph, FT_LOAD_FORCE_AUTOHINT | FT_LOAD_RENDER))
        return 0;

    FT_Bitmap const *bitmap = &face->glyph->bitmap;
    unsigned char const *pBitmap = bitmap->buffer;

    /* Create a new glyph and fill out metrics. */ {
        g = new Glyph();
        g->index = glyph;
        g->x0 = face->glyph->bitmap_left;
        g->y0 = -face->glyph->bitmap_top;
        g->sx = bitmap->width;
        g->sy = bitmap->rows;
        g->x1 = g->x0 + g->sx;
        g->y1 = g->y0 + g->sy;
        g->advance = face->glyph->advance.x >> 6;
    }

    vector<Vec4f> buffer(g->sx * g->sy);
    vector<Vector4Df> buffer2(g->sx * g->sy);

    /* Copy rendered bitmap into buffer. */ {
        Vec4f *pBuffer = buffer.data();
        Vector4Df *pBuffer2 = buffer2.data();
        for (uint32_t dy = 0; dy < bitmap->rows; ++dy) {
            for (uint32_t dx = 0; dx < bitmap->width; ++dx) {
                float a = std::pow((float)(pBitmap[dx]) / 255.0f, kRcpGamma);
                *pBuffer++ = Vec4f_Create(1.0f, 1.0f, 1.0f, a);
                *pBuffer2++ = { 1.0f, 1.0f, 1.0f, a };
            }
            pBitmap += bitmap->pitch;
        }
    }

    /* Upload to texture. */ {
        g->Texture = Texture::Create({ (uint32_t)g->sx, (uint32_t)g->sy, TextureFormat::RGBA8, TextureDataType::Float }, buffer.data(), sizeof(buffer.data()));
    }


    /* Add to glyph cache. */
    if (codepoint < 256) {
        self->AsciiGlyphs[codepoint] = g;
    } else {
        //HashMap_Set(self->Glyphs, &codepoint, g);
        self->Glyphs[codepoint] = g;
    }
    return g;
}

int Font::GetKerning(FontData *self, int a, int b) {
    FT_Vector kern;
    FT_Get_Kerning(self->Handle, a, b, FT_KERNING_DEFAULT, &kern);
    return kern.x >> 6;
}

}

module;

#include "Vec4.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_BITMAP_H

// ToDo: Check Profiler
#define FRAME_BEGIN
#define FRAME_END

module Ultra.Engine.Font;

import Ultra.Engine.Renderer.Texture;
import Ultra.Engine.UIRenderer;
import Ultra.Engine.Resource;

namespace Ultra {

Font::Font(const string &font) {
}

Font::~Font() {
}

/* TODO : Re-implement UTF-8 support */
/* TODO : Atlas instead of individual textures. */

struct Glyph {
    size_t index;

    int32_t x0;
    int32_t y0;
    int32_t x1;
    int32_t y1;
    
    int32_t sx;
    int32_t sy;

    int32_t advance;

    Reference<Texture2D> Texture;
};

struct FontData {
    FT_Face Handle;

    Glyph *AsciiGlyphs[256];
    unordered_map<uint32_t, Glyph *> Glyphs;
};


FontData *Font::Load(string_view name, int size) {
    if (!mLibrary) FT_Init_FreeType(&mLibrary);

    string path = Resource::GetPath(PhyResourceType::Font, name.data());
    FontData *data = new FontData();

    if (FT_New_Face(mLibrary, path.c_str(), 0, &data->Handle)) LogFatal("Font_Load: Failed to load font <%s> at <%s>", name, path);
    FT_Set_Pixel_Sizes(data->Handle, 0, size);

    memset(data->AsciiGlyphs, 0, sizeof(data->AsciiGlyphs));
    //self->Glyphs = HashMap_Create(sizeof(uint32_t), 16);
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
    FRAME_BEGIN;
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

    FRAME_END;
}

int Font::GetLineHeight(FontData *self) {
    return self->Handle->size->metrics.height >> 6;
}

void Font::GetSize(FontData *self, Vector4Di *out, const char * text) {
    FRAME_BEGIN;
    int x = 0, y = 0;
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
    FRAME_END;
}

///
/// @note: The height returned here is the maximal *ascender* height for the string. This allows easy centering of text while still allowing descending characters to look correct.
/// To correctly center text, first compute bounds via this function, then draw it at:
/// pos.x - (size.x - bound.x) / 2
/// pos.y - (size.y + bound.y) / 2
///
void Font::GetSize2(FontData *self, Vector2Di *out, const char * text) {
    FRAME_BEGIN;
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

    FRAME_END;
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

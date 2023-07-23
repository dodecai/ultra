module;

// ToDo: Check Profiler
#define FRAME_BEGIN
#define FRAME_END

//#include "HashMap.h"
#include "Vec2.h"
#include "Vec4.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_BITMAP_H

module Ultra.Engine.Font;

import Ultra.Engine.Phoenix;

namespace Ultra {

/* TODO : Re-implement UTF-8 support */
/* TODO : Atlas instead of individual textures. */

/* NOTE : Gamma of 1.8 recommended by FreeType */
const float kGamma = 1.8f;
const float kRcpGamma = 1.0f / kGamma;

struct Glyph {
    int index;
    Tex2DData *tex;
    int x0, y0, x1, y1;
    int sx, sy;
    int advance;
};

struct FontData {
    FT_Face handle;
    //HashMap *glyphs;
    unordered_map<uint32_t, Glyph *> glyphs;
    Glyph *glyphsAscii[256];
};

static FT_Library ft = 0;

static Glyph *Font_GetGlyph(FontData *self, uint32 codepoint) {
    if (codepoint < 256 && self->glyphsAscii[codepoint])
        return self->glyphsAscii[codepoint];

    //Glyph *g = (Glyph *)HashMap_Get(self->glyphs, &codepoint);
    Glyph *g = self->glyphs[codepoint];
    if (g) return g;

    FT_Face face = self->handle;
    int glyph = FT_Get_Char_Index(face, codepoint);
    if (glyph == 0)
        return 0;
    if (FT_Load_Glyph(face, glyph, FT_LOAD_FORCE_AUTOHINT | FT_LOAD_RENDER))
        return 0;

    FT_Bitmap const *bitmap = &face->glyph->bitmap;
    uchar const *pBitmap = bitmap->buffer;

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

    vector<Vec4f> buffer (g->sx * g->sy);

    /* Copy rendered bitmap into buffer. */ {
        Vec4f *pBuffer = buffer.data();
        for (uint dy = 0; dy < bitmap->rows; ++dy) {
            for (uint dx = 0; dx < bitmap->width; ++dx) {
                float a = Pow((float)(pBitmap[dx]) / 255.0f, kRcpGamma);
                *pBuffer++ = Vec4f_Create(1.0f, 1.0f, 1.0f, a);
            }
            pBitmap += bitmap->pitch;
        }
    }

    /* Upload to texture. */ {
        g->tex = Tex2D::Create(g->sx, g->sy, PhxTextureFormat::RGBA8);
        Tex2D::SetData(g->tex, buffer.data(), PhxPixelFormat::RGBA, PhxDataFormat::Float);
    }


    /* Add to glyph cache. */
    if (codepoint < 256) {
        self->glyphsAscii[codepoint] = g;
    } else {
        //HashMap_Set(self->glyphs, &codepoint, g);
        self->glyphs[codepoint] = g;
    }
    return g;
}

inline static int Font_GetKerning(FontData *self, int a, int b) {
    FT_Vector kern;
    FT_Get_Kerning(self->handle, a, b, FT_KERNING_DEFAULT, &kern);
    return kern.x >> 6;
}

FontData *Font::Load(cstr name, int size) {
    if (!ft) FT_Init_FreeType(&ft);

    string path = Resource::GetPath(PhyResourceType::Font, name);
    FontData *self = new FontData();

    if (FT_New_Face(ft, path.c_str(), 0, &self->handle)) Fatal("Font_Load: Failed to load font <%s> at <%s>", name, path);
    FT_Set_Pixel_Sizes(self->handle, 0, size);

    memset(self->glyphsAscii, 0, sizeof(self->glyphsAscii));
    //self->glyphs = HashMap_Create(sizeof(uint32), 16);
    return self;
}

void Font::Acquire(FontData *self) {
    //RefCounted_Acquire(self);
}

void Font::Free(FontData *self) {
    {
      /* TODO : Free glyphs! */
        FT_Done_Face(self->handle);
        delete self;
    }
}

void Font::Draw(FontData *self, cstr text, float x, float y, float r, float g, float b, float a) {
    FRAME_BEGIN;
    int glyphLast = 0;
    uint32 codepoint = *text++;
    x = Floor(x);
    y = Floor(y);
    RenderState::PushBlendMode(PhxBlendMode::Alpha);
    Draw::Color(r, g, b, a);

    while (codepoint) {
        Glyph *glyph = Font_GetGlyph(self, codepoint);
        if (glyph) {
            if (glyphLast)
                x += Font_GetKerning(self, glyphLast, glyph->index);
            float x0 = (float)(x + glyph->x0);
            float y0 = (float)(y + glyph->y0);
            float x1 = (float)(x + glyph->x1);
            float y1 = (float)(y + glyph->y1);
            Tex2D::DrawEx(glyph->tex, x0, y0, x1, y1, 0, 0, 1, 1);
            x += glyph->advance;
            glyphLast = glyph->index;
        } else {
            glyphLast = 0;
        }
        codepoint = *text++;
    }

    Draw::Color(1, 1, 1, 1);
    RenderState::PopBlendMode();
    FRAME_END;
}

void Font::DrawShaded(FontData *self, cstr text, float x, float y) {
    FRAME_BEGIN;
    int glyphLast = 0;
    uint32 codepoint = *text++;
    x = Floor(x);
    y = Floor(y);

    while (codepoint) {
        Glyph *glyph = Font_GetGlyph(self, codepoint);
        if (glyph) {
            if (glyphLast)
                x += Font_GetKerning(self, glyphLast, glyph->index);
            float x0 = (float)(x + glyph->x0);
            float y0 = (float)(y + glyph->y0);
            float x1 = (float)(x + glyph->x1);
            float y1 = (float)(y + glyph->y1);
            PhxShader::SetTex2D("glyph", glyph->tex);
            Tex2D::DrawEx(glyph->tex, x0, y0, x1, y1, 0, 0, 1, 1);
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
    return self->handle->size->metrics.height >> 6;
}

void Font::GetSize(FontData *self, Vector4Di *out, cstr text) {
    FRAME_BEGIN;
    int x = 0, y = 0;
    Vec2i lower = { INT_MAX, INT_MAX };
    Vec2i upper = { INT_MIN, INT_MIN };

    int glyphLast = 0;
    uint32 codepoint = *text++;
    if (!codepoint) {
        *out = { 0, 0, 0, 0 };
        return;
    }

    while (codepoint) {
        Glyph *glyph = Font_GetGlyph(self, codepoint);
        if (glyph) {
            if (glyphLast)
                x += Font_GetKerning(self, glyphLast, glyph->index);
            lower.x = Min(lower.x, x + glyph->x0);
            lower.y = Min(lower.y, y + glyph->y0);
            upper.x = Max(upper.x, x + glyph->x1);
            upper.y = Max(upper.y, y + glyph->y1);
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

/* 
 * NOTE: The height returned here is the maximal *ascender* height for the string. This allows easy centering of text while still allowing descending characters to look correct.
 *
 *        To correctly center text, first compute bounds via this function,
 *        then draw it at:
 *
 *           pos.x - (size.x - bound.x) / 2
 *           pos.y - (size.y + bound.y) / 2
 */

void Font::GetSize2(FontData *self, Vector2Di *out, cstr text) {
    FRAME_BEGIN;
    out->x = 0;
    out->y = 0;

    int glyphLast = 0;
    uint32 codepoint = *text++;
    while (codepoint) {
        Glyph *glyph = Font_GetGlyph(self, codepoint);
        if (glyph) {
            if (glyphLast)
                out->x += Font_GetKerning(self, glyphLast, glyph->index);
            out->x += glyph->advance;
            out->y = Max(out->y, -glyph->y0 + 1);
            glyphLast = glyph->index;
        } else {
            glyphLast = 0;
        }
        codepoint = *text++;
    }

    FRAME_END;
}

}

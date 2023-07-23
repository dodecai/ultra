export module Ultra.Engine.Font;

import Ultra.Core;
import Ultra.Math;

export namespace Ultra {

/* --- Font --------------------------------------------------------------------
 *
 *   This type is REFERENCE-COUNTED. See ../doc/RefCounted.txt for details.
 *
 *   Font_DrawShaded : In addition to drawing glyphs as textured quads,
 *                     DrawShaded binds the texture for the current glyph to
 *                     the uniform sampler2D "glyph" of the currently-active
 *                     shader. This allows for text to be rendered through
 *                     the shader pipeline and, for example, output HDR values
 *                     for glyph colors.
 *
 * -------------------------------------------------------------------------- */

struct FontData;

class Font {
public:
    static FontData *Load(const char *name, int size);
    static void Acquire(FontData *);
    static void Free(FontData *);

    static void Draw(FontData *, const char *text, float x, float y, float r, float g, float b, float a);
    static void DrawShaded(FontData *, const char *text, float x, float y);

    static int GetLineHeight(FontData *);

    static void GetSize(FontData *, Vector4Di *out, const char *text);
    static void GetSize2(FontData *, Vector2Di *out, const char *text);
};


}

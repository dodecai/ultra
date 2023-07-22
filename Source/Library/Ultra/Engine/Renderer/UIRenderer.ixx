export module Ultra.Engine.UIRenderer;

import Ultra.Core;
import Ultra.Engine.Phoenix;
export import Ultra.Engine.Font;

export namespace Ultra {

class UIRenderer {
public:
    static void Begin();
    static void End();
    static void Draw();
    static void BeginLayer(float x, float y, float sx, float sy, bool clip);
    static void EndLayer();
    static void Image(Tex2DData *, float x, float y, float sx, float sy);
    static void Panel(float x, float y, float sx, float sy, float r, float g, float b, float a, float bevel, float innerAlpha);
    static void Rect(float x, float y, float sx, float sy, float r, float g, float b, float a, bool outline);
    static void Text(FontData *font, const char  *text, float x, float y, float r, float g, float b, float a);
};

}

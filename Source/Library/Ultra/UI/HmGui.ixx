module;

#include "Button.h"
#include "Hash.h"
#include "HashMap.h"
#include "Input.h"
#include "Profiler.h"
#include "Vec2.h"
#include "Vec4.h"

export module Ultra.UI.HmGui;

import Ultra.Core;
import Ultra.Engine.Phoenix;
import Ultra.Engine.UIRenderer;
export import Ultra.Engine.Font;

export namespace Ultra::UI {

#define HMGUI_DRAW_GROUP_FRAMES 0
#define HMGUI_ENABLE_DRAGGING 1

///
/// @brief 
///
enum class Layout {
    None        = 0,
    Stack       = 1,
    Vertical    = 2,
    Horizontal  = 3,
};

enum class WidgetType {
    Group   = 0,
    Text    = 1,
    Rect    = 2,
    Image   = 3,
};

enum class FocusStyle {
    None        = 0,
    Fill        = 1,
    Outline     = 2,
    Underline   = 3,
};

enum class FocusMouse {
    Mouse       = 0,
    Scroll      = 1,
    Size        = 2,
};

///
/// @brief 
///
struct HmGuiWidget {
    struct HmGuiGroup *parent;
    HmGuiWidget *next;
    HmGuiWidget *prev;
    uint64 hash;
    WidgetType type;
    Vec2f pos;
    Vec2f size;
    Vec2f minSize;
    Vec2f align;
    Vec2f stretch;
};

struct HmGuiGroup: public HmGuiWidget {
    HmGuiWidget *head;
    HmGuiWidget *tail;
    Layout layout;
    uint32 children;
    FocusStyle focusStyle;
    Vec2f paddingLower;
    Vec2f paddingUpper;
    Vec2f offset;
    Vec2f maxSize;
    Vec2f totalStretch;
    float spacing;
    float frameOpacity;
    bool clip;
    bool expand;
    bool focusable[(int)FocusMouse::Size];
    bool storeSize;
};

struct HmGuiText: public HmGuiWidget {
    FontData *font;
    string text;
    Vec4f color;
};

struct HmGuiRect: public HmGuiWidget {
    Vec4f color;
};

struct HmGuiImage: public HmGuiWidget {
    Tex2DData *image;
};

struct HmGuiClipRect {
    HmGuiClipRect *prev;
    Vec2f lower;
    Vec2f upper;
};

struct HmGuiData {
    Vec2f offset;
    Vec2f minSize;
    Vec2f size;
};

struct HmGuiStyle {
    HmGuiStyle *prev;
    FontData *font;
    float spacing;
    Vec4f colorPrimary;
    Vec4f colorFrame;
    Vec4f colorText;
};

///
/// @brief 
///
struct HmGuiDataStructure {
    HmGuiGroup *group;
    HmGuiGroup *root;
    HmGuiWidget *last;
    HmGuiStyle *style;
    HmGuiClipRect *clipRect;
    HashMap *data;

    uint64 focus[(int)FocusMouse::Size];
    Vec2f focusPos;
    bool activate;
};

class HmGui {
public:
    HmGui() = default;
    ~HmGui() = default;

    static void Begin(float sx, float sy) {
        static bool init = false;
        if (!init) {
            init = true;
            self.group = 0;
            self.root = 0;

            self.style = new HmGuiStyle();
            self.style->prev = 0;
            self.style->font = Font::Load("Rajdhani", 14);
            self.style->spacing = 6;

            self.style->colorPrimary = Vec4f_Create(0.1f, 0.5f, 1.0f, 1.0f);
            self.style->colorFrame = Vec4f_Create(0.1f, 0.1f, 0.1f, 0.5f);
            self.style->colorText = Vec4f_Create(1, 1, 1, 1);

            self.clipRect = 0;
            self.data = HashMap_Create(0, 128);

            for (int i = 0; i < (int)FocusMouse::Size; ++i) self.focus[i] = 0;
            self.activate = false;
        }

        if (self.root) {
            FreeGroup(self.root);
            self.root = 0;
        }
        self.last = 0;
        self.activate = Input_GetPressed(Button_Mouse_Left);

        BeginGroup(Layout::None);
        self.group->clip = true;
        self.group->pos = Vec2f_Create(0, 0);
        self.group->size = Vec2f_Create(sx, sy);
        self.root = self.group;
    }
    static void End() {
        FRAME_BEGIN;
        EndGroup();
        ComputeSize(self.root);
        LayoutGroup(self.root);

        for (int i = 0; i < (int)FocusMouse::Size; ++i) self.focus[i] = 0;
        Vec2i mouse; Input_GetMousePosition(&mouse);
        self.focusPos = Vec2f_Create(mouse.x, mouse.y);
        CheckFocus(self.root);
        FRAME_END;
    }
    static void Draw() {
        FRAME_BEGIN;
        RenderState::PushBlendMode(PhxBlendMode::Alpha);
        UIRenderer::Begin();
        DrawGroup(self.root);
        UIRenderer::End();
        RenderState::PopBlendMode();
        UIRenderer::Draw();
        FRAME_END;
    }

    static void BeginGroupX() {
        BeginGroup(Layout::Horizontal);
    }
    static void BeginGroupY() {
        BeginGroup(Layout::Vertical);
    }
    static void BeginGroupStack() {
        BeginGroup(Layout::Stack);
    }
    static void EndGroup() {
        self.last = self.group;
        self.group = self.group->parent;
    }
    static void BeginScroll(float maxSize) {
        BeginGroupX();
        SetStretch(1, 1);
        self.group->clip = true;
        SetSpacing(2);
        BeginGroupY();
        SetPadding(6, 6);
        SetStretch(1, 1);
        self.group->expand = false;
        self.group->storeSize = true;
        self.group->maxSize.y = maxSize;

        HmGuiData *data = GetData(self.group);
        self.group->offset.y = -data->offset.y;
    }
    static void EndScroll() {
        HmGuiData *data = GetData(self.group);
        if (GroupHasFocus(FocusMouse::Scroll)) {
            Vec2i scroll; Input_GetMouseScroll(&scroll);
            data->offset.y -= 10.0f * scroll.y;
        }

        float maxScroll = Max(0.0f, data->minSize.y - data->size.y);
        data->offset.y = Clamp(data->offset.y, 0.0f, maxScroll);

        EndGroup();

        BeginGroupY();
        SetStretch(0, 1);
        SetSpacing(0);
        if (maxScroll > 0) {
            float handleSize = data->size.y * (data->size.y / data->minSize.y);
            float handlePos = Lerp(0.0f, data->size.y - handleSize, data->offset.y / maxScroll);
            Rect(4.0f, handlePos, 0.0f, 0.0f, 0.0f, 0.0f);
            Rect(4.0f, handleSize, UNPACK4(self.style->colorFrame));
        } else {
            Rect(4.0f, 16.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        }
        EndGroup();
        EndGroup();
    }
    static void BeginWindow(cstr title) {
        BeginGroupStack();
        SetStretch(0, 0);
        self.group->focusStyle = FocusStyle::None;
        self.group->frameOpacity = 0.95f;
        #if HMGUI_ENABLE_DRAGGING
        HmGuiData *data = GetData(self.group);
        if (GroupHasFocus(FocusMouse::Mouse)) {
            if (Input_GetDown(Button_Mouse_Left)) {
                Vec2i md; Input_GetMouseDelta(&md);
                data->offset.x += md.x;
                data->offset.y += md.y;
            }
        }

        self.group->pos.x += data->offset.x;
        self.group->pos.y += data->offset.y;
        #endif

        BeginGroupY();
        self.group->clip = true;
        SetPadding(8, 8);
        SetStretch(1, 1);
        // TextColored(title, 1.0f, 1.0f, 1.0f, 0.3f);
        // SetAlign(0.5f, 0.0f);
    }
    static void EndWindow() {
        EndGroup();
        EndGroup();
    }

    static bool Button(cstr label) {
        BeginGroupStack();
        self.group->focusStyle = FocusStyle::Fill;
        self.group->frameOpacity = 0.5f;
        bool focus = GroupHasFocus(FocusMouse::Mouse);
        SetPadding(8, 8);
        Text(label);
        SetAlign(0.5f, 0.5f);
        EndGroup();
        return focus && self.activate;
    }
    static bool Checkbox(cstr label, bool value) {
        BeginGroupX();
        self.group->focusStyle = FocusStyle::Underline;
        if (GroupHasFocus(FocusMouse::Mouse) && self.activate)
            value = !value;
        SetPadding(4, 4);
        SetSpacing(8);
        SetStretch(1, 0);

        Text(label);
        SetAlign(0.0f, 0.5f);
        SetStretch(1, 0);

        BeginGroupStack();
        Rect(16, 16, UNPACK4(self.style->colorFrame));
        if (value) {
            Rect(10, 10, UNPACK4(self.style->colorPrimary));
            SetAlign(0.5f, 0.5f);
        }
        EndGroup();
        SetStretch(0, 0);
        EndGroup();
        return value;
    }
    static float Slider(float lower, float upper, float value) {
        BeginGroupStack();
        Rect(0, 2, 0.5f, 0.5f, 0.5f, 1.0f);
        SetAlign(0.5f, 0.5f);
        SetStretch(1, 0);
        EndGroup();
        return 0.0f;
    }

    static void Image(Tex2DData *image) {
        HmGuiImage *e = new HmGuiImage();
        InitWidget(e, WidgetType::Image);
        e->image = image;
        e->stretch = Vec2f_Create(1, 1);
    }
    static void Rect(float sx, float sy, float r, float g, float b, float a) {
        HmGuiRect *e = new HmGuiRect();
        InitWidget(e, WidgetType::Rect);
        e->color = Vec4f_Create(r, g, b, a);
        e->minSize = Vec2f_Create(sx, sy);
    }
    static void Text(cstr text) {
        TextEx(self.style->font, text, UNPACK4(self.style->colorText));
    }
    static void TextColored(cstr text, float r, float g, float b, float a) {
        TextEx(self.style->font, text, r, g, b, a);
    }
    static void TextEx(FontData *font, cstr text, float r, float g, float b, float a) {
        HmGuiText *e = new HmGuiText();
        InitWidget(e, WidgetType::Text);
        e->font = font;
        e->text = string(text);
        e->color = Vec4f_Create(r, g, b, a);
        Vec2i size; Font::GetSize2(e->font, &size, e->text.c_str());
        e->minSize = Vec2f_Create(size.x, size.y);
        SetAlign(0.0f, 1.0f);
    }

    static void SetAlign(float ax, float ay) {
        self.last->align = Vec2f_Create(ax, ay);
    }
    static void SetPadding(float px, float py) {
        self.group->paddingLower = Vec2f_Create(px, py);
        self.group->paddingUpper = Vec2f_Create(px, py);
    }
    static void SetPaddingEx(float left, float top, float right, float bottom) {
        self.group->paddingLower = Vec2f_Create(left, top);
        self.group->paddingUpper = Vec2f_Create(right, bottom);
    }
    static void SetPaddingLeft(float padding) {
        self.group->paddingLower.x = padding;
    }
    static void SetPaddingTop(float padding) {
        self.group->paddingLower.y = padding;
    }
    static void SetPaddingRight(float padding) {
        self.group->paddingUpper.x = padding;
    }
    static void SetPaddingBottom(float padding) {
        self.group->paddingUpper.y = padding;
    }
    static void SetSpacing(float spacing) {
        self.group->spacing = spacing;
    }
    static void SetStretch(float x, float y) {
        self.last->stretch = Vec2f_Create(x, y);
    }
    static bool GroupHasFocus(FocusMouse type) {
        self.group->focusable[(int)type] = true;
        return self.focus[(int)type] == self.group->hash;
    }

    static void PushStyle() {
        HmGuiStyle *style = new HmGuiStyle();
        *style = *self.style;
        style->prev = self.style;
        self.style = style;
    }
    static void PushFont(FontData *font) {
        PushStyle();
        self.style->font = font;
    }
    static void PushTextColor(float r, float g, float b, float a) {
        PushStyle();
        self.style->colorText = Vec4f_Create(r, g, b, a);
    }
    static void PopStyle(int depth) {
        for (int i = 0; i < depth; ++i) {
            HmGuiStyle *style = self.style;
            self.style = style->prev;
            delete style;
        }
    }

private:
    static void InitWidget(HmGuiWidget *e, WidgetType type) {
        e->parent = self.group;
        e->next = 0;
        e->prev = self.group ? self.group->tail : 0;

        if (e->parent) {
            e->parent->children++;
            e->hash = Hash_FNV64_Incremental(
              e->parent->hash,
              &e->parent->children,
              sizeof(e->parent->children));
            (e->next ? e->next->prev : e->parent->tail) = e;
            (e->prev ? e->prev->next : e->parent->head) = e;
        } else {
            e->hash = Hash_FNV64_Init();
        }

        e->type = type;
        e->pos = Vec2f_Create(0, 0);
        e->size = Vec2f_Create(0, 0);
        e->minSize = Vec2f_Create(0, 0);
        e->align = Vec2f_Create(0, 0);
        e->stretch = Vec2f_Create(0, 0);

        self.last = e;
    }
    static void BeginGroup(Layout layout) {
        HmGuiGroup *e = new HmGuiGroup();
        InitWidget(e, WidgetType::Group);
        e->head = 0;
        e->tail = 0;
        e->layout = layout;
        e->children = 0;
        e->focusStyle = FocusStyle::None;
        e->paddingLower = Vec2f_Create(0, 0);
        e->paddingUpper = Vec2f_Create(0, 0);
        e->offset = Vec2f_Create(0, 0);
        e->maxSize = Vec2f_Create(1e30f, 1e30f);
        e->spacing = self.style->spacing;
        e->frameOpacity = 0.0f;
        e->clip = false;
        e->expand = true;
        for (int i = 0; i < (int)FocusMouse::Size; ++i) e->focusable[i] = false;
        e->storeSize = false;
        self.group = e;

        switch (layout) {
            case Layout::Stack:
                e->stretch = Vec2f_Create(1, 1);
                break;
            case Layout::Vertical:
                e->stretch = Vec2f_Create(1, 0);
                break;
            case Layout::Horizontal:
                e->stretch = Vec2f_Create(0, 1);
                break;
        }
    }
    static void FreeText(HmGuiText *e) {
        delete e;
    }
    static void FreeGroup(HmGuiGroup *g) {
        HmGuiWidget *e = g->head;
        while (e) {
            HmGuiWidget *next = e->next;
            switch (e->type) {
                case WidgetType::Group: FreeGroup((HmGuiGroup *)e); break;
                case WidgetType::Text: FreeText((HmGuiText *)e); break;
                default: delete e; break;
            }
            e = next;
        }
        delete g;
    }

    static HmGuiData *GetData(HmGuiGroup *g) {
        HmGuiData *data = (HmGuiData *)HashMap_GetRaw(self.data, g->hash);
        if (!data) {
            data = new HmGuiData();
            data->offset = Vec2f_Create(0, 0);
            data->minSize = Vec2f_Create(0, 0);
            data->size = Vec2f_Create(0, 0);
            HashMap_SetRaw(self.data, g->hash, data);
        }
        return data;
    }

    static void PushClipRect(HmGuiGroup *g) {
        HmGuiClipRect *rect = new HmGuiClipRect();
        rect->prev = self.clipRect;
        rect->lower = g->pos;
        rect->upper = Vec2f_Add(g->pos, g->size);
        if (rect->prev) {
            rect->lower.x = Max(rect->lower.x, rect->prev->lower.x);
            rect->lower.y = Max(rect->lower.y, rect->prev->lower.y);
            rect->upper.x = Min(rect->upper.x, rect->prev->upper.x);
            rect->upper.y = Min(rect->upper.y, rect->prev->upper.y);
        }
        self.clipRect = rect;
    }
    static void PopClipRect() {
        HmGuiClipRect *rect = self.clipRect;
        self.clipRect = rect->prev;
        delete rect;
    }

    static void ComputeSize(HmGuiGroup *g) {
        for (HmGuiWidget *e = g->head; e; e = e->next)
            if (e->type == WidgetType::Group)
                ComputeSize((HmGuiGroup *)e);

        g->minSize = Vec2f_Create(0, 0);

        for (HmGuiWidget *e = g->head; e; e = e->next) {
            switch (g->layout) {
                case Layout::Stack:
                    g->minSize.x = Max(g->minSize.x, e->minSize.x);
                    g->minSize.y = Max(g->minSize.y, e->minSize.y);
                    break;
                case Layout::Vertical:
                    g->minSize.x = Max(g->minSize.x, e->minSize.x);
                    g->minSize.y += e->minSize.y;
                    if (e != g->head) g->minSize.y += g->spacing;
                    break;
                case Layout::Horizontal:
                    g->minSize.x += e->minSize.x;
                    g->minSize.y = Max(g->minSize.y, e->minSize.y);
                    if (e != g->head) g->minSize.x += g->spacing;
                    break;
            }
        }

        g->minSize.x += g->paddingLower.x + g->paddingUpper.x;
        g->minSize.y += g->paddingLower.y + g->paddingUpper.y;

        if (g->storeSize) {
            HmGuiData *data = GetData(g);
            data->minSize = g->minSize;
        }

        g->minSize.x = Min(g->minSize.x, g->maxSize.x);
        g->minSize.y = Min(g->minSize.y, g->maxSize.y);
    }
    static void LayoutWidget(HmGuiWidget *e, Vec2f pos, float sx, float sy) {
        e->pos = pos;
        e->size = e->minSize;
        e->size.x += e->stretch.x * (sx - e->minSize.x);
        e->size.y += e->stretch.y * (sy - e->minSize.y);
        e->pos.x += e->align.x * (sx - e->size.x);
        e->pos.y += e->align.y * (sy - e->size.y);
    }
    static void LayoutGroup(HmGuiGroup *g) {
        Vec2f pos = g->pos;
        Vec2f size = g->size;
        float extra = 0;
        float totalStretch = 0;

        pos.x += g->paddingLower.x + g->offset.x;
        pos.y += g->paddingLower.y + g->offset.y;
        size.x -= g->paddingLower.x + g->paddingUpper.x;
        size.y -= g->paddingLower.y + g->paddingUpper.y;

        if (g->expand) {
            if (g->layout == Layout::Vertical) {
                extra = g->size.y - g->minSize.y;
                for (HmGuiWidget *e = g->head; e; e = e->next)
                    totalStretch += e->stretch.y;
            } else if (g->layout == Layout::Horizontal) {
                extra = g->size.x - g->minSize.x;
                for (HmGuiWidget *e = g->head; e; e = e->next)
                    totalStretch += e->stretch.x;
            }

            if (totalStretch > 0)
                extra /= totalStretch;
        }

        float s;
        for (HmGuiWidget *e = g->head; e; e = e->next) {
            switch (g->layout) {
                case Layout::None:
                    LayoutWidget(e, e->pos, size.x, size.y);
                    break;
                case Layout::Stack:
                    LayoutWidget(e, pos, size.x, size.y);
                    break;
                case Layout::Vertical:
                    s = e->minSize.y;
                    if (extra > 0) s += e->stretch.y * extra;
                    LayoutWidget(e, pos, size.x, s);
                    pos.y += e->size.y + g->spacing;
                    break;
                case Layout::Horizontal:
                    s = e->minSize.x;
                    if (extra > 0) s += e->stretch.x * extra;
                    LayoutWidget(e, pos, s, size.y);
                    pos.x += e->size.x + g->spacing;
                    break;
            }

            if (e->type == WidgetType::Group)
                LayoutGroup((HmGuiGroup *)e);
        }


        if (g->storeSize) {
            HmGuiData *data = GetData(g);
            data->size = g->size;
        }
    }
    inline static bool IsClipped(HmGuiGroup *g, Vec2f p) {
        return
            p.x < g->pos.x ||
            p.y < g->pos.y ||
            g->pos.x + g->size.x < p.x ||
            g->pos.y + g->size.y < p.y;
    }
    static void CheckFocus(HmGuiGroup *g) {
        if (g->clip && IsClipped(g, self.focusPos))
            return;

        for (HmGuiWidget *e = g->tail; e; e = e->prev)
            if (e->type == WidgetType::Group)
                CheckFocus((HmGuiGroup *)e);

        for (int i = 0; i < (int)FocusMouse::Size; ++i) {
            if (self.focus[i] == 0 && g->focusable[i]) {
                if (g->pos.x <= self.focusPos.x &&
                    g->pos.y <= self.focusPos.y &&
                    self.focusPos.x <= g->pos.x + g->size.x &&
                    self.focusPos.y <= g->pos.y + g->size.y) {
                    self.focus[i] = g->hash;
                }
            }
        }
    }

    static void DrawText(HmGuiText *e) {
        #if HMGUI_DRAW_GROUP_FRAMES
        Draw_Color(0.5f, 0.2f, 0.2f, 0.5f);
        Draw_Border(1.0f, e->pos.x, e->pos.y, e->size.x, e->size.y);
        #endif

        UIRenderer::Text(e->font, e->text.c_str(), e->pos.x, e->pos.y + e->minSize.y, UNPACK4(e->color));
    }
    static void DrawRect(HmGuiRect *e) {
        UIRenderer::Rect(e->pos.x, e->pos.y, e->size.x, e->size.y, UNPACK4(e->color), false);
    }
    static void DrawImage(HmGuiImage *e) {
        UIRenderer::Image(e->image, e->pos.x, e->pos.y, e->size.x, e->size.y);
    }
    static void DrawGroup(HmGuiGroup *g) {
        #if HMGUI_DRAW_GROUP_FRAMES
        Draw_Color(0.2f, 0.2f, 0.2f, 0.5f);
        Draw_Border(2.0f, g->pos.x, g->pos.y, g->size.x, g->size.y);
        #endif

        UIRenderer::BeginLayer(g->pos.x, g->pos.y, g->size.x, g->size.y, g->clip);

        for (HmGuiWidget *e = g->tail; e; e = e->prev) {
            switch (e->type) {
                case WidgetType::Group: DrawGroup((HmGuiGroup *)e); break;
                case WidgetType::Text: DrawText((HmGuiText *)e); break;
                case WidgetType::Rect: DrawRect((HmGuiRect *)e); break;
                case WidgetType::Image: DrawImage((HmGuiImage *)e); break;
            }
        }

        if (g->focusable[(int)FocusMouse::Mouse]) {
            bool focus = self.focus[(int)FocusMouse::Mouse] == g->hash;
            if (g->focusStyle == FocusStyle::None) {
                UIRenderer::Panel(g->pos.x, g->pos.y, g->size.x, g->size.y, 0.1f, 0.12f, 0.13f, 1.0f, 8.0f, g->frameOpacity);
            }

            else if (g->focusStyle == FocusStyle::Fill) {
                if (focus)
                    UIRenderer::Panel(g->pos.x, g->pos.y, g->size.x, g->size.y, 0.1f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f);
                else
                    UIRenderer::Panel(g->pos.x, g->pos.y, g->size.x, g->size.y, 0.15f, 0.15f, 0.15f, 0.8f, 0.0f, g->frameOpacity);
            }

            else if (g->focusStyle == FocusStyle::Outline) {
                if (focus) {
                    UIRenderer::Rect(g->pos.x, g->pos.y, g->size.x, g->size.y, 0.1f, 0.5f, 1.0f, 1.0f, true);
                }
            }

            else if (g->focusStyle == FocusStyle::Underline) {
                UIRenderer::Rect(g->pos.x, g->pos.y, g->size.x, g->size.y, 0.3f, 0.3f, 0.3f, focus ? 0.5f : g->frameOpacity, false);
            }
        }

        UIRenderer::EndLayer();
    }

private:
    static HmGuiDataStructure self;
};

HmGuiDataStructure HmGui::self = { 0 };

}

export module Ultra.UI.HmGui;

import Ultra.Core;
import Ultra.Engine.Phoenix;
import Ultra.Engine.UIRenderer;
import Ultra.Math;
import Ultra.System.Input;
export import Ultra.Engine.Font;

export namespace Ultra::UI {

#define HMGUI_DRAW_GROUP_FRAMES 0
#define HMGUI_ENABLE_DRAGGING 1
// ToDo: Check Profiler
#define FRAME_BEGIN
#define FRAME_END

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
    uint64_t hash;
    WidgetType type;
    Vector2Df pos;
    Vector2Df size;
    Vector2Df minSize;
    Vector2Df align;
    Vector2Df stretch;
};

struct HmGuiGroup: public HmGuiWidget {
    HmGuiWidget *head;
    HmGuiWidget *tail;
    Layout layout;
    uint32_t children;
    FocusStyle focusStyle;
    Vector2Df paddingLower;
    Vector2Df paddingUpper;
    Vector2Df offset;
    Vector2Df maxSize;
    Vector2Df totalStretch;
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
    Vector4Df color;
};

struct HmGuiRect: public HmGuiWidget {
    Vector4Df color;
};

struct HmGuiImage: public HmGuiWidget {
    Tex2DData *image;
};

struct HmGuiClipRect {
    HmGuiClipRect *prev;
    Vector2Df lower;
    Vector2Df upper;
};

struct HmGuiData {
    Vector2Df offset;
    Vector2Df minSize;
    Vector2Df size;
};

struct HmGuiStyle {
    HmGuiStyle *prev;
    FontData *font;
    float spacing;
    Vector4Df colorPrimary;
    Vector4Df colorFrame;
    Vector4Df colorText;
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
    //HashMap *data;
    unordered_map<uint64_t, HmGuiData *> data;

    uint64_t focus[(int)FocusMouse::Size];
    Vector2Df focusPos;
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

            self.style->colorPrimary = { 0.1f, 0.5f, 1.0f, 1.0f };
            self.style->colorFrame = { 0.1f, 0.1f, 0.1f, 0.5f };
            self.style->colorText = { 1.0f, 1.0f, 1.0f, 1.0f };

            self.clipRect = 0;
            //self.data = HashMap_Create(0, 128);

            for (int i = 0; i < (int)FocusMouse::Size; ++i) self.focus[i] = 0;
            self.activate = false;
        }

        if (self.root) {
            FreeGroup(self.root);
            self.root = 0;
        }
        self.last = 0;
        //self.activate = Input_GetPressed(Button_Mouse_Left);
        self.activate = Input::GetMouseButtonState(MouseButton::Left);

        BeginGroup(Layout::None);
        self.group->clip = true;
        self.group->pos = { 0, 0 };
        self.group->size = { sx, sy };
        self.root = self.group;
    }
    static void End() {
        FRAME_BEGIN;
        EndGroup();
        ComputeSize(self.root);
        LayoutGroup(self.root);

        for (int i = 0; i < (int)FocusMouse::Size; ++i) self.focus[i] = 0;
        Vector2Di mouse; //Input_GetMousePosition(&mouse);
        auto [x, y] = Input::GetMousePosition();
        mouse.x = x;
        mouse.y = y;
        self.focusPos = { (float)mouse.x, (float)mouse.y };
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
            Vector2Di scroll; //Input_GetMouseScroll(&scroll);
            auto delta = Input::GetMouseWheelDelta();
            scroll.y = delta;
            data->offset.y -= 10.0f * scroll.y;
        }

        float maxScroll = std::max(0.0f, data->minSize.y - data->size.y);
        data->offset.y = std::clamp(data->offset.y, 0.0f, maxScroll);

        EndGroup();

        BeginGroupY();
        SetStretch(0, 1);
        SetSpacing(0);
        if (maxScroll > 0) {
            float handleSize = data->size.y * (data->size.y / data->minSize.y);
            float handlePos = std::lerp(0.0f, data->size.y - handleSize, data->offset.y / maxScroll);
            Rect(4.0f, handlePos, 0.0f, 0.0f, 0.0f, 0.0f);
            Rect(4.0f, handleSize, self.style->colorFrame.x, self.style->colorFrame.y, self.style->colorFrame.z, self.style->colorFrame.w);
        } else {
            Rect(4.0f, 16.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        }
        EndGroup();
        EndGroup();
    }
    static void BeginWindow(const char * title) {
        BeginGroupStack();
        SetStretch(0, 0);
        self.group->focusStyle = FocusStyle::None;
        self.group->frameOpacity = 0.95f;
        #if HMGUI_ENABLE_DRAGGING
        HmGuiData *data = GetData(self.group);
        if (GroupHasFocus(FocusMouse::Mouse)) {
            //if (Input_GetDown(Button_Mouse_Left)) {
            if (Input::GetMouseButtonState(MouseButton::Left)) {
                Vector2Di md; //Input_GetMouseDelta(&md);
                auto [x, y] = Input::GetMousePositionDelta();
                md.x = x;
                md.y = y;
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

    static bool Button(const char * label) {
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
    static bool Checkbox(const char * label, bool value) {
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
        Rect(16, 16, self.style->colorFrame.x, self.style->colorFrame.y, self.style->colorFrame.z, self.style->colorFrame.w);
        if (value) {
            Rect(10, 10, self.style->colorPrimary.x, self.style->colorPrimary.y, self.style->colorPrimary.z, self.style->colorPrimary.w);
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
        e->stretch = { 1, 1 };
    }
    static void Rect(float sx, float sy, float r, float g, float b, float a) {
        HmGuiRect *e = new HmGuiRect();
        InitWidget(e, WidgetType::Rect);
        e->color = { r, g, b, a };
        e->minSize = { sx, sy };
    }
    static void Text(const char * text) {
        TextEx(self.style->font, text, self.style->colorText.x, self.style->colorText.y, self.style->colorText.z, self.style->colorText.w);
    }
    static void TextColored(const char * text, float r, float g, float b, float a) {
        TextEx(self.style->font, text, r, g, b, a);
    }
    static void TextEx(FontData *font, const char * text, float r, float g, float b, float a) {
        HmGuiText *e = new HmGuiText();
        InitWidget(e, WidgetType::Text);
        e->font = font;
        e->text = string(text);
        e->color = { r, g, b, a };
        Vector2Di size; Font::GetSize2(e->font, &size, e->text.c_str());
        e->minSize = { (float)size.x, (float)size.y };
        SetAlign(0.0f, 1.0f);
    }

    static void SetAlign(float ax, float ay) {
        self.last->align = { ax, ay };
    }
    static void SetPadding(float px, float py) {
        self.group->paddingLower = { px, py };
        self.group->paddingUpper = { px, py };
    }
    static void SetPaddingEx(float left, float top, float right, float bottom) {
        self.group->paddingLower = { left, top };
        self.group->paddingUpper = { right, bottom };
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
        self.last->stretch = { x, y };
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
        self.style->colorText = { r, g, b, a };
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

        std::hash<uint64_t> hasher;
        if (e->parent) {
            e->parent->children++;
            e->hash = hasher(e->parent->hash ^ e->parent->children);
            (e->next ? e->next->prev : e->parent->tail) = e;
            (e->prev ? e->prev->next : e->parent->head) = e;
        } else {
            e->hash = hasher(0);
        }

        e->type = type;
        e->pos = { 0, 0 };
        e->size = { 0, 0 };
        e->minSize = { 0, 0 };
        e->align = { 0, 0 };
        e->stretch = { 0, 0 };

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
        e->paddingLower = { 0.0f, 0.0f };
        e->paddingUpper = { 0.0f, 0.0f };
        e->offset = { 0.0f, 0.0f };
        e->maxSize = { 1e30f, 1e30f };
        e->spacing = self.style->spacing;
        e->frameOpacity = 0.0f;
        e->clip = false;
        e->expand = true;
        for (int i = 0; i < (int)FocusMouse::Size; ++i) e->focusable[i] = false;
        e->storeSize = false;
        self.group = e;

        switch (layout) {
            case Layout::Stack:
                e->stretch = { 1, 1 };
                break;
            case Layout::Vertical:
                e->stretch = { 1, 0 };
                break;
            case Layout::Horizontal:
                e->stretch = { 0, 1 };
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
        //HmGuiData *data = (HmGuiData *)HashMap_GetRaw(self.data, g->hash);
        auto data = self.data[g->hash];
        if (!data) {
            data = new HmGuiData();
            data->offset = { 0, 0 };
            data->minSize = { 0, 0 };
            data->size = { 0, 0 };
            //HashMap_SetRaw(self.data, g->hash, data);
            self.data[g->hash] = data;
        }
        return data;
    }

    static void PushClipRect(HmGuiGroup *g) {
        HmGuiClipRect *rect = new HmGuiClipRect();
        rect->prev = self.clipRect;
        rect->lower = g->pos;
        // ToDo: :(
        //rect->upper = { g->pos, g->size };
        if (rect->prev) {
            rect->lower.x = std::max(rect->lower.x, rect->prev->lower.x);
            rect->lower.y = std::max(rect->lower.y, rect->prev->lower.y);
            rect->upper.x = std::min(rect->upper.x, rect->prev->upper.x);
            rect->upper.y = std::min(rect->upper.y, rect->prev->upper.y);
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

        g->minSize = { 0, 0 };

        for (HmGuiWidget *e = g->head; e; e = e->next) {
            switch (g->layout) {
                case Layout::Stack:
                    g->minSize.x = std::max(g->minSize.x, e->minSize.x);
                    g->minSize.y = std::max(g->minSize.y, e->minSize.y);
                    break;
                case Layout::Vertical:
                    g->minSize.x = std::max(g->minSize.x, e->minSize.x);
                    g->minSize.y += e->minSize.y;
                    if (e != g->head) g->minSize.y += g->spacing;
                    break;
                case Layout::Horizontal:
                    g->minSize.x += e->minSize.x;
                    g->minSize.y = std::max(g->minSize.y, e->minSize.y);
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

        g->minSize.x = std::min(g->minSize.x, g->maxSize.x);
        g->minSize.y = std::min(g->minSize.y, g->maxSize.y);
    }
    static void LayoutWidget(HmGuiWidget *e, Vector2Df pos, float sx, float sy) {
        e->pos = pos;
        e->size = e->minSize;
        e->size.x += e->stretch.x * (sx - e->minSize.x);
        e->size.y += e->stretch.y * (sy - e->minSize.y);
        e->pos.x += e->align.x * (sx - e->size.x);
        e->pos.y += e->align.y * (sy - e->size.y);
    }
    static void LayoutGroup(HmGuiGroup *g) {
        Vector2Df pos = g->pos;
        Vector2Df size = g->size;
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
    inline static bool IsClipped(HmGuiGroup *g, Vector2Df p) {
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
        Draw::Color(0.5f, 0.2f, 0.2f, 0.5f);
        Draw::Border(1.0f, e->pos.x, e->pos.y, e->size.x, e->size.y);
        #endif

        UIRenderer::Text(e->font, e->text.c_str(), e->pos.x, e->pos.y + e->minSize.y, e->color.x, e->color.y, e->color.z, e->color.w);
    }
    static void DrawRect(HmGuiRect *e) {
        UIRenderer::Rect(e->pos.x, e->pos.y, e->size.x, e->size.y, e->color.x, e->color.y, e->color.z, e->color.w, false);
    }
    static void DrawImage(HmGuiImage *e) {
        UIRenderer::Image(e->image, e->pos.x, e->pos.y, e->size.x, e->size.y);
    }
    static void DrawGroup(HmGuiGroup *g) {
        #if HMGUI_DRAW_GROUP_FRAMES
        Draw::Color(0.2f, 0.2f, 0.2f, 0.5f);
        Draw::Border(2.0f, g->pos.x, g->pos.y, g->size.x, g->size.y);
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

﻿export module Ultra.UI.HmGui;

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
/// @brief  Types
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

    Ultra::Position Position;
    Ultra::Size Size;
    Ultra::Size MinSize;
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

struct HmGuiRect: public HmGuiWidget {
    Vector4Df color;
};

struct HmGuiText: public HmGuiWidget {
    FontData *font;
    string text;
    Vector4Df color;
};

struct HmGuiImage: public HmGuiWidget {
    Tex2DData *image;
};

struct HmGuiClipRect {
    HmGuiClipRect *prev;
    Position lower;
    Position upper;
};


struct HmGuiData {
    Ultra::Position Offset;
    Ultra::Size MinSize;
    Ultra::Size Size;
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
    unordered_map<uint64_t, HmGuiData *> data;

    uint64_t focus[(int)FocusMouse::Size];
    Position FocusPos;
    bool activate;
};

class HmGui {
public:
    HmGui() = default;
    ~HmGui() = default;
    static HmGui &Instance() {
        static HmGui instance;
        return instance;
    }

    // Functions
    static void BeginGroupX() {
        BeginGroup(Layout::Horizontal);
    }
    static void BeginGroupY() {
        BeginGroup(Layout::Vertical);
    }
    static void BeginGroupStack() {
        BeginGroup(Layout::Stack);
    }

    ///
    /// @brief ToDo
    ///
    static void Begin(Size size) {
        static auto &initialize = UIRenderer::Instance();

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
        self.activate = Input::GetMouseButtonState(MouseButton::Left);

        BeginGroup(Layout::None);
        self.group->clip = true;
        self.group->Position = { 0, 0 };
        self.group->Size = size;
        self.root = self.group;
    }
    static void End() {
        FRAME_BEGIN;
        EndGroup();
        ComputeSize(self.root);
        LayoutGroup(self.root);

        for (int i = 0; i < (int)FocusMouse::Size; ++i) {
            self.focus[i] = 0;
        }
        Position mouse;
        auto [x, y] = Input::GetMousePosition();
        mouse.X = x;
        mouse.Y = y;
        self.FocusPos = mouse;
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

    static void BeginWindow(const char *title) {
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
                data->Offset.X += md.x;
                data->Offset.Y += md.y;
            }
        }

        self.group->Position.X += data->Offset.X;
        self.group->Position.Y += data->Offset.Y;
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
        self.group->offset.y = -data->Offset.Y;
    }
    static void EndScroll() {
        HmGuiData *data = GetData(self.group);
        if (GroupHasFocus(FocusMouse::Scroll)) {
            Vector2Di scroll; //Input_GetMouseScroll(&scroll);
            auto delta = Input::GetMouseWheelDelta();
            scroll.y = delta;
            data->Offset.Y -= 10.0f * scroll.y;
        }

        float maxScroll = std::max(0.0f, data->MinSize.Height - data->Size.Height);
        data->Offset.Y = std::clamp(data->Offset.Y, 0.0f, maxScroll);

        EndGroup();

        BeginGroupY();
        SetStretch(0, 1);
        SetSpacing(0);
        if (maxScroll > 0) {
            float handleSize = data->Size.Height * (data->Size.Height / data->MinSize.Height);
            float handlePos = std::lerp(0.0f, data->Size.Height - handleSize, data->Offset.Y / maxScroll);
            Rectangle(4.0f, handlePos, 0.0f, 0.0f, 0.0f, 0.0f);
            Rectangle(4.0f, handleSize, self.style->colorFrame.x, self.style->colorFrame.y, self.style->colorFrame.z, self.style->colorFrame.w);
        } else {
            Rectangle(4.0f, 16.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        }
        EndGroup();
        EndGroup();
    }
    static bool Button(const string &label) {
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
    static bool Checkbox(const string &label, bool value) {
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
        Rectangle(16, 16, self.style->colorFrame.x, self.style->colorFrame.y, self.style->colorFrame.z, self.style->colorFrame.w);
        if (value) {
            Rectangle(10, 10, self.style->colorPrimary.x, self.style->colorPrimary.y, self.style->colorPrimary.z, self.style->colorPrimary.w);
            SetAlign(0.5f, 0.5f);
        }
        EndGroup();
        SetStretch(0, 0);
        EndGroup();
        return value;
    }
    static float Slider(float lower, float upper, float value) {
        BeginGroupStack();
        Rectangle(0, 2, 0.5f, 0.5f, 0.5f, 1.0f);
        SetAlign(0.5f, 0.5f);
        SetStretch(1, 0);
        EndGroup();
        return 0.0f;
    }

    static void Rectangle(float sx, float sy, float r, float g, float b, float a) {
        HmGuiRect *e = new HmGuiRect();
        InitWidget(e, WidgetType::Rect);
        e->color = { r, g, b, a };
        e->MinSize = { sx, sy };
    }
    static void Image(Tex2DData *image) {
        HmGuiImage *e = new HmGuiImage();
        InitWidget(e, WidgetType::Image);
        e->image = image;
        e->stretch = { 1, 1 };
    }
    static void Text(const string &text) {
        TextEx(self.style->font, text, self.style->colorText.x, self.style->colorText.y, self.style->colorText.z, self.style->colorText.w);
    }
    static void TextColored(const string &text, float r, float g, float b, float a) {
        TextEx(self.style->font, text, r, g, b, a);
    }
    static void TextEx(FontData *font, const string &text, float r, float g, float b, float a) {
        HmGuiText *e = new HmGuiText();
        InitWidget(e, WidgetType::Text);
        e->font = font;
        e->text = text;
        e->color = { r, g, b, a };
        Vector2Di size; Font::GetSize2(e->font, &size, e->text.c_str());
        e->MinSize = { (float)size.x, (float)size.y };
        SetAlign(0.0f, 1.0f);
    }

    // Accessors
    static bool GroupHasFocus(FocusMouse type) {
        self.group->focusable[(int)type] = true;
        return self.focus[(int)type] == self.group->hash;
    }

    // Modifiers
    static void SetAlign(float x, float y) {
        self.last->align = { x, y };
    }
    static void SetPadding(float x, float y) {
        self.group->paddingLower = { x, y };
        self.group->paddingUpper = { x, y };
    }
    static void SetPaddingExtended(float left, float top, float right, float bottom) {
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

    // State Modifiers
    static void PushFont(FontData *font) {
        PushStyle();
        self.style->font = font;
    }
    static void PushStyle() {
        HmGuiStyle *style = new HmGuiStyle();
        *style = *self.style;
        style->prev = self.style;
        self.style = style;
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
    static HmGuiData *GetData(HmGuiGroup *g) {
        //HmGuiData *data = (HmGuiData *)HashMap_GetRaw(self.data, g->hash);
        auto data = self.data[g->hash];
        if (!data) {
            data = new HmGuiData();
            data->Offset = { 0, 0 };
            data->MinSize = { 0, 0 };
            data->Size = { 0, 0 };
            //HashMap_SetRaw(self.data, g->hash, data);
            self.data[g->hash] = data;
        }
        return data;
    }

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
        e->Position = { 0, 0 };
        e->Size = { 0, 0 };
        e->MinSize = { 0, 0 };
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

    static void PushClipRect(HmGuiGroup *g) {
        HmGuiClipRect *rect = new HmGuiClipRect();
        rect->prev = self.clipRect;
        rect->lower = g->Position;
        // ToDo: :(
        //rect->upper = { g->Position, g->Size };
        if (rect->prev) {
            rect->lower.X = std::max(rect->lower.X, rect->prev->lower.X);
            rect->lower.Y = std::max(rect->lower.Y, rect->prev->lower.Y);
            rect->upper.X = std::min(rect->upper.X, rect->prev->upper.X);
            rect->upper.Y = std::min(rect->upper.Y, rect->prev->upper.Y);
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

        g->MinSize = { 0, 0 };

        for (HmGuiWidget *e = g->head; e; e = e->next) {
            switch (g->layout) {
                case Layout::Stack:
                    g->MinSize.Width = std::max(g->MinSize.Width, e->MinSize.Width);
                    g->MinSize.Height = std::max(g->MinSize.Height, e->MinSize.Height);
                    break;
                case Layout::Vertical:
                    g->MinSize.Width = std::max(g->MinSize.Width, e->MinSize.Width);
                    g->MinSize.Height += e->MinSize.Height;
                    if (e != g->head) g->MinSize.Height += g->spacing;
                    break;
                case Layout::Horizontal:
                    g->MinSize.Width += e->MinSize.Width;
                    g->MinSize.Height = std::max(g->MinSize.Height, e->MinSize.Height);
                    if (e != g->head) g->MinSize.Width += g->spacing;
                    break;
            }
        }

        g->MinSize.Width += g->paddingLower.x + g->paddingUpper.x;
        g->MinSize.Height += g->paddingLower.y + g->paddingUpper.y;

        if (g->storeSize) {
            HmGuiData *data = GetData(g);
            data->MinSize = g->MinSize;
        }

        g->MinSize.Width = std::min(g->MinSize.Width, g->maxSize.x);
        g->MinSize.Height = std::min(g->MinSize.Height, g->maxSize.y);
    }
    static void LayoutWidget(HmGuiWidget *e, Position position, Size size) {
        e->Position = position;
        e->Size = e->MinSize;
        e->Size.Width += e->stretch.x * (size.Width - e->MinSize.Width);
        e->Size.Height += e->stretch.y * (size.Height - e->MinSize.Height);
        e->Position.X += e->align.x * (size.Width - e->Size.Width);
        e->Position.Y += e->align.y * (size.Height - e->Size.Height);
    }
    static void LayoutGroup(HmGuiGroup *g) {
        Position pos = g->Position;
        Size size = g->Size;
        float extra = 0;
        float totalStretch = 0;

        pos.X += g->paddingLower.x + g->offset.x;
        pos.Y += g->paddingLower.y + g->offset.y;
        size.Width -= g->paddingLower.x + g->paddingUpper.x;
        size.Height -= g->paddingLower.y + g->paddingUpper.y;

        if (g->expand) {
            if (g->layout == Layout::Vertical) {
                extra = g->Size.Height - g->MinSize.Height;
                for (HmGuiWidget *e = g->head; e; e = e->next)
                    totalStretch += e->stretch.y;
            } else if (g->layout == Layout::Horizontal) {
                extra = g->Size.Width - g->MinSize.Width;
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
                    LayoutWidget(e, e->Position, size);
                    break;
                case Layout::Stack:
                    LayoutWidget(e, pos, size);
                    break;
                case Layout::Vertical:
                    s = e->MinSize.Height;
                    if (extra > 0) s += e->stretch.y * extra;
                    LayoutWidget(e, pos, { size.Width, s });
                    pos.Y += e->Size.Height + g->spacing;
                    break;
                case Layout::Horizontal:
                    s = e->MinSize.Width;
                    if (extra > 0) s += e->stretch.x * extra;
                    LayoutWidget(e, pos, { s, size.Height });
                    pos.X += e->Size.Width + g->spacing;
                    break;
            }

            if (e->type == WidgetType::Group)
                LayoutGroup((HmGuiGroup *)e);
        }


        if (g->storeSize) {
            HmGuiData *data = GetData(g);
            data->Size = g->Size;
        }
    }
    inline static bool IsClipped(HmGuiGroup *g, Position position) {
        return
            position.X < g->Position.X || position.Y < g->Position.Y ||
            g->Position.X + g->Size.Width < position.X ||
            g->Position.Y + g->Size.Height < position.Y;
    }
    static void CheckFocus(HmGuiGroup *g) {
        if (g->clip && IsClipped(g, self.FocusPos))
            return;

        for (HmGuiWidget *e = g->tail; e; e = e->prev)
            if (e->type == WidgetType::Group)
                CheckFocus((HmGuiGroup *)e);

        for (int i = 0; i < (int)FocusMouse::Size; ++i) {
            if (self.focus[i] == 0 && g->focusable[i]) {
                if (g->Position.X <= self.FocusPos.X &&
                    g->Position.Y <= self.FocusPos.Y &&
                    self.FocusPos.X <= g->Position.X + g->Size.Width &&
                    self.FocusPos.Y <= g->Position.Y + g->Size.Height) {
                    self.focus[i] = g->hash;
                }
            }
        }
    }

    static void DrawText(HmGuiText *e) {
        #if HMGUI_DRAW_GROUP_FRAMES
        Draw::Color(0.5f, 0.2f, 0.2f, 0.5f);
        Draw::Border(1.0f, e->Position.X, e->Position.Y, e->Size.Width, e->Size.Height);
        #endif

        UIRenderer::Text(Position { e->Position.X, e->Position.Y + e->MinSize.Height }, e->text, Color { e->color.x, e->color.y, e->color.z, e->color.w }, e->font);
    }
    static void DrawRect(HmGuiRect *e) {
        UIRenderer::Rect(Position { e->Position.X, e->Position.Y }, Size { e->Size.Width, e->Size.Height }, Color { e->color.x, e->color.y, e->color.z, e->color.w }, false);
    }
    static void DrawImage(HmGuiImage *e) {
        UIRenderer::Image(Position { e->Position.X, e->Position.Y }, Size { e->Size.Width, e->Size.Height }, e->image);
    }
    static void DrawGroup(HmGuiGroup *g) {
        #if HMGUI_DRAW_GROUP_FRAMES
        Draw::Color(0.2f, 0.2f, 0.2f, 0.5f);
        Draw::Border(2.0f, g->Position.X, g->Position.Y, g->Size.Width, g->Size.Height);
        #endif

        UIRenderer::BeginLayer(Position { g->Position.X, g->Position.Y }, Size { g->Size.Width, g->Size.Height }, g->clip);

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
                UIRenderer::Panel(Position { g->Position.X, g->Position.Y }, Size { g->Size.Width, g->Size.Height }, Color { 0.1f, 0.12f, 0.13f, 1.0f }, 8.0f , g->frameOpacity);
            }

            else if (g->focusStyle == FocusStyle::Fill) {
                if (focus)
                    UIRenderer::Panel(Position { g->Position.X, g->Position.Y }, Size { g->Size.Width, g->Size.Height }, Color { 0.1f, 0.5f, 1.0f, 1.0f }, 0.0f, 1.0f);
                else
                    UIRenderer::Panel(Position { g->Position.X, g->Position.Y }, Size { g->Size.Width, g->Size.Height }, Color { 0.15f, 0.15f, 0.15f, 0.8f }, 0.0f, g->frameOpacity);
            }

            else if (g->focusStyle == FocusStyle::Outline) {
                if (focus) {
                    UIRenderer::Rect(Position { g->Position.X, g->Position.Y }, Size { g->Size.Width, g->Size.Height }, Color { 0.1f, 0.5f, 1.0f, 1.0f }, true);
                }
            }

            else if (g->focusStyle == FocusStyle::Underline) {
                UIRenderer::Rect(Position { g->Position.X, g->Position.Y }, Size { g->Size.Width, g->Size.Height }, Color { 0.3f, 0.3f, 0.3f, focus ? 0.5f : g->frameOpacity }, false);
            }
        }

        UIRenderer::EndLayer();
    }

private:
    inline static HmGuiDataStructure self = { 0 };
};

}
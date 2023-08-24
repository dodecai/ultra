export module Ultra.UI.HmGui;

export import Ultra.Engine.Font;

import Ultra.Core;
import Ultra.Engine.Renderer.Texture;
import Ultra.Engine.Renderer.Viewport;
import Ultra.Engine.UIRenderer;
import Ultra.Math;
import Ultra.System.Input;
import Ultra.Utility.Timer;
import Ultra.Utility.String;

///
/// @brief Enumeration and Types
///
export namespace Ultra::UI {

enum class FocusStyle {
    None,
    Fill,
    Outline,
    Underline,
};

enum class FocusType {
    Mouse,
    Keyboard,
    Scroll,
};

enum class Layout {
    None,
    Stack,
    Vertical,
    Horizontal,
};

}

///
/// @brief Private
///
namespace Ultra::UI {

///
/// Tests
///

using CheckListEntry = std::map<std::string, bool>;
using CheckList = std::map<std::string, CheckListEntry>;

CheckList gCheckList = {
    { "Basic Widgets", {
        { "Hash Storage Invalidation", false },
        { "Text", true },
        { "Buttons", true },
        { "Checkboxes", true },
        { "Sliders", false },
        { "Radio Groups", false },
        { "Selectable", false },
        { "Tooltips", false },
    }},
    { "Layout", {
        { "Deferred Layout Pass", true },
        { "Horizontal Groups", true },
        { "Vertical Groups", true },
        { "Stacked Groups", true },
        { "Group Padding", true },
        { "Group Margins", false },
        { "Child Spacing ", true },
        { "ScrollViews", true },
        { "Windows", true },
    }},
    { "Input", {
        { "Clip Groups", true },
        { "Input Clipping", true },
        { "Sticky Drag", false },
        { "Keyboard Navigation", false },
        { "Gamepad Navigation", false },
    }},
    { "Draw", {
        { "Draw Layers", true },
        { "Shader-Based Render", true },
        { "Glyph Render", false },
        }},
    { "Technical", {
        { "Hash Storage", true },
        { "Hash Storage Invalidation", false },
        { "Deferred Metrics", true },
    }},
};

constexpr const char *gCodeExample = R"(
#include <print>

// The amazing and well known foo class.
class Foo {
public:
    Foo() = default;
    virtual ~Foo() = default;

    virtual void Update() = 0;
};

// As amazing as foo can be, it is nothing without Bar.
class Bar: Foo {
public:
    Bar() = default;
    virtual ~Bar() = default;

    void Update() override {
    }
};


int main() {
    print("{}", "Hello World!");
}
)";

///
/// @brief Base Types and Component Interface
///

enum class ComponentType {
    // Containers
    None        = 0x00,
    Container   = 0x01,
    Window      = 0x02,
    Panel       = 0x03,
    ScrollView  = 0x04,
    Containers  = 0x1F, // Used as less than boundary

    // Controls
    Button      = 0x20,
    Cursor      = 0x21,
    CheckBox    = 0x22,
    ColorPicker = 0x23,
    Grid        = 0x24,
    Image       = 0x25,
    Input       = 0x26,
    Label       = 0x27,
    Selection   = 0x28,
    Seperator   = 0x29,
    Slider      = 0x2A,
    Table       = 0x2B,
    Controls    = 0x4F, // Used as less than boundary

    // Information
    Badge           = 0x50,
    Notification    = 0x51,
    Tooltip         = 0x52,
    Information     = 0x6F, // Used as less than boundary

    // Navigation (multiple-controls)
    MenuBar     = 0x70,
    SideBar     = 0x71,
    TabBar      = 0x72,
    Navigations = 0x8F, // Used as less than boundary
};

struct ClipArea {
    Position TopLeft;
    Position BottomRight;
};

struct Style {
    Color ColorPrimary { 0.1f, 0.5f, 1.0f, 1.0f };
    Color ColorSecondary { 0.1f, 1.0f, 0.5f, 1.0f };
    Color ColorTernary {};
    Color ColorFrame { 0.1f, 0.1f, 0.1f, 0.5f };
    Color ColorText { 1.0f, 1.0f, 1.0f, 1.0f };


    Color ColorFocusNone { 0.1f, 0.12f, 0.13f, 1.0f };          // Panel
    Color ColorFocusFill { 0.1f, 0.5f, 1.0f, 1.0f };            // Panel
    Color ColorFocusFillNone { 0.15f, 0.15f, 0.15f, 0.8f };     // Panel
    Color ColorFocusOutline { 0.1f, 0.5f, 1.0f, 1.0f };         // Rect
    Color ColorFocusUnderline { 0.3f, 0.3f, 0.3f, 0.5 };        // Rect
    Color ColorFocusUnderlineNone { 0.3f, 0.3f, 0.3f, 1.0 };    // Rect + FrameOpacity
    Color ColorGroupFrames { 0.2f, 0.2f, 0.2f, 0.5f };          // Border


    Reference<Font> Font { nullptr };

    float Spacing { 6.0f };
};

class Component {
public:
    Component(const string &id, const ComponentType &type):
        mID(id),
        mHash(mHasher(id)),
        Type(type) {
    }
    virtual ~Component() = default;

    // Accessors
    const string &GetID() const {
        return mID;
    }
    size_t GetHash() const {
        return mHash;
    }
    bool Hovered(const Position &hoverPosition) const {
        if (Position.X <= hoverPosition.X &&
            Position.Y <= hoverPosition.Y &&
            hoverPosition.X <= Position.X + Size.Width &&
            hoverPosition.Y <= Position.Y + Size.Height) {
            return true;
        }
        return false;
    }

    // Converters
    template<typename T>
    T *As() {
        return reinterpret_cast<T *>(this);
    }

    // Methods
    virtual void Draw() {}

protected:
    const string mID;
    const size_t mHash;

public:
    Component *Parent {};
    ComponentType Type {};

    // Transformation
    Ultra::Alignment Alignment {};
    bool Clip {};
    ClipArea ClipArea {};
    Ultra::Padding Padding {};
    Ultra::Position Offset {};
    Ultra::Size MaxSize { 1e30f, 1e30f };
    Ultra::Size MinSize {};
    Position Position {};
    Size Size {};
    Ultra::Stretch Stretch {};
    float Spacing {};

    // States
    bool Active {};
    bool Disabled {};
    bool Focused {};
    bool Visible { true };

    // Style
    Color Color {};
    float FrameOpacity { 1.0f };
    Ultra::UI::Layout Layout {};
    float ZIndex {};

    FocusStyle FocusStyle {};
private:
    std::hash<std::string> mHasher;
};

///
/// @brief Controls
///

class Control: public Component {
public:
    Control(const string &id, const ComponentType &type):
        Component(id, type) {
    }
    virtual ~Control() = default;

    // Methods
    virtual void DebugPrint(size_t indent) const {
        LogInfo("{}{}{}", indent == 0 ? "" : string(indent * 2, ' '), indent == 0 ? "" : "◌ ", mID);
    }

private:
    Ultra::Position CanvasPosition {};
    Ultra::Size CanvasSize {};
};

class Cursor: public Control {
public:
    Cursor(const string &id):
        Control(id, ComponentType::Cursor) {
    }
    virtual ~Cursor() = default;

    Texture *Data {};
    bool Outline {};
};

class Button: public Control {
public:
    Button(const string &id):
        Control(id, ComponentType::Button) {
    }
    virtual ~Button() = default;
    static Scope<Button> Create(const string &id, string_view text);

    // Methods
    void Draw() override;
    void DebugPrint(size_t indent) const override {
        Log("{}{}{} [ Text: '{}' | Position: 'x={}, y={}' | Size '{}x{}' ]",
            indent == 0 ? "" : string(indent * 2, ' '),
            indent == 0 ? "" : "◌ ",
            mID,
            Text,
            Position.X, Position.Y,
            Size.Width, Size.Height
        );
    }

public:
    std::function<void()> Callback;
    Font *Font {};
    bool Outline {};
    string Text {};
};

class CheckBox: public Control {
public:
    CheckBox(const string &id):
        Control(id, ComponentType::CheckBox) {
    }
    virtual ~CheckBox() = default;

    // Methods
    void Draw() override;
    void DebugPrint(size_t indent) const override {
        Log("{}{}{} [ Text: '{}' | Position: 'x={}, y={}' | Size '{}x{}' ]",
            indent == 0 ? "" : string(indent * 2, ' '),
            indent == 0 ? "" : "◌ ",
            mID,
            Text,
            Position.X, Position.Y,
            Size.Width, Size.Height
        );
    }

public:
    bool Value { false };

    Font *Font {};
    bool Outline {};
    string Text {};
    
    Ultra::Color InnerColor {};
    Ultra::Position InnerPosition {};
    Ultra::Size InnerSize {};

    Ultra::Color OutherColor {};
    Ultra::Position OutherPosition {};
    Ultra::Size OutherSize {};
};

class Label: public Control {
public:
    Label(const string &id):
        Control(id, ComponentType::Label) {
    }
    virtual ~Label() = default;

    // Methods
    void Draw() override;
    void DebugPrint(size_t indent) const override {
        Log("{}{}{} [ Text: '{}' | Position: 'x={}, y={}' | Size '{}x{}' ]",
            indent == 0 ? "" : string(indent * 2, ' '),
            indent == 0 ? "" : "◌ ",
            mID,
            Text,
            Position.X, Position.Y,
            Size.Width, Size.Height
        );
    }

public:
    Font *Font {};
    bool Outline {};
    string Text {};
};

class Seperator: public Control {
public:
    Seperator(const string &id):
        Control(id, ComponentType::Seperator) {
    }
    virtual ~Seperator() = default;

    void Draw() override;
};

struct Image: public Control {
public:
    Image(const string &id):
        Control(id, ComponentType::Image) {
    }
    virtual ~Image() = default;

    void Draw() override;

public:
    Reference<Texture> Data {};
};

class Slider: public Control {
public:
    Slider(const string &id):
        Control(id, ComponentType::Slider) {
    }
    virtual ~Slider() = default;

    void Draw() override;
};

///
/// @brief Containers
///

class Container: public Component {
public:
    Container(const string &id):
        Component(id, ComponentType::Container) {
    }
    virtual ~Container() = default;

    // Factory Methods
    Container *CreateContainer(const Ultra::UI::Layout &layout = Layout::None, const string &uniqueId = "")  {
        string id = uniqueId.empty() ? std::format("Container#{}", sContainerCounter++) : uniqueId;

        auto *component = GetChild(id);
        if (component) return component->As<Container>();

        auto container = CreateScope<Container>(id);
        container->Layout = layout;
        container->Expand = true;
        container->MaxSize = { 1e30f, 1e30f };
        container->Spacing = this->Style.Spacing;
        container->Style.Font = this->Style.Font;
        switch (layout) {
            case Layout::Stack:
                container->Stretch = { 1.0f, 1.0f };
                break;
            case Layout::Vertical:
                container->Stretch = { 1.0f, 0.0f };
                break;
            case Layout::Horizontal:
                container->Stretch = { 0.0f, 1.0f };
                break;
        }

        auto result = container.get();
        AddChild(std::move(container));
        Parent = this;
        return result;
    }
    Button *CreateButton(string_view text)  {
        string id = std::format("Button#{}", sButtonCounter++);

        auto *component = GetChild(id)->As<Button>();
        if (component) return component;

        auto button = Button::Create(id, text);
        button->Parent = this;
        button->Color = this->Style.ColorText;
        button->Font = this->Style.Font.get();
        if (button->Font) {
            button->MinSize = button->Font->GetSize(text);
            button->MinSize.Width += button->Padding.Left + button->Padding.Right;
            button->MinSize.Height += button->Padding.Top + button->Padding.Bottom;
            button->Position.X -= button->Padding.Left + button->Offset.X;
            button->Position.Y -= button->Padding.Top + button->Offset.Y;
        }

        auto result = button.get();
        AddChild(std::move(button));
        return result;
    }
    CheckBox *CreateCheckBox(string_view text, bool value = false)  {
        string id = std::format("CheckBox#{}", sCheckBoxCounter++);

        auto *component = GetChild(id);
        if (component) return component->As<CheckBox>();

        // ToDo: ... was horizontal group
        auto checkbox = CreateScope<CheckBox>(id);
        checkbox->Value = value;
        checkbox->Layout = Layout::Horizontal;
        checkbox->Alignment = { 0.0f, 1.0f };
        checkbox->Color = this->Style.ColorText;
        checkbox->Text = text;
        checkbox->FocusStyle = FocusStyle::Underline;
        //if (checkbox->As<Control>()->HasFocus(FocusType::Mouse) && checkbox->Active) value = !value;
        checkbox->Padding = { 4.0f, 4.0f, 4.0f, 4.0f };
        checkbox->Spacing = 8.0f;
        checkbox->Stretch = { 1.0f, 0.0f };
        checkbox->Font = this->Style.Font.get();
        if (checkbox->Font) {
            checkbox->MinSize = checkbox->Font->GetSize(text);
            checkbox->MinSize.Width += (checkbox->Padding.Left + checkbox->Padding.Right) + 32.0f;
            checkbox->MinSize.Height += checkbox->Padding.Top + checkbox->Padding.Bottom + 4.0f;
            checkbox->Position.X -= checkbox->Padding.Left * 2 + checkbox->Offset.X;
            checkbox->Position.Y -= checkbox->Padding.Top + checkbox->Offset.Y;
        } else {
            LogError("Something went wrong in the style tree!");
        }

        //auto canvas = checkbox->CreateContainer(Layout::Stack);
        //auto rect = canvas->CreateControl(ControlType::Rect)->As<UIRectOld>();
        //rect->Type = ControlType::Rect;
        //rect->Color = this->Style.ColorFrame;
        //rect->Size = { 16.0f, 16.0f };
        checkbox->OutherColor = this->Style.ColorFrame;
        checkbox->OutherSize = { 16.0f, 16.0f };

        if (value) {
        //    auto rect = canvas->CreateControl(ControlType::Rect)->As<UIRectOld>();
        //    rect->Type = ControlType::Rect;
        //    rect->Alignment = { 0.5f, 0.5f };
        //    rect->Color = this->Style.ColorPrimary;
        //    rect->Size = { 10.0f, 10.0f };
            checkbox->InnerColor = this->Style.ColorPrimary;
            checkbox->InnerSize = { 10.0f, 10.0f };
        }

        auto result = checkbox.get();
        AddChild(std::move(checkbox));
        Parent = this;
        return result;
    }
    Image *CreateImage(string_view path)  {
        string id = std::format("Image#{}", sImageCounter++);

        auto *component = GetChild(id);
        if (component) return component->As<Image>();

        auto image = CreateScope<Image>(id);
        image->Stretch = { 1.0f, 1.0f };
        //image->Data = Texture::Create(TextureProperties(), path);

        auto result = image.get();
        AddChild(std::move(image));
        Parent = this;
        return result;
    }
    Label *CreateLabel(string_view text, Font *font = nullptr, const string &uniqueId = "")  {
        string id = uniqueId.empty() ? std::format("Label#{}", sLabelCounter++) : uniqueId;

        auto *component = GetChild(id);
        if (component) return component->As<Label>();

        auto label = CreateScope<Label>(id);
        label->Text = text;
        label->Color = this->Style.ColorText;
        if (font) {
            label->Font = font;
        } else {
            label->Font = this->Style.Font.get();
        }
        label->Alignment = { 0.0f, 1.0f };
        label->Padding = { 2.0f, 0.0f, 2.0f, 0.0f };
        if (label->Font) {
            label->MinSize = label->Font->GetSize(text);
            label->MinSize.Width += label->Padding.Left + label->Padding.Right;
            label->MinSize.Height += label->Padding.Top + label->Padding.Bottom;
            label->Position.X -= label->Padding.Left + label->Offset.X;
            label->Position.Y -= label->Padding.Top + label->Offset.Y;
        } else {
            LogError("Something went wrong in the style tree!");
        }

        auto result = label.get();
        AddChild(std::move(label));
        Parent = this;
        return result;
    }
    Container *CreateScrollView(float maxHeight)  {
        string id = std::format("ScrollView#{}", sScrollViewCounter);

        auto *component = GetChild(id);
        if (component) {
            sScrollViewCounter++;
            return component->As<Container>();
        }

        auto scrollview = CreateContainer(Layout::Vertical, id);
        scrollview->Clip = true;
        scrollview->Expand = false;
        scrollview->Type = ComponentType::ScrollView;
        scrollview->Padding = { 6.0f, 6.0f, 6.0f, 6.0f };
        scrollview->Stretch = { 1.0f, 1.0f };
        scrollview->Spacing = 2.0f;
        scrollview->MaxSize.Height = maxHeight;

        // ToDo: ScrollBar currently missing...

        sScrollViewCounter++;
        return scrollview;
    }
    Slider *CreateSlider(float value, float min = 0.0f, float max = 128.0f)  {
        string id = std::format("Slider#{}", sSliderCounter++);

        auto *component = GetChild(id);
        if (component) return component->As<Slider>();

        auto slider = CreateScope<Slider>(id);
        slider->Alignment = { 0.5f, 0.5f };
        slider->Stretch = { 1.0f, 0.0f };
        slider->Color = { 0.5f, 0.5f, 0.5f, 1.0f };
        slider->Size = { 0.0f, 2.0f };
        //{
        //BeginGroup(Layout::Stack);
        //Rectangle({ 0, 2 }, { 0.5f, 0.5f, 0.5f, 1.0f });
        //SetAlign(0.5f, 0.5f);
        //SetStretch(1, 0);
        //EndGroup();
        //return 0.0f;
        //
        ////HmGuiData *data = GetData(self.group);
        ////if (GroupHasFocus(FocusType::Scroll)) {
        ////    Vector2Di scroll; //Input_GetMouseScroll(&scroll);
        ////    auto delta = Input::GetMouseWheelDelta();
        ////    scroll.y = delta;
        ////    data->Offset.Y -= 10.0f * scroll.y;
        ////}
        //
        ////float maxScroll = std::max(0.0f, data->MinSize.Height - data->Size.Height);
        ////data->Offset.Y = std::clamp(data->Offset.Y, 0.0f, maxScroll);
        //
        ////EndGroup();
        //
        ////BeginGroup(Layout::Vertical);
        ////SetStretch(0, 1);
        ////SetSpacing(0);
        ////if (maxScroll > 0) {
        ////    float handleSize = data->Size.Height * (data->Size.Height / data->MinSize.Height);
        ////    float handlePos = std::lerp(0.0f, data->Size.Height - handleSize, data->Offset.Y / maxScroll);
        ////    Rectangle({ 4.0f, handlePos }, { 0.0f, 0.0f, 0.0f, 0.0f });
        ////    Rectangle({ 4.0f, handleSize }, self.style->ColorFrame);
        ////} else {
        ////    Rectangle({ 4.0f, 16.0f }, { 0.0f, 0.0f, 0.0f, 0.0f });
        ////}

        auto result = slider.get();
        AddChild(std::move(slider));
        Parent = this;
        return result;
    }
    Seperator *CreateSeperator() {
        // ToDo: Check also line shader "vertex/ui" "fragment/ui/line"
        // ToDo: Additive BlendMode
        //const float pad = 64.0f;
        //float xMin = Min(e->p1.x, e->p2.x) - pad;
        //float yMin = Min(e->p1.y, e->p2.y) - pad;
        //float xMax = Max(e->p1.x, e->p2.x) + pad;
        //float yMax = Max(e->p1.y, e->p2.y) + pad;
        //float sx = xMax - xMin;
        //float sy = yMax - yMin;
        // UIRenderer::Line(e->Start, e->Stop, e->Color);
        return {};
    }

    // Methods
    void AddChild(Scope<Component> child) {
        child->Parent = this;
        mChildren.push_back(std::move(child));
    }
    void ComputeSize() {
        // Calculate the minimum size for each child
        for (const auto &child : mChildren) {
            if (child->Type < ComponentType::Containers) {
                child->As<Container>()->ComputeSize();
            }
        }
        MinSize = { 0.0f, 0.0f };

        // Calculate the minimum size of the current container based on its children
        for (const auto &child : mChildren) {
            switch (Layout) {
                // For stacking layout, choose the maximum width and height among children
                case Layout::Stack: {
                    MinSize.Width = std::max(MinSize.Width, child->MinSize.Width);
                    MinSize.Height = std::max(MinSize.Height, child->MinSize.Height);
                    break;
                }
                // For vertical layout, add up children's heights and choose maximum width
                case Layout::Vertical: {
                    MinSize.Width = std::max(MinSize.Width, child->MinSize.Width);
                    MinSize.Height += child->MinSize.Height;
                    if (child.get() != GetFirstChild()) MinSize.Height += Spacing;
                    break;
                }
                // For horizontal layout, add up children's widths and choose maximum height
                case Layout::Horizontal: {
                    MinSize.Width += child->MinSize.Width;
                    MinSize.Height = std::max(MinSize.Height, child->MinSize.Height);
                    if (child.get() != GetFirstChild()) MinSize.Width += Spacing;
                    break;
                }
            }
        }

        // Add padding to the minimum size
        MinSize.Width += Padding.Left + Padding.Right;
        MinSize.Height += Padding.Top + Padding.Bottom;

        // Clamp the calculated minimum size to the specified maximum size
        MinSize.Width = std::min(MinSize.Width, MaxSize.Width);
        MinSize.Height = std::min(MinSize.Height, MaxSize.Height);
    }
    void Draw() override;
    void DebugPrint(size_t level = 0) const {
        Log("{}{}{} [ childs: {} | Position: 'x={}, y={}' | Size '{}x{}' ]",
            level == 0 ? "" : string(level * 2 - 2, ' '),
            level == 0 ? "" : "◌ ", mID,
            mChildren.size(),
            Position.X, Position.Y,
            Size.Width, Size.Height
        );
        for (const auto &child : mChildren) {
            if (child->Type < ComponentType::Containers) {
                child->As<Container>()->DebugPrint(level + 1);
            } else {
                child->As<Control>()->DebugPrint(level);
            }
        }
    }
    void LayoutGroup() {
        auto pos = Position;
        auto size = Size;
        auto extra = 0.0f;
        auto totalStretch = 0.0f;

        // Adjust the position and size for padding and offset
        pos.X += Padding.Left + Offset.X;
        pos.Y += Padding.Top + Offset.Y;
        size.Width -= Padding.Left + Padding.Right;
        size.Height -= Padding.Top + Padding.Bottom;

        // Calculate extra space and total stretch based on expand settings
        if (Expand) {
            if (Layout == Layout::Vertical) {
                extra = Size.Height - MinSize.Height;
                for (auto &child : mChildren) {
                    totalStretch += child->Stretch.Y;
                }
            } else if (Layout == Layout::Horizontal) {
                extra = Size.Width - MinSize.Width;
                for (auto &child : mChildren) {
                    totalStretch += child->Stretch.X;
                }
            }
        }

        // Distribute extra space among stretched children
        if (totalStretch > 0.0f) extra /= totalStretch;

        auto s { 0.0f };
        for (auto &child : mChildren) {
            switch (Layout) {
                case Layout::None: {
                    LayoutControl(child, child->Position, size);
                    break;

                }
                case Layout::Stack: {
                    LayoutControl(child, pos, size);
                    break;
                }
                case Layout::Vertical: {
                    // Calculate height based on child's minimum size and stretch factor
                    // Update child's position and size
                    // Move the position for the next child vertically
                    s = child->MinSize.Height;
                    if (extra > 0) s += child->Stretch.Y * extra;
                    LayoutControl(child, pos, { size.Width, s });
                    pos.Y += child->Size.Height + Spacing;
                    break;
                }
                case Layout::Horizontal: {
                    // Calculate width based on child's minimum size and stretch factor
                    // Update child's position and size
                    // Move the position for the next child horizontally
                    s = child->MinSize.Width;
                    if (extra > 0) s += child->Stretch.X * extra;
                    LayoutControl(child, pos, { s, size.Height });
                    pos.X += child->Size.Width + Spacing;
                    break;
                }
            }

            // Recursively layout child containers or windows
            if (child->Type < ComponentType::Containers) child->As<Container>()->LayoutGroup();
        }
    }
    void LayoutControl(const Scope<Component> &component, const Ultra::Position &position, const Ultra::Size &size) {
        // Update the position and size of a control based on its alignment and stretch factors
        // Calculate new size considering stretch factors and available space

        component->Position = position;
        component->Size = component->MinSize;


        component->Size.Width += component->Stretch.X * (size.Width - component->MinSize.Width);
        component->Size.Height += component->Stretch.Y * (size.Height - component->MinSize.Height);
        component->Position.X += component->Alignment.X * (size.Width - component->Size.Width);
        component->Position.Y += component->Alignment.Y * (size.Height - component->Size.Height);
    }
    void MoveToTop(const string &id) {
        auto it = std::find_if(mChildren.begin(), mChildren.end(), [&id](const auto &child) {
            return child->GetID() == id;
        });
        if (it != mChildren.end()) {
            std::rotate(it, it + 1, mChildren.end());
        }
    }
    static void Update() {
        sContainerCounter = 0;
        sButtonCounter = 0;
        sCheckBoxCounter = 0;
        sImageCounter = 0;
        sLabelCounter = 0;
        sScrollViewCounter = 0;
        sSliderCounter = 0;
    }

    // Accessors
    Component *GetParent() {
        return Parent;
    }
    Component *GetChild(const string &id) {
        if (mID == id) return this;
        for (const auto &child : mChildren) {
            if (child->Type < ComponentType::Containers) {
                if (auto *const result = child->As<Container>()->GetChild(id)) {
                    return result;
                }
            } else {
                if (child->GetID() == id) {
                    return child.get();
                }
            }
        }
        return nullptr;
    }
    vector<Scope<Component>> &GetChildren() {
        return mChildren;
    }
    Component *GetFirstChild() {
        if (mChildren.empty()) return nullptr;
        return mChildren.front().get();
    }
    Component *GetLastChild() {
        if (mChildren.empty()) return nullptr;
        return mChildren.back().get();
    }

    // Mutators
    void UpdateClipRect() {
        //ClipRect.Right = Position.X;
        //ClipRect.Bottom = Position.Y;

        //?rect->Upper = { g->Position, g->Size };

        //ClipRect->Left = std::min(rect->Upper.X, rect->Previous->Upper.X);
        //ClipRect->Top = std::min(rect->Upper.Y, rect->Previous->Upper.Y);
        //ClipRect->Right = std::max(rect->Lower.Y, rect->Previous->Lower.Y);
        //ClipRect->Bottom = std::max(rect->Lower.X, rect->Previous->Lower.X);
    }

protected:
    vector<Scope<Component>> mChildren;

public:
    Style Style {};

    bool Expand { true };

    Ultra::Stretch TotalStretch {};

private:
    // Counter for ComponentIDs
    inline static size_t sContainerCounter {};
    inline static size_t sButtonCounter {};
    inline static size_t sCheckBoxCounter {};
    inline static size_t sImageCounter {};
    inline static size_t sLabelCounter {};
    inline static size_t sScrollViewCounter {};
    inline static size_t sSliderCounter {};
};

///
/// @brief This class manages the GUI layout.
///
class UILayoutManager {
public:
    UILayoutManager():
        mRoot(CreateScope<Container>("Root")) {
    }
    ~UILayoutManager() = default;

    // Accessors
    Container *GetRoot() {
        return mRoot.get();
    }
    Component *GetComponent(const string &id) {
        auto element = mRoot->GetChild(id);
        return element;
    }

    // Methods
    void AddContainer(Scope<Container> id) {
        mRoot->AddChild(std::move(id));
    }
    void DebugPrint(size_t level = 0) {
        mRoot->DebugPrint(level);
    }

private:
    Scope<Container> mRoot;
    unordered_map<string, Component> mComponents {};
};

}

///
/// @brief Public
///
export namespace Ultra::UI {

///
/// @brief Hybrid-Mode-GUI, a combination of immediate mode and retained mode.
/// It's simple but effective and has a built-in automatic layout system.
/// @note This implementation is inspired by Josh Parnell
/// @link http://forums.ltheory.com/viewtopic.php?t=6582
///
class HmGui {
    // Constructors and Destructor
    HmGui() {
        mUILayout = CreateScope<UILayoutManager>();
        auto root = mUILayout->GetRoot();

        //root->Active = Input::GetMouseButtonState(MouseButton::Left);
        root->Clip = true;
        root->Position = { 0.0f, 0.0f };
        root->Size = { 1280.0f, 1024.0f };
        root->Style.Font = CreateReference<Font>("Rajdhani", 14);

        mViewport = Viewport::Create({ 0, 0, 1280, 1024, true });

        mFontExo2Bold = CreateScope<Font>("Exo2Bold", 30);
        mFontFiraMono = CreateScope<Font>("FiraMono", 10);
        mFontRajdhani = CreateScope<Font>("Rajdhani", 18);
    };
    HmGui(const HmGui &hmgui) = delete;
    HmGui(HmGui &&hmgui) = delete;
    HmGui &operator=(const Logger &) = delete;
    HmGui &operator=(Logger &&) noexcept = delete;
    ~HmGui() = default;

    // Global Instance
    static HmGui &Instance() {
        static HmGui instance;
        return instance;
    }

public:

    /// @brief Creates a window or returns an existing window
    /// @param title Specify the window title.
    /// @return Returns a non owning pointer to the object.
    static Container *CreateWindow(string_view title) {
        string id = std::format("Window#{}", sWindowCounter);

        auto &manager = Instance().mUILayout;
        auto *component = manager->GetComponent(id);
        if (component) {
            sWindowCounter++;
            return component->As<Container>();
        }

        // Container
        auto window = CreateScope<Container>(id);
        window->Type = ComponentType::Window;
        window->Layout = Layout::Vertical;
        window->Expand = true;
        window->FrameOpacity = 0.97f;
        window->Padding = { 8.0f, 0.0f, 8.0f, 8.0f };
        window->Style.Font = manager->GetRoot()->Style.Font;

        // TitleBar
        auto titleBarId = std::format("TitleBar#{}", sWindowCounter);
        auto titleBar = window->CreateContainer(Layout::Stack, titleBarId);
        titleBar->Clip = true;
        titleBar->Padding = { 8.0f, 8.0f, 8.0f, 8.0f };
        titleBar->Stretch = { 1.0f, 1.0f };

        auto titleLabelId = std::format("TitleLabel#{}", sWindowCounter);
        auto titleLabel = titleBar->CreateLabel(title, nullptr, titleLabelId);
        titleLabel->Color = { 1.0f, 1.0f, 1.0f, 0.3f };
        titleLabel->Alignment = { 0.5f, 0.5f };

        auto *result = window.get();
        manager->AddContainer(std::move(window));
        sWindowCounter++;
        return result;
    }

    // Draws the GUI
    static void Draw() {
        if (mUpdateInput) {
            auto [mouseX, mouseY] = Input::GetMousePosition();
            sLastMousePosition.X = mouseX;
            sLastMousePosition.Y = mouseY;

            sMouseClicked = Input::GetMouseButtonStateDelta(MouseButton::Left);
            sMousePressed = Input::GetMouseButtonState(MouseButton::Left);

            if (sMouseClicked && sMousePressed) {
                sMouseDragStartPosition.X = mouseX;
                sMouseDragStartPosition.Y = mouseY;
            }

            if (sMousePressed) {
                sMouseDragCurrentPosition.X = mouseX - sMouseDragStartPosition.X;
                sMouseDragCurrentPosition.Y = mouseY - sMouseDragStartPosition.Y;
            } else {
                sMouseDragStartPosition = {};
                sMouseDragCurrentPosition = {};
            }
        }

        UIRenderer::Begin(Instance().mViewport);
        auto root = Instance().mUILayout->GetRoot();

        if (static bool once = true) {
            root->DebugPrint();
            root->ComputeSize();
            root->LayoutGroup();
            once = false;
        }
        root->Draw();

        sWindowCounter = 0;
        Container::Update();
        UIRenderer::End();
        UIRenderer::Draw();
    }

    // Shows some demo windows and is also used for tests
    static void ShowDemo(Timestamp deltaTime) {
        #pragma warning(disable: 4189)
        /// @brief Here we test new API styles, until we find the best one...
        mFrames++;
        mDeltaDelay += deltaTime;
        mUpdateInput = false;
        mUpdateInput = true;
        if (mDeltaDelay > 0.1f) {
            mUpdateInput = true;
        }
        if (mDeltaDelay > 1.0f) {
            mFPS = mFrames;
            mMSPF = 1000.0 / mFrames;
            mFrames = 0;
            mDeltaDelay = 0;
        }

        //
        // Simple Window Test
        //
        {
            auto window = CreateWindow("This is a test Window!");
            window->Alignment = { 0.5f, 0.5f };

            // TabBar
            {
                auto tabs = window->CreateContainer(Layout::Horizontal);
                tabs->Stretch = { 1.0f, 1.0f };

                auto tabsButtonLeft = tabs->CreateButton(" < ");
                tabsButtonLeft->Stretch = { 0.0f, 1.0f };
                auto tabsButton1 = tabs->CreateButton(" Tab1 ");
                auto tabsButton2 = tabs->CreateButton(" Tab2 ");
                auto tabsButton3 = tabs->CreateButton(" Tab3 ");
                auto tabsButtonRight = tabs->CreateButton(" > ");
                tabsButtonRight->Stretch = { 0.0f, 1.0f };
            }

            // Buttons and CheckBoxes
            {
                auto buttons = window->CreateContainer(Layout::Horizontal);
                buttons->Stretch = { 1.0f, 0.0f };

                // Column A
                {
                    auto group = buttons->CreateContainer(Layout::Vertical);
                    group->Padding = { 4.0f, 8.0f, 4.0f, 8.0f };
                    group->Stretch = { 1.0f, 1.0f };

                    auto welcome = group->CreateLabel("Welcome to...");
                    welcome->Alignment = { 0.5f, 0.5f };

                    auto caption = group->CreateLabel("~ Hybrid Mode ~", Instance().mFontExo2Bold.get());
                    caption->Color = { 1.0f, 0.0f, 0.3f, 1.0f };
                    caption->Alignment = { 0.5f, 0.5f };

                    auto subscript = group->CreateLabel("GUI");
                    subscript->Alignment = { 0.5f, 0.5f };

                    auto buttonNotSoStretchy = group->CreateButton("Not-So-Stretchy");
                    buttonNotSoStretchy->Stretch = { 1.0f, 0.0f };
                    auto buttonStretchy = group->CreateButton("Stretchy");
                    buttonStretchy->Stretch = { 1.0f, 1.0f };

                    auto subGroup = group->CreateContainer(Layout::Horizontal);
                    subGroup->Stretch = { 1.0f, 1.0f };
                    for (auto i = 1; i < 3; i++) {
                        auto innerGroup = subGroup->CreateContainer(Layout::Vertical);
                        innerGroup->Stretch = { 1.0f, 1.0f };
                        for (auto j = 1; j < 3; j++) {
                            auto button = innerGroup->CreateButton(":)");
                        }
                    }
                }

                // Column B
                {
                    auto group = buttons->CreateContainer(Layout::Vertical);
                    //group->Alignment = { 0.0f, 1.0f };
                    group->Padding = { 4.0f, 8.0f, 4.0f, 8.0f };
                    group->Stretch = { 1.0f, 1.0f };

                    auto eventButton = group->CreateButton("-- OPT 1 --");
                    eventButton->Callback = [&]() {
                        LogInfo("Button '-- OPT 1 --' clicked!");
                    };

                    auto silentButton = group->CreateButton("-- OPT 2 --");

                    auto checkBoxA = group->CreateCheckBox("Yas", true);
                    auto checkBoxB = group->CreateCheckBox("Nope", false);
                    auto checkBoxC = group->CreateCheckBox("Possibly?", false);

                    auto doneButton = group->CreateButton("DONE");
                }

                // Column C
                {
                    auto group = buttons->CreateContainer(Layout::Vertical);
                    group->Padding = { 4.0f, 8.0f, 4.0f, 8.0f };
                    group->Stretch = { 1.0f, 1.0f };

                    for (auto i = 1; i < 9; i++) {
                        auto autoGroup = group->CreateContainer(Layout::Horizontal);
                        autoGroup->Alignment = { 0.5f, 0.5f };

                        for (auto j = 1; j < i; j++) {
                            auto result = std::format("{}.{}", i, j);
                            auto button = autoGroup->CreateButton(result);
                        }
                    }
                }

                // Column D
                {
                    auto view = buttons->CreateScrollView(256.0f);
                    view->Spacing = 8.0f;
                    for (auto &[entry, list] : gCheckList) {
                        auto caption = view->CreateLabel(entry, Instance().mFontRajdhani.get());
                        caption->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
                        auto group = view->CreateContainer(Layout::Vertical);
                        group->Spacing = 2.0f;
                        group->Padding.Left = 12.0f;
                        for (auto &[key, value] : list) {
                            auto check = group->CreateCheckBox(key, value);
                        }
                    }
                }
            }


            // Editor
            {
                auto editorTitle = window->CreateLabel("水 Behöld, the codez! \\o/");
                auto editors = window->CreateContainer(Layout::Horizontal);
                editors->Stretch = { 1.0f, 1.0f };
                for (auto i = 0; i < 2; i++) {
                    auto scrollView = editors->CreateScrollView(200);
                    auto lines = String::Split(gCodeExample, '\n');
                    for (auto &line : lines) {
                        auto text = scrollView->CreateLabel(line, Instance().mFontFiraMono.get());
                        text->Color = { 0.1f, 0.5f, 1.0f, 1.0f };
                    }
                }
            }
        }

        //
        // ToDo List Window
        //
        {
            auto window = CreateWindow("ToDo List");
            window->Alignment = { 1.0f, 0.0f };

            auto view = window->CreateScrollView(256.0f);
            view->Spacing = 8.0f;
            for (auto &[entry, list] : gCheckList) {
                auto caption = view->CreateLabel(entry, Instance().mFontRajdhani.get());
                caption->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
                auto group = view->CreateContainer(Layout::Vertical);
                group->Spacing = 2.0f;
                group->Padding.Left = 12.0f;
                for (auto &[key, value] : list) {
                    auto check = group->CreateCheckBox(key, value);
                }
            }
        }

        //
        // Metrics Window
        //
        {
            auto window = CreateWindow("Metrics");
            window->Alignment = { 1.0f, 1.0f };

            auto container = window->CreateContainer(Layout::Vertical);
            container->Alignment = { 1.0f, 1.0f };

            auto caption = container->CreateLabel("Application Statistics");
            auto fps = container->CreateLabel("frames/s: ####.##");
            fps->Text = std::format("frames/s: {:.2f}", mFPS);
            auto msf = container->CreateLabel("ms/Frame: ####.##");
            msf->Text = std::format("ms/Frame: {:.2f}", mMSPF);
        }
        #pragma warning(default: 4189)
    }

private:
    // Counter for ComponentIDs
    inline static size_t sWindowCounter {};

    // Instances
    Scope<UILayoutManager> mUILayout {};
    Scope<Viewport> mViewport;

    // Test
    Scope<Font> mFontExo2Bold = nullptr;
    Scope<Font> mFontFiraMono = nullptr;
    Scope<Font> mFontRajdhani = nullptr;
    inline static thread_local double mFrames {};
    inline static thread_local double mFPS {};
    inline static thread_local double mMSPF {};
    inline static thread_local double mDeltaDelay {};

public:
    inline static thread_local bool mUpdateInput {};
    inline static thread_local bool sMouseClicked {};
    inline static thread_local bool sMousePressed {};

    inline static thread_local Position sLastMousePosition {};
    inline static thread_local Position sMouseDragCurrentPosition {};
    inline static thread_local Position sMouseDragStartPosition {};
};

}

module :private;

///
/// @brief The internal Magic
///

namespace Ultra::UI {

///
/// @brief Switches
///

constexpr bool DrawLayoutFrames = false;
constexpr bool DrawGroupFrames = false;
constexpr bool EnableWindowDragging = true;


void Container::Draw() {
    UIRenderer::BeginLayer(Position, Size, Clip);
    for (auto &element : GetChildren()) {
        switch (element->Type) {
            // Container
            case ComponentType::Window: {
                if constexpr (EnableWindowDragging) {
                    auto hovered = element->Hovered(HmGui::sLastMousePosition);
                    if (hovered && HmGui::sMousePressed) {
                        element->Offset = HmGui::sMouseDragCurrentPosition;
                    }
                    if (!HmGui::sMousePressed && element->Offset.X != 0 && element->Offset.Y != 0) {
                        element->Position.X += element->Offset.X;
                        element->Position.Y += element->Offset.Y;
                        element->Offset = {};
                    }
                    element->As<Container>()->LayoutGroup();
                }

                const auto color = Ultra::Color(0.1f, 0.12f, 0.13f, 1.0f);
                auto position = element->Position;
                position.X += element->Offset.X;
                position.Y += element->Offset.Y;
                UIRenderer::Panel(position, element->Size, color, 8.0f, element->FrameOpacity);
                element->As<Container>()->Draw();
                break;
            }
            case ComponentType::ScrollView:  {
                auto hovered = element->Hovered(HmGui::sLastMousePosition);
                float offsetY {};
                if (hovered) {
                    auto scrollDelta = Input::GetMouseWheelDelta();
                    if (scrollDelta) {
                        offsetY -= 12.0f * scrollDelta;
                    }
                }
                float maxScroll = std::max(0.0f, element->MinSize.Height - element->Size.Height) + 12.0f;
                element->Offset.Y = std::clamp(offsetY, 0.0f, maxScroll);
                element->As<Container>()->LayoutGroup();

                auto position = element->Position;
                position.X += element->Size.Width;
                if (maxScroll > 0) {
                    float handleSize = element->Size.Height * (element->Size.Height / element->MinSize.Height);
                    float handlePos = std::lerp(0.0f, element->Size.Height - handleSize, element->Offset.Y / maxScroll);
                    UIRenderer::Rect(position, { 6.0, handlePos }, {}, false);
                    UIRenderer::Rect(position, { 6.0, handleSize }, { 0.1f, 0.1f, 0.1f, 0.5f }, false);
                } else {
                    UIRenderer::Rect(position, { 6.0f, 16.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, false);
                }
                if constexpr (DrawLayoutFrames) {
                    UIRenderer::DrawBorder(3.0f, element->Position, element->Size, { 1.0f, 1.0f, 1.0f, 1.0f });
                }
                element->As<Container>()->Draw();
                break;
            }

            case ComponentType::Panel: [[fallthrough]];
            case ComponentType::Container: {
                if constexpr (DrawLayoutFrames) {
                    UIRenderer::DrawBorder(3.0f, element->Position, element->Size, { 1.0f, 1.0f, 1.0f, 1.0f });
                }
                element->As<Container>()->Draw();
                break;
            }

            // Controls
            default: {
                element->Draw();
                break;
            }
        }
    }
    UIRenderer::EndLayer();
}


Scope<Button> Button::Create(const string &id, string_view text) {
    auto button = CreateScope<Button>(id);
    button->Text = text;

    button->Layout = Layout::Stack;
    button->Alignment = { 0.5f, 0.5f };
    button->Stretch = { 1.0f, 1.0f };
    button->FocusStyle = FocusStyle::Fill;
    button->FrameOpacity = 0.5f;
    button->Padding = { 8.0f, 8.0f, 8.0f, 8.0f };
    return std::move(button);
}

void Button::Draw() {
    auto hovered = Hovered(HmGui::sLastMousePosition);
    if (hovered) {
        if (HmGui::sMouseClicked) {
            if (Callback) Callback();
        }

        if (HmGui::sMousePressed) {
            const auto color = Ultra::Color(0.2f, 0.7f, 1.0f, 1.0f);
            UIRenderer::Panel(Position, Size, color, 0.0f, 1.0f);
            // ToDo: Execute bound lambda
        } else {
            const auto color = Ultra::Color(0.1f, 0.5f, 1.0f, 1.0f);
            UIRenderer::Panel(Position, Size, color, 0.0f, 1.0f);
        }
    } else {
        const auto color = Ultra::Color(0.15f, 0.15f, 0.15f, 0.8f);
        UIRenderer::Panel(Position, Size, color, 0.0f, FrameOpacity);
    }

    //if constexpr (DrawLayoutFrames) {
    //    UIRenderer::DrawBorder(1.0f, Position, Size, { 1.0f, 1.0f, 1.0f, 1.0f });
    //}

    Ultra::Position position = {
        (Position.X + 8.0f + (Size.Width - MinSize.Width) * Alignment.X),
        (Position.Y + 4.0f + (Size.Height) * Alignment.Y),
    };
    UIRenderer::Text(position, Text, Color, Font);

}


void CheckBox::Draw() {
    auto hovered = Hovered(HmGui::sLastMousePosition);
    if (hovered) {
        auto color = Ultra::Color(0.3f, 0.3f, 0.3f, 0.5f);
        UIRenderer::Rect(Position, Size, color, true);

        if (HmGui::sMouseClicked) Value = !Value;
    }

    //if constexpr (DrawLayoutFrames) {
    //    UIRenderer::DrawBorder(1.0f, native->Position, native->Size, { 1.0f, 1.0f, 1.0f, 1.0f });
    //}

    // CheckBox Label
    Ultra::Position position = { Position.X + 8.0f, (Position.Y + MinSize.Height / 2 + 4.0f) };
    UIRenderer::Text(position, Text, Color, Font);

    // CheckBox Outer Rectangle
    position.X = Position.X + Size.Width - OutherSize.Width - 4.0f;
    position.Y -= 12.0f;
    UIRenderer::Rect(position, OutherSize, OutherColor, true);

    // CheckBox Inner Rectangle
    position.X += 3.0f;
    position.Y += 3.0f;
    if (Value) {
        UIRenderer::Rect(position, InnerSize, InnerColor, false);
    } else {
        UIRenderer::Rect(position, InnerSize, {}, false);
    }


}


void Label::Draw() {
    //if constexpr (DrawLayoutFrames) {
    //    UIRenderer::DrawBorder(1.0f, native->Position, native->Size, { 1.0f, 1.0f, 1.0f, 1.0f });
    //}


    Ultra::Position position = { Position.X, Position.Y + MinSize.Height };

    UIRenderer::Text(position, Text, Color, Font);
}


void Image::Draw() {
    //UIRenderer::Image(Position, Size, CreateReference<Texture>(Data));
}


void Seperator::Draw() {
    return;
    // ToDo: Check also line shader "vertex/ui" "fragment/ui/line"
    // ToDo: Additive BlendMode
    //const float padding = 64.0f;
    //float minX = std::min(native->Start.X, native->Stop.X) - padding;
    //float minY = std::min(native->Start.Y, native->Stop.Y) - padding;
    //float maxX = std::max(native->Start.X, native->Stop.X) + padding;
    //float maxY = std::max(native->Start.Y, native->Stop.Y) + padding;
    //float width = maxX - minX;
    //float height = maxY - minY;
    // UIRenderer::Line(e->Start, e->Stop, e->Color);
}


void Slider::Draw() {
}

}

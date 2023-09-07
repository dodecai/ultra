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

enum class ComponentType {
    // Containers
    None        = 0x00,
    Container   = 0x01,
    Window      = 0x02,
    Panel       = 0x03,
    Popup       = 0x04,
    ScrollView  = 0x05,
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
    Tree        = 0x2C,
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

enum class FocusStyle {
    None,
    Fill,
    Outline,
    Scroll,
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

struct Alignment {
    float X {};
    float Y {};
};

struct ClipArea {
    Position TopLeft {};
    Position BottomRight {};
};

struct Padding {
    float Left {};
    float Top {};
    float Bottom {};
    float Right {};
};

struct Stretch {
    float X {};
    float Y {};
};

struct Style {
    Color ColorDebugBorder  { 1.0f, 1.0f, 1.0f, 0.5f };

    Color ColorPrimary      { 0.1f, 0.5f, 1.0f, 1.0f };
    Color ColorSecondary    { 0.1f, 1.0f, 0.5f, 1.0f };
    Color ColorTernary      {};
    Color ColorFrame        { 0.1f, 0.1f, 0.1f, 0.5f };
    Color ColorText         { 1.0f, 1.0f, 1.0f, 1.0f };

    Color ColorFillNone             { 0.15f, 0.15f, 0.15f, 0.8f };  // Panel
    Color ColorFillPressed          { 0.2f, 0.7f, 1.0f, 1.0f };     // Panel
    Color ColorFillHovered          { 0.1f, 0.5f, 1.0f, 1.0f };     // Panel
    Color ColorFocusUnderlineActive { 0.4f, 0.5f, 0.5f, 1.0 };      // Rect + FrameOpacity
    Color ColorFocusUnderline       { 0.3f, 0.3f, 0.3f, 0.5 };      // Rect


    Color ColorFocusNone            { 0.1f, 0.12f, 0.13f, 1.0f };   // Panel
    Color ColorFocusOutline         { 0.1f, 0.5f, 1.0f, 1.0f };     // Rect

    Reference<Font> Font { nullptr };

    float Spacing { 6.0f };
};

///
/// @brief Component: Base for Containers and Controls
///

class Component {
public:
    Component(const string &id, const ComponentType &type):
        ID(id),
        Hash(mHasher(id)),
        Type(type) {
    }
    virtual ~Component() = default;

    // Comparison
    bool operator==(const Component &other) const {
        return Hash == other.Hash;
    }
    bool operator==(const string &id) const {
        return Hash == mHasher(id);
    }

    // Converters
    template<typename T>
    T *As() {
        return reinterpret_cast<T *>(this);
    }

    // Interface
    virtual void Draw() {}
    virtual void Update() {}

    // Methods
    void CalculateLayout(const Position &position, const Size &size) {
        // Update position and size of a control based on its alignment and stretch factors
        Position = position;
        Size = MinSize;

        Size.Width += Stretch.X * (size.Width - MinSize.Width);
        Size.Height += Stretch.Y * (size.Height - MinSize.Height);

        Position.X += Alignment.X * (size.Width - Size.Width);
        Position.Y += Alignment.Y * (size.Height - Size.Height);
    }
    bool Hovered(const Position &hoverPosition) const {
        return (
            hoverPosition.X >= Position.X &&
            hoverPosition.X <= Position.X + Size.Width &&
            hoverPosition.Y >= Position.Y &&
            hoverPosition.Y <= Position.Y + Size.Height
        );
    }

public:
    const Component *Parent {};
    const string ID;
    const size_t Hash;
    const ComponentType Type;

    // States
    bool Active {};
    bool Disabled {};
    bool Focused {};
    bool Interactive { true };
    bool Visible { true };

    // Style
    Color Color {};
    FocusStyle FocusStyle {};
    float FrameOpacity { 1.0f };

    // Transformation
    Ultra::Position Position {};
    Ultra::Size Size {};
    Ultra::Position Offset {};
    Ultra::Size MinSize {};
    Alignment Alignment {};
    Stretch Stretch {};
    Padding Padding {};
    Ultra::Size OriginalSize {};

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

    template <typename T, typename... Args> requires std::is_base_of_v<Control, T>
    static Scope<T> Create(const std::string &id, Args&&... args) {
        return CreateScope<T>(id, std::forward<Args>(args)...);
    }

    virtual void DebugPrint(size_t indent) const {
        Log("{}{}{}", indent == 0 ? "" : string(indent * 2, ' '), indent == 0 ? "" : "◌ ", ID);
    }

private:
    Ultra::Position CanvasPosition {};
    Ultra::Size CanvasSize {};
};

class Button: public Control {
public:
    Button(const string &id, const string &text):
        Control(id, ComponentType::Button),
        Text(text) {
        Alignment = { 0.5f, 0.5f };
        FocusStyle = FocusStyle::Fill;
        FrameOpacity = 0.5f;
        Padding = { 8.0f, 8.0f, 8.0f, 8.0f };
        Stretch = { 1.0f, 1.0f };
    }
    virtual ~Button() = default;

    void DebugPrint(size_t indent) const override {
        Log("{}{}{} [ Text: '{}' | Position: 'x={}, y={}' | Size '{}x{}' ]",
            indent == 0 ? "" : string(indent * 2, ' '),
            indent == 0 ? "" : "◌ ",
            ID,
            Text,
            Position.X, Position.Y,
            Size.Width, Size.Height
        );
    }
    void Draw() override;
    void Update() override {
        if (Font) {
            MinSize = Font->GetSize(Text);
            MinSize.Width += Padding.Left + Padding.Right;
            MinSize.Height += Padding.Top + Padding.Bottom;
            Position.X -= Padding.Left + Offset.X;
            Position.Y -= Padding.Top + Offset.Y;
        }
    }

public:
    string Text {};
    Font *Font {};

    std::function<void()> Click;
};

class CheckBox: public Control {
public:
    CheckBox(const string &id, string_view text, bool &value):
        Control(id, ComponentType::CheckBox),
        Text(text),
        Value(value) {
        Alignment = { 0.0f, 1.0f };
        FocusStyle = FocusStyle::Underline;
        Padding = { 4.0f, 4.0f, 4.0f, 4.0f };
        Stretch = { 1.0f, 0.0f };

        //Alignment = { 0.5f, 0.5f };
        OutherSize = { 16.0f, 16.0f };
        InnerSize = { 10.0f, 10.0f };
    }
    virtual ~CheckBox() = default;

    void DebugPrint(size_t indent) const override {
        Log("{}{}{} [ Text: '{}' | Position: 'x={}, y={}' | Size '{}x{}' ]",
            indent == 0 ? "" : string(indent * 2, ' '),
            indent == 0 ? "" : "◌ ",
            ID,
            Text,
            Position.X, Position.Y,
            Size.Width, Size.Height
        );
    }
    void Draw() override;
    void Update() override {
        if (Font) {
            MinSize = Font->GetSize(Text);
            MinSize.Width += Padding.Left + Padding.Right + 32.0f;
            MinSize.Height += Padding.Top + Padding.Bottom + 4.0f;
            Position.X -= Padding.Left * 2 + Offset.X;
            Position.Y -= Padding.Top + Offset.Y;
        }
    }

public:
    string Text {};
    bool Value { false };
    Font *Font {};
    
public:
    Ultra::Color InnerColor {};
    Ultra::Position InnerPosition {};
    Ultra::Size InnerSize {};

    Ultra::Color OutherColor {};
    Ultra::Position OutherPosition {};
    Ultra::Size OutherSize {};
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

class InputBox: public Control {
public:
    InputBox(const string &id):
        Control(id, ComponentType::Input) {
    }
    ~InputBox() = default;

    void Draw() override;
    void DebugPrint(size_t indent) const override {
        Log("{}{}{} [ Text: '{}' | Position: 'x={}, y={}' | Size '{}x{}' ]",
            indent == 0 ? "" : string(indent * 2, ' '),
            indent == 0 ? "" : "◌ ",
            ID,
            Text,
            Position.X, Position.Y,
            Size.Width, Size.Height
        );
    }

public:
    Font *Font {};
    string Text;
};

class Label: public Control {
public:
    Label(const string &id, string_view text):
        Control(id, ComponentType::Label),
        Text(text) {
        Alignment = { 0.0f, 1.0f };
        Padding = { 2.0f, 1.0f, 2.0f, 1.0f };
        Interactive = false;
        //Stretch = { 1.0f, 1.0f };
    }
    virtual ~Label() = default;

    void DebugPrint(size_t indent) const override {
        Log("{}{}{} [ Text: '{}' | Position: 'x={}, y={}' | Size '{}x{}' ]",
            indent == 0 ? "" : string(indent * 2, ' '),
            indent == 0 ? "" : "◌ ",
            ID,
            Text,
            Position.X, Position.Y,
            Size.Width, Size.Height
        );
    }
    void Draw() override;
    void Update() override {
        if (Font) {
            MinSize = Font->GetSize(Text);
            MinSize.Width += Padding.Left + Padding.Right;
            MinSize.Height += Padding.Top + Padding.Bottom;
            Position.X -= Padding.Left + Offset.X;
            Position.Y -= Padding.Top + Offset.Y;
        }
    }

public:
    string Text {};
    Font *Font {};
};

class Selection: public Control {
public:
    Selection(const string &id, string_view text, const vector<string> &options):
        Control(id, ComponentType::Selection),
        Text(text),
        Options(options) {
        Padding = { 8.0f, 8.0f, 8.0f, 8.0f };
        SelectedIndex = 0;
    }
    ~Selection() = default;

    void Draw() override;
    void Update() override {
        if (Font) {
            MinSize = Font->GetSize(Text);
            MinSize.Width += Padding.Left + Padding.Right;
            MinSize.Height += Padding.Top + Padding.Bottom;
            Position.X -= Padding.Left + Offset.X;
            Position.Y -= Padding.Top + Offset.Y;
        }
        if (Options.size() > 0) {
            auto minSize = Ultra::Size{};
            for (auto &&option : Options) {
                auto size = Font->GetSize(option);
                //minSize += size;
            }
            //minSize.Width += Padding.Left + Padding.Right;
            //minSize.Height += Padding.Top + Padding.Bottom;
        }
    }

private:
    void DrawDropdown();

public:
    Font *Font {};
    string Text {};
    vector<string> Options;
    std::function<void()> Click;

private:
    size_t SelectedIndex {};
    bool ShowDropdown = false;
};

class Seperator: public Control {
public:
    Seperator(const string &id):
        Control(id, ComponentType::Seperator) {
    }
    virtual ~Seperator() = default;

    void Draw() override;
};

class Slider: public Control {
public:
    Slider(const string &id, float value):
        Control(id, ComponentType::Slider),
        Value(value) {
        Alignment = { 0.5f, 0.5f };
        Color = { 0.5f, 0.5f, 0.5f, 1.0f };
        FocusStyle = FocusStyle::Scroll;
        MinSize = { 200.0f, 24.0f };
        Stretch = { 1.0f, 0.0f };
    }
    virtual ~Slider() = default;

    void DebugPrint(size_t indent) const override {
        Log("{}{}{} [ Text: '{}' | Position: 'x={}, y={}' | Size '{}x{}' ]",
            indent == 0 ? "" : string(indent * 2, ' '),
            indent == 0 ? "" : "◌ ",
            ID,
            Value,
            Position.X, Position.Y,
            Size.Width, Size.Height
        );
    }
    void Draw() override;
    void Update() override {

    }

public:
    float Value {};
    float MinValue {};
    float MaxValue {};
    float HandleWidth { 6.0f };
    float DragOffset {};
};


// Todo

class Cursor: public Control {
public:
    Cursor(const string &id):
        Control(id, ComponentType::Cursor) {
    }
    virtual ~Cursor() = default;

    Texture *Data {};
    bool Outline {};
};

class ColorPicker: public Control {
public:
    ColorPicker(const string &id):
        Control(id, ComponentType::ColorPicker) {
    }
    ~ColorPicker() = default;

    void Draw() override;
};

class Table: public Control {
public:
    Table(const string &id): Control(id, ComponentType::Table) {
        ColumnWidths = { 100.0f, 150.0f, 200.0f };
        RowHeight = 30.0f;
        Header = { "Column 1", "Column 2", "Column 3" };
        Data = {
            { "Row 1 Data 1", "Row 1 Data 2", "Row 1 Data 3" },
            { "Row 2 Data 1", "Row 2 Data 2", "Row 2 Data 3" },
        };
    }
    ~Table() = default;

    void Draw() override {
        DrawHeader();
        DrawRows();
    }

private:
    void DrawHeader();
    void DrawRows();

private:
    std::vector<float> ColumnWidths;
    float RowHeight;
    std::vector<string> Header;
    std::vector<std::vector<string>> Data;
};

class Tree: public Control {
public:
    Tree(const string &id):
        Control(id, ComponentType::Tree) {
    }
    ~Tree() = default;

    void Draw() override;

private:
    void DrawTreeNode(const Ultra::Position &position, const string_view &text);

};

///
/// @brief Containers
///

class Window;

class Container: public Component {
public:
    Container(const string &id, const ComponentType &type = ComponentType::Container):
        Component(id, type) {
    }
    virtual ~Container() = default;

    // Accessors
    Component *GetChild(const string &id)  {
        if (*this == id) return this;
        for (const auto &child : mChildren) {
            if (child->Type < ComponentType::Containers) {
                if (auto *const result = child->As<Container>()->GetChild(id)) {
                    return result;
                }
            } else {
                if (*child == id) {
                    return child.get();
                }
            }
        }
        return nullptr;
    }
    const vector<Scope<Component>> &GetChildren()  {
        return mChildren;
    }

    // Mutators
    void AddChild(Scope<Component> child) {
        child->Parent = this;
        mChildren.push_back(std::move(child));
    }

    // Factory Methods
    
    //Window *CreateWindow(string_view title, const string &uniqueId = "") {
    //    string id = uniqueId.empty() ? std::format("Window#{}", sWindowCounter++) : uniqueId;
    //    auto *component = GetChild(id);
    //    if (component) return component->As<Window>();
    //}
    Container *CreateContainer(const Ultra::UI::Layout &layout = Layout::None, const string &uniqueId = "", const ComponentType &type = ComponentType::Container)  {
        string id = uniqueId.empty() ? std::format("Container#{}", sContainerCounter++) : uniqueId;

        auto *component = GetChild(id);
        if (component) return component->As<Container>();

        auto container = CreateScope<Container>(id, type);
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
    Container *CreateScrollView(float maxHeight) {
        string id = std::format("ScrollView#{}", sScrollViewCounter);

        auto *component = GetChild(id);
        if (component) {
            sScrollViewCounter++;
            return component->As<Container>();
        }

        auto scrollview = CreateContainer(Layout::Vertical, id, ComponentType::ScrollView);
        scrollview->Clip = true;
        scrollview->Expand = false;
        scrollview->Padding = { 6.0f, 6.0f, 6.0f, 6.0f };
        scrollview->Stretch = { 1.0f, 1.0f };
        scrollview->Spacing = 2.0f;
        scrollview->MaxSize.Height = maxHeight;

        // ToDo: ScrollBar currently missing...

        sScrollViewCounter++;
        return scrollview;
    }
    Button *CreateButton(string_view text)  {
        string id = std::format("Button#{}", sButtonCounter++);
        auto *component = GetChild(id)->As<Button>();
        if (component) return component;

        auto button = Control::Create<Button>(id, string(text));
        button->Parent = this;
        button->Color = this->Style.ColorText;
        button->Font = this->Style.Font.get();
        button->Update();

        auto result = button.get();
        AddChild(std::move(button));
        return result;
    }
    CheckBox *CreateCheckBox(string_view text, bool value = false)  {
        string id = std::format("CheckBox#{}", sCheckBoxCounter++);
        auto *component = GetChild(id);
        if (component) return component->As<CheckBox>();

        auto checkbox = Control::Create<CheckBox>(id, text, value);
        checkbox->Parent = this;
        checkbox->Color = this->Style.ColorText;
        checkbox->Font = this->Style.Font.get();
        checkbox->OutherColor = this->Style.ColorFrame;
        checkbox->InnerColor = this->Style.ColorPrimary;
        checkbox->Update();

        auto result = checkbox.get();
        AddChild(std::move(checkbox));
        return result;
    }
    Image *CreateImage(string_view path) {
        string id = std::format("Image#{}", sImageCounter++);

        auto *component = GetChild(id);
        if (component) return component->As<Image>();

        auto image = Control::Create<Image>(id);
        image->Stretch = { 1.0f, 1.0f };
        image->Data = Texture::Create({}, string(path));

        auto result = image.get();
        AddChild(std::move(image));
        Parent = this;
        return result;
    }
    InputBox *CreateInputBox(string_view text) {
        string id = std::format("Input#{}", sInputBoxCounter++);

        auto *component = GetChild(id)->As<InputBox>();
        if (component) return component;

        auto input = Control::Create<InputBox>(id);
        input->Parent = this;
        input->Color = this->Style.ColorText;
        input->Font = this->Style.Font.get();
        input->Text = text;
        if (input->Font) {
            input->MinSize = input->Font->GetSize(text);
            input->MinSize.Width += input->Padding.Left + input->Padding.Right;
            input->MinSize.Height += input->Padding.Top + input->Padding.Bottom;
            input->Position.X -= input->Padding.Left + input->Offset.X;
            input->Position.Y -= input->Padding.Top + input->Offset.Y;
        }

        auto result = input.get();
        AddChild(std::move(input));
        return result;
    }
    Label *CreateLabel(string_view text, Font *font = nullptr, const string &uniqueId = "")  {
        string id = uniqueId.empty() ? std::format("Label#{}", sLabelCounter++) : uniqueId;

        auto *component = GetChild(id);
        if (component) return component->As<Label>();

        auto label = Control::Create<Label>(id, text);
        label->Color = this->Style.ColorText;
        if (font) {
            label->Font = font;
        } else {
            label->Font = this->Style.Font.get();
        }
        label->Update();

        auto result = label.get();
        AddChild(std::move(label));
        Parent = this;
        return result;
    }
    Selection *CreateSelection(string_view text, const vector<string> &options) {
        string id = std::format("Selection#{}", sSelectionCounter++);

        auto *component = GetChild(id)->As<Selection>();
        if (component) return component;

        auto selection = Control::Create<Selection>(id, text, options);
        selection->Parent = this;
        selection->Color = this->Style.ColorText;
        selection->Font = this->Style.Font.get();
        selection->Update();

        auto result = selection.get();
        AddChild(std::move(selection));
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
    Slider *CreateSlider(float value, float min = 0.0f, float max = 128.0f)  {
        string id = std::format("Slider#{}", sSliderCounter++);

        auto *component = GetChild(id);
        if (component) return component->As<Slider>();

        auto slider = Control::Create<Slider>(id, value);
        slider->MinValue = min;
        slider->MaxValue = max;

        auto result = slider.get();
        AddChild(std::move(slider));
        Parent = this;
        return result;
    }

    // Methods

    // Check if a container and its child at position is hovered
    bool Focused(const Ultra::Position position) {
        Reset();
        for (auto &child : mChildren | std::views::reverse) {
            if (child->Hovered(position)) {
                if (child->Type < ComponentType::Containers) {
                    if (child->As<Container>()->Focused(position)) return true;
                }
                if (!child->Interactive) return false;
                child->Focused = true;
                Log("{}", child->ID);
                return true;
            }
        }
        return false;
    }

    // Compute the size of the container based on its children
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
                    if (child != mChildren.front()) MinSize.Height += Spacing;
                    break;
                }
                // For horizontal layout, add up children's widths and choose maximum height
                case Layout::Horizontal: {
                    MinSize.Width += child->MinSize.Width;
                    MinSize.Height = std::max(MinSize.Height, child->MinSize.Height);
                    if (child != mChildren.front()) MinSize.Width += Spacing;
                    break;
                }
            }
        }

        // Add padding to the minimum size
        MinSize.Width += Padding.Left + Padding.Right;
        MinSize.Height += Padding.Top + Padding.Bottom;

        // Clamp the calculated minimum size to the specified maximum size
        OriginalSize = MinSize;
        MinSize.Width = std::min(MinSize.Width, MaxSize.Width);
        MinSize.Height = std::min(MinSize.Height, MaxSize.Height);
    }

    // Compute the position of the container and update its children
    void ComputePosition() {
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
                    child->CalculateLayout(child->Position, size);
                    break;

                }
                case Layout::Stack: {
                    child->CalculateLayout(pos, size);
                    break;
                }

                // Calculate height based on child's minimum size and stretch factor, than update childs position and size, move the position for the next child vertically
                case Layout::Vertical: {
                    s = child->MinSize.Height;
                    if (extra > 0) s += child->Stretch.Y * extra;
                    child->CalculateLayout(pos, { size.Width, s });
                    pos.Y += child->Size.Height + Spacing;
                    break;
                }

                // Calculate height based on child's minimum size and stretch factor, than update childs position and size, move the position for the next child horizontally
                case Layout::Horizontal: {
                    s = child->MinSize.Width;
                    if (extra > 0) s += child->Stretch.X * extra;
                    child->CalculateLayout(pos, { s, size.Height });
                    pos.X += child->Size.Width + Spacing;
                    break;
                }
            }

            // Recursively layout child containers or windows
            if (child->Type < ComponentType::Containers) child->As<Container>()->ComputePosition();
        }
    }

    // Draw the container and its children
    void Draw() override;

    void DebugPrint(size_t level = 0) const {
        Log("{}{}{} [ childs: {} | Position: 'x={}, y={}' | Size '{}x{}' ]",
            level == 0 ? "" : string(level * 2 - 2, ' '),
            level == 0 ? "" : "◌ ", ID,
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
    void MoveToTop(const string &id) {
        auto it = std::find_if(mChildren.begin(), mChildren.end(), [&id](const auto &child) {
            return child->ID == id;
        });
        if (it != mChildren.end()) {
            std::rotate(it, mChildren.end() - 1, mChildren.end());
        }
    }
    static void Update() {
        sContainerCounter = 0;
        sButtonCounter = 0;
        sCheckBoxCounter = 0;
        sImageCounter = 0;
        sInputBoxCounter = 0;
        sLabelCounter = 0;
        sScrollViewCounter = 0;
        sSelectionCounter = 0;
        sSeparatorCounter = 0;
        sSliderCounter = 0;
    }


private: // Methods
    void Reset() {
        for (auto &child : mChildren) {
            child->Focused = false;
            if (child->Type < ComponentType::Containers) {
                child->As<Container>()->Reset();
            }
        }
    }
    void UpdateClipRect() {
        //ClipRect.Right = Position.X;
        //ClipRect.Bottom = Position.Y;

        //?rect->Upper = { g->Position, g->Size };

        //ClipRect->Left = std::min(rect->Upper.X, rect->Previous->Upper.X);
        //ClipRect->Top = std::min(rect->Upper.Y, rect->Previous->Upper.Y);
        //ClipRect->Right = std::max(rect->Lower.Y, rect->Previous->Lower.Y);
        //ClipRect->Bottom = std::max(rect->Lower.X, rect->Previous->Lower.X);
    }

public: // Properties
    bool Clip {};
    bool Expand { true };
    Layout Layout {};
    Ultra::Size MaxSize { 1e30f, 1e30f };
    float Spacing {};
    Style Style {};
    float ZIndex {};

private: // Properties
    vector<Scope<Component>> mChildren;

private: // States
    inline static size_t sContainerCounter {};
    inline static size_t sWindowCounter {};

    inline static size_t sButtonCounter {};
    inline static size_t sCheckBoxCounter {};
    inline static size_t sImageCounter {};
    inline static size_t sInputBoxCounter {};
    inline static size_t sLabelCounter {};
    inline static size_t sScrollViewCounter {};
    inline static size_t sSelectionCounter {};
    inline static size_t sSeparatorCounter {};
    inline static size_t sSliderCounter {};
};

class Window: public Container {
public:
    Window(const string &id, string_view title = ""):
        Container(id, ComponentType::Window),
        Title(title) {
    }
    virtual ~Window() = default;

public:
    string Title {};
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
    void CalculateLayout() {
        mRoot->ComputeSize();
        mRoot->ComputePosition();
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
private:
    // Friends
    friend Container;
    friend Window;
    friend Button;
    friend CheckBox;
    friend ColorPicker;
    friend InputBox;
    friend Label;
    friend Selection;
    friend Slider;
    friend Seperator;
    friend Tree;

private:
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

    // Accessors
    static Scope<UILayoutManager> &GetLayoutManager() { return Instance().mUILayout; }
    static Container *GetRoot() { return Instance().mUILayout->GetRoot(); }
    static Style &GetStyle() { return Instance().mUILayout->GetRoot()->Style; }

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
        auto window = CreateScope<Window>(id, title);
        window->Layout = Layout::Vertical;
        window->Expand = true;
        window->FrameOpacity = 0.97f;
        window->Padding = { 8.0f, 0.0f, 8.0f, 8.0f };
        window->Style.Font = manager->GetRoot()->Style.Font;

        // TitleBar
        auto titleBarId = std::format("TitleBar#{}", sWindowCounter);
        auto titleBar = window->CreateContainer(Layout::Stack, titleBarId);
        titleBar->Clip = true;
        titleBar->Interactive = false;
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
        UIRenderer::Begin(Instance().mViewport);
        GetRoot()->Draw();
        UIRenderer::End();
        UIRenderer::Draw();
    }
    static void Update(Timestamp deltaTime) {
        // Calculate Frames
        mFrames++;
        mDeltaDelay += deltaTime;

        // Update Inputs
        if (mDeltaDelay > 0.016333f) {
            auto [mouseX, mouseY] = Input::GetMousePosition();
            sInputState.LastMousePosition = { mouseX, mouseY };

            sInputState.MouseClicked = Input::GetMouseButtonStateDelta(MouseButton::Left);
            sInputState.MousePressed = Input::GetMouseButtonState(MouseButton::Left);

            // Start Dragging
            if (sInputState.MouseClicked && sInputState.MousePressed) {
                sInputState.Dragging = true;
                sInputState.DragStartPosition.X = mouseX;
                sInputState.DragStartPosition.Y = mouseY;
            }
            
            // Update Dragging
            if (sInputState.MousePressed) {
                sInputState.DragCurrentPosition.X = mouseX - sInputState.DragStartPosition.X;
                sInputState.DragCurrentPosition.Y = mouseY - sInputState.DragStartPosition.Y;
            } else {
                sInputState.Dragging = false;
                sInputState.DragStartPosition = {};
                sInputState.DragCurrentPosition = {};
            }
        }

        // Update Information
        if (mDeltaDelay > 1.0f) {
            mFPS = mFrames;
            mMSPF = 1000.0 / mFrames;
            mFrames = 0;
            mDeltaDelay = 0;
        }

        // Update Initial Layout
        if (static bool once = true) {
            GetLayoutManager()->CalculateLayout();
            GetLayoutManager()->GetRoot()->DebugPrint();
            once = false;
        }

        // Check if component is active (hovered and on top)
        if (!sInputState.Dragging) {
            GetRoot()->Focused(sInputState.LastMousePosition);
        }

        sWindowCounter = 0;
        Container::Update();
    }

    // Shows some demo windows and is also used for tests
    static void ShowDemo() {
        #pragma warning(disable: 4189)
        ///
        /// @brief Here we test new API styles, until we find the best one...
        ///

        // Background
        //auto image = HmGui::GetRoot()->CreateImage("Assets/Textures/Wallpaper.jpg");
        auto image = HmGui::GetRoot()->CreateImage("Data/Cache/Fonts/Rajdhani.bmp");
        image->Interactive = false;

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
                    eventButton->Click = [&]() {
                        Log("Button '-- OPT 1 --' clicked!");
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

        // Under Construction Window
        {
            auto window = HmGui::CreateWindow("Under Construction");
            window->Alignment = { 0.0f, 1.0f };

            auto container = window->CreateContainer(Layout::Vertical);
            container->Alignment = { 0.5f, 0.5f };

            auto input = container->CreateInputBox("Type in your text...");
            input->MinSize = { 200.0f, 64.0f };

            auto slider = container->CreateSlider(0.5f, 0.0f, 1.0f);
            slider->MinSize = { 200.0f, 20.0f };

            //auto separator = container->CreateSeparator();
            //separator->Size = { 300.0f, 2.0f };
            
            auto selection = container->CreateSelection("Select a Option", {"A", "B", "C"});
            selection->Click = [&](const string &option = "") {
                Log("Selected option: {}", option);
            };
        }

        #pragma warning(default: 4189)
    }

private:
    // Counter for ComponentIDs
    inline static size_t sWindowCounter {};

    // Instances
    Scope<UILayoutManager> mUILayout {};
    Scope<Viewport> mViewport;

    // States
    struct InputState {
        bool Dragging {};
        bool MouseClicked {};
        bool MousePressed {};

        Position DragStartPosition {};
        Position DragCurrentPosition {};
        Position LastMousePosition {};
    } inline static sInputState {};

    // Test
    Scope<Font> mFontExo2Bold = nullptr;
    Scope<Font> mFontFiraMono = nullptr;
    Scope<Font> mFontRajdhani = nullptr;
    inline static thread_local double mFrames {};
    inline static thread_local double mFPS {};
    inline static thread_local double mMSPF {};
    inline static thread_local double mDeltaDelay {};
};

}

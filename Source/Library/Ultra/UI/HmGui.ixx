export module Ultra.UI.HmGui;

export import Ultra.Engine.Font;

import Ultra.Core;
import Ultra.Engine.Renderer.Texture;
import Ultra.Engine.Renderer.Viewport;
import Ultra.Engine.UIRenderer;
import Ultra.Math;
import Ultra.System.Input;

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
    Size,
};

enum class Layout {
    None,
    Stack,
    Vertical,
    Horizontal,
};

}

///
/// @brief ToDo: Remove this legacy code
///
namespace Ultra::UI {

///
/// @brief Enumerations and Types
///
enum class ControlType {
    None,
    Panel,
    Image,
    Line,
    Rect,
    Text,
    Window,
};

///
/// @brief Containers
///

struct HmGuiClipRect {
    HmGuiClipRect *Previous {};
    Position Lower {};
    Position Upper {};
};

struct HmGuiWidget {
    struct HmGuiGroup *Parent {};
    HmGuiWidget *Next {};
    HmGuiWidget *Previous {};

    uint64_t Hash {};
    ControlType Type {};

    Ultra::Position Position {};
    Ultra::Size Size {};
    Ultra::Size MinSize {};
    Ultra::Alignment Align {};
    Ultra::Stretch Stretch {};
};

struct HmGuiGroup: public HmGuiWidget {
    HmGuiWidget *Head {};
    HmGuiWidget *Tail {};

    Layout Layout {};

    uint32_t Children {};
    FocusStyle FocusStyle {};

    Ultra::Padding Padding {};
    Ultra::Position Offset {};
    Ultra::Size MaxSize {};
    Ultra::Stretch TotalStretch {};

    float Spacing {};
    float FrameOpacity {};
    bool Clip {};
    bool Expand {};
    bool Focusable[(int)FocusType::Size] {};
    bool StoreSize {};
};

///
/// @brief Controls
///

struct HmGuiCursor: public HmGuiWidget {};

struct HmGuiImage: public HmGuiWidget {
    Reference<Texture> Image {};
};

struct HmGuiLine: public HmGuiWidget {
    Ultra::Color Color;
    Ultra::Position Start;
    Ultra::Position Stop;
};

struct HmGuiRect: public HmGuiWidget {
    Color Color {};
    bool Outline;
};

struct HmGuiText: public HmGuiWidget {
    string Text {};
    Color Color {};
    Font *Font {};
};

///
/// @brief Data
///

struct HmGuiData {
    Ultra::Position Offset {};
    Ultra::Size MinSize {};
    Ultra::Size Size {};
};

///
/// @brief Style
///

struct HmGuiStyle {
    HmGuiStyle *Previous {};

    Font *Font {};

    float Spacing {};

    Color ColorPrimary {};
    Color ColorFrame {};
    Color ColorText {};
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

///
/// @brief Base Types and Component Interface
///

enum class ComponentType {
    // Containers
    None,
    Container,
    Panel,
    ScrollView,
    Window,

    // Controls
    Button,
    Cursor,
    CheckBox,
    ColorPicker,
    Grid,
    Image,
    Input,
    Label,
    Selection,
    Seperator,
    Slider,
    Table,

    // Information
    Badge,
    Notification,
    Tooltip,

    // Navigation (multiple-controls)
    MenuBar,
    SideBar,
    TabBar,
};

struct ClipArea {
    Position TopLeft;
    Position BottomRight;
};

struct Style {
    Color ColorPrimary {};
    Color ColorSecondary {};
    Color ColorFrame {};
    Color ColorText {};

    Font *Font {};

    float Spacing {};
};

class Component {
public:
    Component(const string &id, const ComponentType &type):
        mID(id),
        mHash(mHasher(id)),
        Type(type) {
    }
    virtual ~Component() = default;

    // Converters
    template<typename T>
    T *As() {
        return dynamic_cast<T *>(this);
    }

    // Accessors
    const string &GetID() {
        return mID;
    }

protected:
    string mID;
    size_t mHash;

public:
    Component *Parent {};
    ComponentType Type {};

    Color Color {};
    Position Position {};
    Size Size {};

    // Transformation
    Ultra::Padding Padding {};
    Ultra::Position Offset {};
    Ultra::Size MinSize {};
    Ultra::Stretch Stretch {};

    // States
    bool Active {};
    bool Clip {};
    ClipArea ClipArea {};
    float FrameOpacity {};
    bool Visible { true };
    float ZIndex {};

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
    virtual void DebugPrint(int indent) const {
        LogInfo("{}{}{}", indent == 0 ? "" : string(indent * 2, ' '), indent == 0 ? "" : "◌ ", mID);
    }

public:
    array<bool, (int)FocusType::Size> Focused {};
    array<bool, (int)FocusType::Size> Focusable {};
    FocusStyle FocusStyle {};
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

    // Methods
    void DebugPrint(int indent) const override {
        LogInfo("{}{}{} [ Text: '{}' ]", indent == 0 ? "" : string(indent * 2, ' '), indent == 0 ? "" : "◌ ", mID, Text);
    }

public:
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
    void DebugPrint(int indent) const override {
        LogInfo("{}{}{} [ Text: '{}' ]", indent == 0 ? "" : string(indent * 2, ' '), indent == 0 ? "" : "◌ ", mID, Text);
    }

public:
    Font *Font {};
    bool Outline {};
    string Text {};
    
    Ultra::Position CheckInner {};
    Ultra::Position CheckOuther {};
};

class Label: public Control {
public:
    Label(const string &id):
        Control(id, ComponentType::Label) {
    }
    virtual ~Label() = default;

    // Methods
    void DebugPrint(int indent) const override {
        LogInfo("{}{}{} [ Text: '{}' ]", indent == 0 ? "" : string(indent * 2, ' '), indent == 0 ? "" : "◌ ", mID, Text);
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
};

struct Image: public Control {
public:
    Image(const string &id):
        Control(id, ComponentType::Image) {
    }
    virtual ~Image() = default;

public:
    Texture *Data {};
};

class ScrollView: public Control {
public:
    ScrollView(const string &id):
        Control(id, ComponentType::ScrollView) {
    }
    virtual ~ScrollView() = default;

    // Methods
    void DebugPrint(int indent) const override {
        LogInfo("{}{}{} [ MaxHeight: {} ]", indent == 0 ? "" : string(indent * 2, ' '), indent == 0 ? "" : "◌ ", mID, MaxHeight);
    }

public:
    float MaxHeight;
};

class Slider: public Control {
public:
    Slider(const string &id):
        Control(id, ComponentType::Slider) {
    }
    virtual ~Slider() = default;
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
    Container *CreateContainer(const Layout &layout = Layout::None, const string &id = "")  {
        string identifier = id.empty() ? std::format("Container#{}", sContainerCounter++) : std::format("{}#{}", id, sContainerCounter++);

        auto *component = GetChild(identifier);
        if (component) return component->As<Container>();

        auto container = CreateScope<Container>(identifier);
        // ToDo: Default Properties
        container->Expand = true;
        container->MaxSize = { 1e30f, 1e30f };
        container->Spacing = this->Style.Spacing;
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
        return result;
    }
    Button *CreateButton(string_view text)  {
        string id = std::format("Button#{}", sButtonCounter++);

        auto *component = GetChild(id);
        if (component) return component->As<Button>();

        auto button = CreateScope<Button>(id);
        button->Text = text;
        //button->Layout = Layout::Stack;
        //button->Alignment = { 0.5f, 0.5f };
        button->FocusStyle = FocusStyle::Fill;
        button->FrameOpacity = 0.5f;
        button->Padding = { 8.0f, 8.0f, 8.0f, 8.0f };
        //bool focus = HasFocus(FocusType::Mouse) && Active;

        auto result = button.get();
        AddChild(std::move(button));
        return result;
    }
    CheckBox *CreateCheckBox(string_view text, bool value)  {
        string id = std::format("CheckBox#{}", sCheckBoxCounter++);

        auto *component = GetChild(id);
        if (component) return component->As<CheckBox>();

        auto checkbox = CreateScope<CheckBox>(id);
        checkbox->Text = text;
        //checkbox->Layout = Layout::Horizontal;
        checkbox->FocusStyle = FocusStyle::Underline;
        //if (HasFocus(FocusType::Mouse) && Active) value = !value;
        //checkbox->Alignment = { 0.0f, 0.5f };
        checkbox->Padding = { 4.0f, 4.0f, 4.0f, 4.0f };
        //checkbox->Spacing = 8.0f;
        checkbox->Stretch = { 1.0f, 0.0f };
        // ToDo: Default Properties
        //auto canvas = checkbox->CreateContainer(Layout::Stack);
        //
        //auto rect = canvas->CreateControl(ControlType::Rect)->As<UIRectOld>();
        //rect->Type = ControlType::Rect;
        //rect->Color = this->Style.ColorFrame;
        //rect->Size = { 16.0f, 16.0f };
        //if (value) {
        //    auto rect = canvas->CreateControl(ControlType::Rect)->As<UIRectOld>();
        //    rect->Type = ControlType::Rect;
        //    rect->Alignment = { 0.5f, 0.5f };
        //    rect->Color = this->Style.ColorPrimary;
        //    rect->Size = { 10.0f, 10.0f };
        //}

        auto result = checkbox.get();
        AddChild(std::move(checkbox));
        return result;
    }
    Image *CreateImage(string_view path)  {
        string id = std::format("Image#{}", sImageCounter++);

        auto *component = GetChild(id);
        if (component) return component->As<Image>();

        auto image = CreateScope<Image>(id);
        // ToDo: Default Properties
        //auto control = CreateScope<UIImageOld>(id);
        //control->Type = ControlType::Image;
        //control->Stretch = { 1.0f, 1.0f };
        ////control->Texture = texture;

        auto result = image.get();
        AddChild(std::move(image));
        return result;
    }
    Label *CreateLabel(string_view text)  {
        string id = std::format("Label#{}", sLabelCounter++);

        auto *component = GetChild(id);
        if (component) return component->As<Label>();

        auto label = CreateScope<Label>(id);
        label->Text = text;
        //label->Alignment = { 0.0f, 1.0f };
        //label->Color = this->Style.Color;
        label->Font = this->Style.Font;
        //auto size = label->Font->GetSize(text);
        //label->MinSize = { (float)size.Width, (float)size.Height };

        auto result = label.get();
        AddChild(std::move(label));
        return result;
    }
    Container *CreateScrollView(float maxHeight)  {
        string id = std::format("ScrollView#{}", sScrollViewCounter++);

        auto *component = GetChild(id);
        if (component) return component->As<Container>();

        auto scrollview = CreateScope<Container>(id);
        // ToDo: Default Properties
        //auto container = CreateScope<UIContainerOld>(Layout::Stack, id);
        //container->Type = ControlType::Panel;
        //
        //auto outer = container->CreateContainer(Layout::Horizontal);
        //outer->Type = ControlType::Panel;
        //outer->Clip = true;
        //outer->Stretch = { 1.0f, 1.0f };
        //outer->Spacing = 2.0f;
        //
        //auto inner = outer->CreateContainer(Layout::Vertical);
        //outer->Type = ControlType::Panel;
        //outer->Expand = false;
        //outer->StoreSize = true;
        //outer->MaxSize.Height = maxHeight;

        auto result = scrollview.get();
        AddChild(std::move(scrollview));
        return result;
    }
    Slider *CreateSlider(float value, float min = 0.0f, float max = 128.0f)  {
        string id = std::format("Slider#{}", sSliderCounter++);

        auto *component = GetChild(id);
        if (component) return component->As<Slider>();

        auto slider = CreateScope<Slider>(id);
        // ToDo: Default Properties
        //auto container = CreateScope<UIContainerOld>(Layout::Stack, id);
        //container->Type = ControlType::Panel;
        //container->Alignment = { 0.5f, 0.5f };
        //container->Stretch = { 1.0f, 0.0f };
        //
        //auto rect = container->CreateControl(ControlType::Rect)->As<UIRectOld>();
        //rect->Type = ControlType::Rect;
        //rect->Color = { 0.5f, 0.5f, 0.5f, 1.0f };
        //rect->Size = { 0.0f, 2.0f };
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
        return result;
    }

    // Methods
    void AddChild(Scope<Component> child) {
        child->Parent = this;
        mChildren.push_back(std::move(child));
    }
    void ComputeSize() {
        //for (const auto &child : mChildren) {
        //    if (child->Type == ControlType::Window || child->Type == ControlType::Panel) {
        //        //child->ComputeSize();
        //    }
        //}
        //MinSize = { 0, 0 };

        //for (const auto &child : mChildren) {
        //    switch (Layout) {
        //        case Layout::Stack:
        //            //MinSize.Width = std::max(MinSize.Width, e->MinSize.Width);
        //            //MinSize.Height = std::max(MinSize.Height, e->MinSize.Height);
        //            break;
        //        case Layout::Vertical:
        //            //MinSize.Width = std::max(MinSize.Width, e->MinSize.Width);
        //            //MinSize.Height += e->MinSize.Height;
        //            //if (e != Head) MinSize.Height += Spacing;
        //            break;
        //        case Layout::Horizontal:
        //            //MinSize.Width += e->MinSize.Width;
        //            //MinSize.Height = std::max(MinSize.Height, e->MinSize.Height);
        //            //if (e != Head) MinSize.Width += Spacing;
        //            break;
        //    }
        //}

        //MinSize.Width += Padding.Left + Padding.Right;
        //MinSize.Height += Padding.Top + Padding.Bottom;

        //if (StoreSize) {
        //    //HmGuiData *data = GetData(g);
        //    //data->MinSize = MinSize;
        //}

        //MinSize.Width = std::min(MinSize.Width, MaxSize.Width);
        //MinSize.Height = std::min(MinSize.Height, MaxSize.Height);
    }
    void DebugPrint(size_t level = 0) const {
        LogInfo("{}{}{} [ childs: {} ]", level == 0 ? "" : string(level * 2 - 2, ' '), level == 0 ? "" : "◌ ", mID, mChildren.size());
        for (const auto &child : mChildren) {
            if (child->Type == ComponentType::Container) {
                child->As<Container>()->DebugPrint(level + 1);
            } else {
                child->As<Control>()->DebugPrint(level);
            }
        }
    }
    void MoveToTop(const string &id) {
        //auto it = std::find_if(mChildren.begin(), mChildren.end(), [&id](const auto &child) {
        //    return child->GetID() == id;
        //});
        //if (it != mChildren.end()) {
        //    std::rotate(it, it + 1, mChildren.end());
        //}
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
            if (child->Type == ComponentType::Container) {
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

    // States
    bool HasFocus(const FocusType &type) {
        //Focusable[(int)type] = true;
        //return Focused[(int)type] == mHash;
    }
    void OnClick(const string &id) {
        //UIElementOld *element = GetElement(id);
        //if (element && element->GetParent()) {
        //    MoveToTop(element->GetParent()->GetID());
        //}
    }

protected:
    Container *mParent = nullptr;
    vector<Scope<Component>> mChildren;

public:
    Layout Layout {};
    Style Style {};

    bool Expand { true };
    float Spacing {};

    Alignment Aligment {};
    Ultra::Size MaxSize { 1e30f, 1e30f };
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
/// @brief This class manages the UI layout.
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
    void DebugPrint(int level = 0) {
        mRoot->DebugPrint();
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
/// @brief Switches
///

#define MODERN_HMGUI_IMPLEMENTATION
constexpr bool DrawGroupFrames = false;
constexpr bool EnableWindowDragging = true;

///
/// @brief Hybrid-Mode-UI, a combination of immediate mode and retained mode.
/// @note This implementation is inspired by Josh Parnell
/// @link http://forums.ltheory.com/viewtopic.php?t=6582
///
class HmGui {
public:
    HmGui() {
        // From Begin init
        mUILayout = CreateScope<UILayoutManager>();
        auto *root = mUILayout->GetRoot();

        root->Style.Font = new Font("Rajdhani", 14);
        root->Style.Spacing = 6;
        root->Style.ColorPrimary = { 0.1f, 0.5f, 1.0f, 1.0f };
        root->Style.ColorSecondary = { 0.1f, 1.0f, 0.5f, 1.0f };
        root->Style.ColorFrame = { 0.1f, 0.1f, 0.1f, 0.5f };
        root->Style.ColorText = { 1.0f, 1.0f, 1.0f, 1.0f };

        // From Begin
        root->Active = Input::GetMouseButtonState(MouseButton::Left);
        root->Clip = true;
        root->Position = { 0.0f, 0.0f };
        root->Size = { 1280.0f, 1024.0f };
    };
    ~HmGui() = default;
    static HmGui &Instance() {
        static HmGui instance;
        return instance;
    }

    /// @brief Creates a window or returns an existing window
    /// @param title Specify the window title.
    /// @return Returns a non owning pointer to the object.
    static Container *CreateWindow(string_view title) {
        string id = std::format("Window#{}", sWindowCounter++);

        auto &manager = Instance().mUILayout;
        auto *component = manager->GetComponent(id);
        if (component) return component->As<Container>();

        auto window = CreateScope<Container>(id);

        // BeginWindow
        //window->Layout = Layout::Stack;
        //window->Type = ControlType::Window;
        //window->FrameOpacity = 0.97f;

        // Title
        //auto titleBar = window->CreateContainer();
        //titleBar->Alignment = { 0.5f, 0.0f };
        //titleBar->Clip = true;
        //titleBar->Padding = { 8.0f, 8.0f, 8.0f, 8.0f };
        //titleBar->Stretch = { 1.0f, 1.0f };

        //auto titleLabel = titleBar->CreateLabel(title);
        //titleLabel->Color = { 1.0f, 1.0f, 1.0f, 0.3f };

        if constexpr (EnableWindowDragging) {
            //if (GroupHasFocus(FocusType::Mouse)) {
            //    if (Input::GetMouseButtonState(MouseButton::Left)) {
            //        auto [x, y] = Input::GetMousePositionDelta();
            //        data->Offset.X += x;
            //        data->Offset.Y += y;
            //    }
            //}

            //container->Position.X += data->Offset.X;
            //container->Position.Y += data->Offset.Y;
        }

        // ~BeginWindow

        auto *result = window.get();
        manager->AddContainer(std::move(window));
        return result;
    }

    // Calculates the layout and draws the complete UI
    //UIRenderer::Begin(viewport);
    //UIRenderer::End();
    //UIRenderer::Draw();
    static void DrawUI(Container *current = nullptr) {
        static bool once = true;
        if (once) {
            Instance().mUILayout->DebugPrint();
        }

        auto &manager = Instance().mUILayout;
        if (!current) current = manager->GetRoot();
        
        UIRenderer::BeginLayer(current->Position, current->Size, current->Clip);

        // Draw Elements
        for (auto &element : current->GetChildren()) {
            switch (element->Type) {
                case ComponentType::Window: [[falltrough]]
                case ComponentType::Panel: [[falltrough]]
                case ComponentType::Container: {
                    const auto color = Color(0.1f, 0.12f, 0.13f, 1.0f);
                    UIRenderer::Panel(current->Position, current->Size, color, 8.0f, current->FrameOpacity);
                    DrawUI(element->As<Container>());
                    break;
                }
                
                case ComponentType::Button: {
                    auto *native = element->As<Ultra::UI::Button>();

                    native->Color = { 0.1f, 0.12f, 0.13f, 1.0f };
                    native->Size = { 200.0f, 200.0f };

                    UIRenderer::Rect(native->Position, native->Size, native->Color, native->Outline);
                    break;
                }
                case ComponentType::Image: {
                    auto *native = element->As<Ultra::UI::Image>();
                    //UIRenderer::Image(native->Position, native->Size, CreateReference<Texture>(native->Data));
                    break;
                }
                case ComponentType::Label: {
                    break;
                    auto *native = element->As<Ultra::UI::Label>();

                    UIRenderer::Rect(native->Position, native->Size, native->Color, native->Outline);

                    if constexpr (DrawGroupFrames) {
                        UIRenderer::DrawColor(0.5f, 0.2f, 0.2f, 0.5f);
                        UIRenderer::DrawBorder(1.0f, native->Position.X, native->Position.Y, native->Size.Width, native->Size.Height);
                    }

                    Position position = { native->Position.X, native->Position.Y + native->MinSize.Height };
                    UIRenderer::Text(position, native->Text, native->Color, native->Font);
                    break;
                }
                case ComponentType::Seperator: {
                    break;
                    auto *native = element->As<Ultra::UI::Seperator>();
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
                    break;
                }
                default: {
                    //LogWarning("The component type was not specified!");
                }
            }
        }

        #if TODO
        // Draw Effects
        if (current->Focusable[(int)FocusType::Mouse]) {
            // ToDo: Find out what has has to do with the focused element, which should be a bool ...
            //bool focus = current->Focused[(int)FocusType::Mouse] == current->Hash;
            bool focus = false;

            switch (current->FocusStyle) {
                case FocusStyle::None: {
                    const auto color = Color(0.1f, 0.12f, 0.13f, 1.0f);
                    UIRenderer::Panel(current->Position, current->Size, color, 8.0f, current->FrameOpacity);
                    break;
                }
                case FocusStyle::Fill: {
                    if (focus) {
                        const auto color = Color(0.1f, 0.5f, 1.0f, 1.0f);
                        UIRenderer::Panel(current->Position, current->Size, color, 0.0f, 1.0f);
                    } else {
                        const auto color = Color(0.15f, 0.15f, 0.15f, 0.8f);
                        UIRenderer::Panel(current->Position, current->Size, color, 0.0f, current->FrameOpacity);
                    }
                    break;
                }
                case FocusStyle::Outline: {
                    if (focus) {
                        const auto color = Color(0.1f, 0.5f, 1.0f, 1.0f);
                        UIRenderer::Rect(current->Position, current->Size, color, true);
                    }
                    break;
                }
                case FocusStyle::Underline: {
                    auto color = Color(0.3f, 0.3f, 0.3f, focus ? 0.5f : current->FrameOpacity);
                    UIRenderer::Rect(current->Position, current->Size, color, false);
                    break;
                }
            }
        }
        if constexpr (DrawGroupFrames) {
            UIRenderer::DrawColor(0.2f, 0.2f, 0.2f, 0.5f);
            UIRenderer::DrawBorder(2.0f, current->Position.X, current->Position.Y, current->Size.Width, current->Size.Height);
        }
        #endif


        UIRenderer::EndLayer();

        sWindowCounter = 0;
        Container::Update();
        once = false;
    }

    // Shows some demo windows and is also used for tests
    static void ShowDemo() {
        /// @brief Here we test new API styles, until we find the best one...

        //
        // Simple Window Test
        //
        {
            auto window = CreateWindow("Test");
            //window->Alignment = { 0.5f, 0.5f };

            // TabBar
            {
                auto tabs = window->CreateContainer(Layout::Horizontal);
                auto tabsButtonLeft = tabs->CreateButton(" < ");
                //tabs->Stretch = { 0.0f, 1.0f };

                auto tabsButton1 = tabs->CreateButton(" Tab1 ");
                auto tabsButton2 = tabs->CreateButton(" Tab2 ");
                auto tabsButton3 = tabs->CreateButton(" Tab3 ");
                
                //tabs->Stretch = { 0.0f, 1.0f };
                
                auto tabsButtonRight = tabs->CreateButton(" > ");
            }
        
            // Buttons and CheckBoxes
            {
                auto buttons = window->CreateContainer(Layout::Horizontal);

                // Column A
                {
                    auto group = buttons->CreateContainer(Layout::Vertical);
                    //group->Alignment = { 0.5f, 0.5f };
                    //group->Padding = { 4.0f, 4.0f, 4.0f, 4.0f };

                    auto welcome = group->CreateLabel("Welcome to...");
                    //welcome->Alignment = { 0.5f, 0.5f };

                    auto caption = group->CreateLabel("~ Hybrid Mode ~");
                    //caption->Color = { 1.0f, 0.0f, 0.3f, 1.0f };
                    //caption->Font = mFontExo2Bold;

                    auto subscript = group->CreateLabel("GUI");

                    auto buttonNotSoStretchy = group->CreateButton("Not-So-Stretchy");
                    //buttonNotSoStretchy->Stretch = { 1.0f, 0.0f };
                    auto buttonStretchy = group->CreateButton("Stretchy");

                    auto subGroup = group->CreateContainer(Layout::Horizontal);
                    for (auto i = 1; i < 3; i++) {
                        auto innerGroup = subGroup->CreateContainer(Layout::Vertical);
                        //innerGroup->Stretch = { 1.0f, 1.0f };
                        for (auto j = 1; j < 3; j++) {
                            auto button = innerGroup->CreateButton(":)");
                        }
                    }
                }

                // Column B
                {
                    auto group = buttons->CreateContainer(Layout::Vertical);
                    //group->Alignment = { 0.0f, 1.0f };
                    //group->Padding = { 4.0f, 4.0f, 4.0f, 4.0f };
                    //group->Stretch = { 1.0f, 1.0f };

                    auto eventButton = group->CreateButton("-- OPT 1 --");
                    //eventButton->OnClick = [](){ logger << "Opt 1!" };

                    auto silentButton = group->CreateButton("-- OPT 2 --");

                    auto checkBoxA = group->CreateCheckBox("Yas", true);
                    auto checkBoxB = group->CreateCheckBox("Nope", false);
                    auto checkBoxC = group->CreateCheckBox("Possibly?", false);

                    auto doneButton = group->CreateButton("DONE");
                }

                // Column C
                {
                    auto group = buttons->CreateContainer(Layout::Vertical);
                    //group->Padding = { 4.0f, 4.0f, 4.0f, 4.0f };
                    //group->Stretch = { 1.0f, 0.0f };

                    for (auto i = 1; i < 9; i++) {
                        auto autoGroup = group->CreateContainer(Layout::Horizontal);
                        //autoGroup->Alignment = { 0.5f, 0.5f };

                        for (auto j = 1; j < i; j++) {
                            auto result = std::format("{}.{}", i, j);
                            auto button = autoGroup->CreateButton(result);
                        }
                    }
                }

                // Column D
                {
                    auto view = window->CreateScrollView(256.0f);
                    //view->Spacing = 8.0f;
                    for (auto &[entry, list] : gCheckList) {
                        auto caption = view->CreateLabel(entry);
                        //caption->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
                        //caption->Font = mFontRajdhani;
                        auto group = view->CreateContainer(Layout::Vertical);
                        //group->Spacing = 2.0f;
                        //group->Padding.Left = 12.0f;
                        for (auto &[key, value] : list) {
                            auto check = group->CreateCheckBox(key, value);
                        }
                    }
                }
            }


            // Editor
            {
                auto editorTitle = window->CreateLabel("Behold, the codez! \\o/");
                auto editors = window->CreateContainer(Layout::Horizontal);
                for (auto i = 0; i < 2; i++) {
                    auto scrollView = editors->CreateScrollView(200);
                    //scrollView->Color = { 0.1f, 0.5f, 1.0f, 1.0f };
                    //scrollView->Font = mFontFiraMono;
                    //auto lines = String::Split(gCodeExample, '\n');
                    //for (auto &line : lines) {
                    //auto text = scrollView->CreateLabel(line);
                }
            }
        }

        //
        // ToDo List Window
        //
        {
            auto window = CreateWindow("ToDo List");
            //window->Alignment = { 1.0f, 0.0f };

            auto view = window->CreateScrollView(256.0f);
            //view->Spacing = 8.0f;
            for (auto &[entry, list] : gCheckList) {
                auto caption = view->CreateLabel(entry);
                //caption->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
                //caption->Font = mFontRajdhani;
                auto group = view->CreateContainer();
                //group->Layout = Layout::Vertical;
                //group->Spacing = 2.0f;
                //group->Padding.Left = 12.0f;
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
            //window->Alignment = { 1.0f, 1.0f };

            auto container = window->CreateContainer(Layout::Horizontal);

            auto fps = container->CreateLabel("");
            //fps.Text = std::format("fps: {:.2f}", 1.0f / deltaTime);
        }

        auto test = true;
    }

private:
    // Counter for ComponentIDs
    inline static size_t sWindowCounter {};

    // Instances
    Scope<UILayoutManager> mUILayout {};

    ///
    /// @brief ToDo: Remove this legacy code
    ///
public:
    // Functions
    static void Begin(Size size) {
        static auto &initialize = HmGui::Instance();
        static auto &renderer = UIRenderer::Instance();

        static bool init = false;
        if (!init) {
            init = true;
            self.root = nullptr;
            self.group = nullptr;

            self.style = new HmGuiStyle();
            self.style->Previous = nullptr;
            self.style->Font = new Font("Rajdhani", 14);
            self.style->Spacing = 6;

            self.style->ColorPrimary = { 0.1f, 0.5f, 1.0f, 1.0f };
            self.style->ColorFrame = { 0.1f, 0.1f, 0.1f, 0.5f };
            self.style->ColorText = { 1.0f, 1.0f, 1.0f, 1.0f };

            self.clipRect = 0;

            for (int i = 0; i < (int)FocusType::Size; ++i) self.focus[i] = 0;
            self.activate = false;
        }

        if (self.root) {
            FreeGroup(self.root);
            self.root = nullptr;
        }

        self.last = 0;
        self.activate = Input::GetMouseButtonState(MouseButton::Left);

        BeginGroup(Layout::None);
        self.group->Clip = true;
        self.group->Position = { 0, 0 };
        self.group->Size = size;
        self.root = self.group;
    }
    static void End() {
        EndGroup();
        ComputeSize(self.root);
        LayoutGroup(self.root);

        for (int i = 0; i < (int)FocusType::Size; ++i) {
            self.focus[i] = 0;
        }
        auto [x, y] = Input::GetMousePosition();
        self.FocusPos = { x, y };
        CheckFocus(self.root);
    }
    static void Draw(const Scope<Viewport> &viewport) {
        UIRenderer::Begin(viewport);
        HmGui::DrawUI();
        DrawGroup(self.root);
        UIRenderer::End();
        UIRenderer::Draw();
    }

    // Container
    static void BeginWindow(string_view title) {
        BeginGroup(Layout::Stack);
        SetStretch(0, 0);
        self.group->FocusStyle = FocusStyle::None;
        self.group->FrameOpacity = 0.97f;

        if constexpr (EnableWindowDragging) {
            HmGuiData *data = GetData(self.group);
            if (GroupHasFocus(FocusType::Mouse)) {
                if (Input::GetMouseButtonState(MouseButton::Left)) {
                    auto [x, y] = Input::GetMousePositionDelta();
                    data->Offset.X += x;
                    data->Offset.Y += y;
                }
            }

            self.group->Position.X += data->Offset.X;
            self.group->Position.Y += data->Offset.Y;
        }

        BeginGroup(Layout::Vertical);
        self.group->Clip = true;
        SetPadding(8, 8);
        SetStretch(1, 1);
        TextColored(title, { 1.0f, 1.0f, 1.0f, 0.3f });
        SetAlign(0.5f, 0.0f);
    }
    static void BeginGroup(Layout layout) {
        HmGuiGroup *e = new HmGuiGroup();
        InitWidget(e, ControlType::Panel);
        e->Head = 0;
        e->Tail = 0;
        e->Layout = layout;
        e->Children = 0;
        e->FocusStyle = FocusStyle::None;
        e->Padding = Ultra::Padding(0.0f, 0.0f, 0.0f, 0.0f);
        e->Offset = { 0.0f, 0.0f };
        e->MaxSize = { 1e30f, 1e30f };
        e->Spacing = self.style->Spacing;
        e->FrameOpacity = 0.0f;
        e->Clip = false;
        e->Expand = true;
        for (int i = 0; i < (int)FocusType::Size; ++i) e->Focusable[i] = false;
        e->StoreSize = false;
        self.group = e;

        switch (layout) {
            case Layout::Stack:
                e->Stretch = { 1, 1 };
                break;
            case Layout::Vertical:
                e->Stretch = { 1, 0 };
                break;
            case Layout::Horizontal:
                e->Stretch = { 0, 1 };
                break;
        }
    }
    static void BeginScroll(float maxSize) {
        BeginGroup(Layout::Horizontal);
        SetStretch(1, 1);
        self.group->Clip = true;
        SetSpacing(2);
        BeginGroup(Layout::Vertical);
        SetPadding(6, 6);
        SetStretch(1, 1);
        self.group->Expand = false;
        self.group->StoreSize = true;
        self.group->MaxSize.Height = maxSize;

        HmGuiData *data = GetData(self.group);
        self.group->Offset.Y = -data->Offset.Y;
    }
    static void EndWindow() {
        EndGroup();
        EndGroup();
    }
    static void EndGroup() {
        self.last = self.group;
        self.group = self.group->Parent;
    }
    static void EndScroll() {
        HmGuiData *data = GetData(self.group);
        if (GroupHasFocus(FocusType::Scroll)) {
            auto delta = Input::GetMouseWheelDelta();
            data->Offset.Y -= 10.0f * delta;
        }

        float maxScroll = std::max(0.0f, data->MinSize.Height - data->Size.Height);
        data->Offset.Y = std::clamp(data->Offset.Y, 0.0f, maxScroll);

        EndGroup();

        BeginGroup(Layout::Vertical);
        SetStretch(0, 1);
        SetSpacing(0);
        if (maxScroll > 0) {
            float handleSize = data->Size.Height * (data->Size.Height / data->MinSize.Height);
            float handlePos = std::lerp(0.0f, data->Size.Height - handleSize, data->Offset.Y / maxScroll);
            Rectangle({ 4.0f, handlePos }, { 0.0f, 0.0f, 0.0f, 0.0f });
            Rectangle({ 4.0f, handleSize }, self.style->ColorFrame);
        } else {
            Rectangle({ 4.0f, 16.0f }, { 0.0f, 0.0f, 0.0f, 0.0f });
        }
        EndGroup();
        EndGroup();
    }

    // Elements
    static bool Button(string_view label) {
        BeginGroup(Layout::Stack);
        self.group->FocusStyle = FocusStyle::Fill;
        self.group->FrameOpacity = 0.5f;
        bool focus = GroupHasFocus(FocusType::Mouse);
        SetPadding(8, 8);
        Text(label);
        SetAlign(0.5f, 0.5f);
        EndGroup();
        return focus && self.activate;
    }
    static bool Checkbox(string_view label, bool value) {
        BeginGroup(Layout::Horizontal);
        self.group->FocusStyle = FocusStyle::Underline;
        if (GroupHasFocus(FocusType::Mouse) && self.activate)
            value = !value;
        SetPadding(4, 4);
        SetSpacing(8);
        SetStretch(1, 0);

        Text(label);
        SetAlign(0.0f, 0.5f);
        SetStretch(1, 0);

        BeginGroup(Layout::Stack);
        Rectangle({ 16, 16 }, self.style->ColorFrame);
        if (value) {
            Rectangle({ 10, 10 }, self.style->ColorPrimary);
            SetAlign(0.5f, 0.5f);
        }
        EndGroup();
        SetStretch(0, 0);
        EndGroup();
        return value;
    }
    static void Image(Reference<Texture> Image) {
        HmGuiImage *e = new HmGuiImage();
        InitWidget(e, ControlType::Image);
        e->Image = Image;
        e->Stretch = { 1, 1 };
    }
    static float Slider(float Lower, float Upper, float value) {
        BeginGroup(Layout::Stack);
        Rectangle({ 0, 2 }, { 0.5f, 0.5f, 0.5f, 1.0f });
        SetAlign(0.5f, 0.5f);
        SetStretch(1, 0);
        EndGroup();
        return 0.0f;
    }
    static void Text(string_view text) {
        TextExtended(text, self.style->ColorText, self.style->Font);
    }
    static void TextColored(string_view text, const Color &color) {
        TextExtended(text, color, self.style->Font);
    }
    static void TextExtended(string_view text, const Color &color, Font *font) {
        HmGuiText *e = new HmGuiText();
        InitWidget(e, ControlType::Text);
        e->Text = text;
        e->Color = color;
        e->Font = font;
        e->MinSize = e->Font->GetSize(e->Text);
        SetAlign(0.0f, 1.0f);
    }

    // Simple Elements
    static void Rectangle(const Size &size, const Color &color) {
        HmGuiRect *e = new HmGuiRect();
        InitWidget(e, ControlType::Rect);
        e->Color = color;
        e->MinSize = size;
    }

    // States
    static bool GroupHasFocus(FocusType type) {
        self.group->Focusable[(int)type] = true;
        return self.focus[(int)type] == self.group->Hash;
    }

    // State Modifiers
    static void SetAlign(float x, float y) {
        self.last->Align = { x, y };
    }
    static void SetPadding(float x, float y) {
        self.group->Padding.Left = x;
        self.group->Padding.Top = y;
        self.group->Padding.Right = x;
        self.group->Padding.Bottom = y;
    }
    static void SetPadding(float left, float top, float right, float bottom) {
        self.group->Padding.Left = left;
        self.group->Padding.Top = top;
        self.group->Padding.Right = right;
        self.group->Padding.Bottom = bottom;
    }
    static void SetPaddingLeft(float left) {
        self.group->Padding.Left = left;
    }
    static void SetPaddingTop(float top) {
        self.group->Padding.Top = top;
    }
    static void SetPaddingRight(float right) {
        self.group->Padding.Right = right;
    }
    static void SetPaddingBottom(float bottom) {
        self.group->Padding.Bottom = bottom;
    }
    static void SetSpacing(float spacing) {
        self.group->Spacing = spacing;
    }
    static void SetStretch(float x, float y) {
        self.last->Stretch = { x, y };
    }

    static void PushFont(Font *Font) {
        PushStyle();
        self.style->Font = Font;
    }
    static void PushStyle() {
        HmGuiStyle *style = new HmGuiStyle();
        *style = *self.style;
        style->Previous = self.style;
        self.style = style;
    }
    static void PushTextColor(const Color &color) {
        PushStyle();
        self.style->ColorText = color;
    }
    static void PopStyle(int depth) {
        for (int i = 0; i < depth; ++i) {
            HmGuiStyle *style = self.style;
            self.style = style->Previous;
            delete style;
        }
    }

private:
    static void ComputeSize(HmGuiGroup *g) {
        for (HmGuiWidget *e = g->Head; e; e = e->Next)
            if (e->Type == ControlType::Panel)
                ComputeSize((HmGuiGroup *)e);

        g->MinSize = { 0, 0 };

        for (HmGuiWidget *e = g->Head; e; e = e->Next) {
            switch (g->Layout) {
                case Layout::Stack:
                    g->MinSize.Width = std::max(g->MinSize.Width, e->MinSize.Width);
                    g->MinSize.Height = std::max(g->MinSize.Height, e->MinSize.Height);
                    break;
                case Layout::Vertical:
                    g->MinSize.Width = std::max(g->MinSize.Width, e->MinSize.Width);
                    g->MinSize.Height += e->MinSize.Height;
                    if (e != g->Head) g->MinSize.Height += g->Spacing;
                    break;
                case Layout::Horizontal:
                    g->MinSize.Width += e->MinSize.Width;
                    g->MinSize.Height = std::max(g->MinSize.Height, e->MinSize.Height);
                    if (e != g->Head) g->MinSize.Width += g->Spacing;
                    break;
            }
        }

        g->MinSize.Width += g->Padding.Left + g->Padding.Right;
        g->MinSize.Height += g->Padding.Top + g->Padding.Bottom;

        if (g->StoreSize) {
            HmGuiData *data = GetData(g);
            data->MinSize = g->MinSize;
        }

        g->MinSize.Width = std::min(g->MinSize.Width, g->MaxSize.Width);
        g->MinSize.Height = std::min(g->MinSize.Height, g->MaxSize.Height);
    }
    static void LayoutGroup(HmGuiGroup *g) {
        Position pos = g->Position;
        Size size = g->Size;
        float extra = 0;
        float TotalStretch = 0;

        pos.X += g->Padding.Left + g->Offset.X;
        pos.Y += g->Padding.Top + g->Offset.Y;
        size.Width -= g->Padding.Left + g->Padding.Right;
        size.Height -= g->Padding.Top + g->Padding.Bottom;

        if (g->Expand) {
            if (g->Layout == Layout::Vertical) {
                extra = g->Size.Height - g->MinSize.Height;
                for (HmGuiWidget *e = g->Head; e; e = e->Next)
                    TotalStretch += e->Stretch.Y;
            } else if (g->Layout == Layout::Horizontal) {
                extra = g->Size.Width - g->MinSize.Width;
                for (HmGuiWidget *e = g->Head; e; e = e->Next)
                    TotalStretch += e->Stretch.X;
            }

            if (TotalStretch > 0)
                extra /= TotalStretch;
        }

        float s;
        for (HmGuiWidget *e = g->Head; e; e = e->Next) {
            switch (g->Layout) {
                case Layout::None:
                    LayoutWidget(e, e->Position, size);
                    break;
                case Layout::Stack:
                    LayoutWidget(e, pos, size);
                    break;
                case Layout::Vertical:
                    s = e->MinSize.Height;
                    if (extra > 0) s += e->Stretch.Y * extra;
                    LayoutWidget(e, pos, { size.Width, s });
                    pos.Y += e->Size.Height + g->Spacing;
                    break;
                case Layout::Horizontal:
                    s = e->MinSize.Width;
                    if (extra > 0) s += e->Stretch.X * extra;
                    LayoutWidget(e, pos, { s, size.Height });
                    pos.X += e->Size.Width + g->Spacing;
                    break;
            }

            if (e->Type == ControlType::Panel)
                LayoutGroup((HmGuiGroup *)e);
        }


        if (g->StoreSize) {
            HmGuiData *data = GetData(g);
            data->Size = g->Size;
        }
    }
    static void LayoutWidget(HmGuiWidget *e, Position position, Size size) {
        e->Position = position;
        e->Size = e->MinSize;
        e->Size.Width += e->Stretch.X * (size.Width - e->MinSize.Width);
        e->Size.Height += e->Stretch.Y * (size.Height - e->MinSize.Height);
        e->Position.X += e->Align.X * (size.Width - e->Size.Width);
        e->Position.Y += e->Align.Y * (size.Height - e->Size.Height);
    }
    static void InitWidget(HmGuiWidget *e, ControlType type) {
        e->Parent = self.group;
        e->Next = 0;
        e->Previous = self.group ? self.group->Tail : 0;

        std::hash<uint64_t> hasher;
        if (e->Parent) {
            e->Parent->Children++;
            e->Hash = hasher(e->Parent->Hash ^ e->Parent->Children);
            (e->Next ? e->Next->Previous : e->Parent->Tail) = e;
            (e->Previous ? e->Previous->Next : e->Parent->Head) = e;
        } else {
            e->Hash = hasher(0);
        }

        e->Type = type;
        e->Position = { 0, 0 };
        e->Size = { 0, 0 };
        e->MinSize = { 0, 0 };
        e->Align = { 0, 0 };
        e->Stretch = { 0, 0 };

        self.last = e;
    }

    static void CheckFocus(HmGuiGroup *g) {
        if (g->Clip && IsClipped(g, self.FocusPos)) return;

        for (HmGuiWidget *e = g->Tail; e; e = e->Previous) {
            if (e->Type == ControlType::Panel) CheckFocus((HmGuiGroup *)e);
        }

        for (int i = 0; i < (int)FocusType::Size; ++i) {
            if (self.focus[i] == 0 && g->Focusable[i]) {
                if (g->Position.X <= self.FocusPos.X &&
                    g->Position.Y <= self.FocusPos.Y &&
                    self.FocusPos.X <= g->Position.X + g->Size.Width &&
                    self.FocusPos.Y <= g->Position.Y + g->Size.Height) {
                    self.focus[i] = g->Hash;
                }
            }
        }
    }
    static HmGuiData *GetData(HmGuiGroup *g) {
        auto data = self.data[g->Hash];
        if (!data) {
            data = new HmGuiData();
            data->Offset = { 0, 0 };
            data->MinSize = { 0, 0 };
            data->Size = { 0, 0 };
            self.data[g->Hash] = data;
        }
        return data;
    }
    inline static bool IsClipped(HmGuiGroup *g, Position position) {
        return
            position.X < g->Position.X || position.Y < g->Position.Y ||
            g->Position.X + g->Size.Width < position.X ||
            g->Position.Y + g->Size.Height < position.Y;
    }

    static void PushClipRect(HmGuiGroup *g) {
        HmGuiClipRect *rect = new HmGuiClipRect();
        rect->Previous = self.clipRect;
        rect->Lower = g->Position;
        // ToDo: :(
        //rect->Upper = { g->Position, g->Size };
        if (rect->Previous) {
            rect->Lower.X = std::max(rect->Lower.X, rect->Previous->Lower.X);
            rect->Lower.Y = std::max(rect->Lower.Y, rect->Previous->Lower.Y);
            rect->Upper.X = std::min(rect->Upper.X, rect->Previous->Upper.X);
            rect->Upper.Y = std::min(rect->Upper.Y, rect->Previous->Upper.Y);
        }
        self.clipRect = rect;
    }
    static void PopClipRect() {
        HmGuiClipRect *rect = self.clipRect;
        self.clipRect = rect->Previous;
        delete rect;
    }

    // Replacement Done...
    static void DrawGroup(HmGuiGroup *g) {
        UIRenderer::BeginLayer(g->Position, g->Size, g->Clip);

        for (HmGuiWidget *e = g->Tail; e; e = e->Previous) {
            switch (e->Type) {
                case ControlType::Panel: {
                    DrawGroup((HmGuiGroup *)e);
                    break;
                }
                case ControlType::Image: {
                    DrawImage((HmGuiImage *)e);
                    break;
                }
                case ControlType::Line: {
                    DrawLine((HmGuiLine *)e);
                }
                case ControlType::Rect: {
                    DrawRect((HmGuiRect *)e);
                    break;
                }
                case ControlType::Text: {
                    DrawText((HmGuiText *)e);
                    break;
                }
            }
        }

        if (g->Focusable[(int)FocusType::Mouse]) {
            bool focus = self.focus[(int)FocusType::Mouse] == g->Hash;

            switch (g->FocusStyle) {
                case FocusStyle::None: {
                    const auto color = Color(0.1f, 0.12f, 0.13f, 1.0f);
                    UIRenderer::Panel(g->Position, g->Size, color, 8.0f, g->FrameOpacity);
                    break;
                }
                case FocusStyle::Fill: {
                    if (focus) {
                        const auto color = Color(0.1f, 0.5f, 1.0f, 1.0f);
                        UIRenderer::Panel(g->Position, g->Size, color, 0.0f, 1.0f);
                    } else {
                        const auto color = Color(0.15f, 0.15f, 0.15f, 0.8f);
                        UIRenderer::Panel(g->Position, g->Size, color, 0.0f, g->FrameOpacity);
                    }
                    break;
                }
                case FocusStyle::Outline: {
                    if (focus) {
                        const auto color = Color(0.1f, 0.5f, 1.0f, 1.0f);
                        UIRenderer::Rect(g->Position, g->Size, color, true);
                    }
                    break;
                }
                case FocusStyle::Underline: {
                    auto color = Color(0.3f, 0.3f, 0.3f, focus ? 0.5f : g->FrameOpacity);
                    UIRenderer::Rect(g->Position, g->Size, color, false);
                    break;
                }
            }
        }

        if constexpr (DrawGroupFrames) {
            UIRenderer::DrawColor(0.2f, 0.2f, 0.2f, 0.5f);
            UIRenderer::DrawBorder(2.0f, g->Position.X, g->Position.Y, g->Size.Width, g->Size.Height);
        }

        UIRenderer::EndLayer();
    }
    static void DrawImage(HmGuiImage *e) {
        UIRenderer::Image(e->Position, e->Size, e->Image);
    }
    static void DrawLine(HmGuiLine *e) {
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
    }
    static void DrawRect(HmGuiRect *e) {
        UIRenderer::Rect(e->Position, e->Size, e->Color, false);
    }
    static void DrawText(HmGuiText *e) {
        if constexpr (DrawGroupFrames) {
            UIRenderer::DrawColor(0.5f, 0.2f, 0.2f, 0.5f);
            UIRenderer::DrawBorder(1.0f, e->Position.X, e->Position.Y, e->Size.Width, e->Size.Height);
        }

        Position position = { e->Position.X, e->Position.Y + e->MinSize.Height };
        UIRenderer::Text(position, e->Text, e->Color, e->Font);
    }

    static void FreeGroup(HmGuiGroup *g) {
        HmGuiWidget *e = g->Head;
        while (e) {
            HmGuiWidget *Next = e->Next;
            switch (e->Type) {
                case ControlType::Panel: FreeGroup((HmGuiGroup *)e); break;
                case ControlType::Text: FreeText((HmGuiText *)e); break;
                default: delete e; break;
            }
            e = Next;
        }
        delete g;
    }
    static void FreeText(HmGuiText *e) {
        delete e;
    }

private:
    inline static struct HmGuiDataStructure {
        HmGuiGroup *root;
        HmGuiGroup *group;
        
        unordered_map<uint64_t, HmGuiData *> data;

        HmGuiWidget *last;
        HmGuiStyle *style;
        HmGuiClipRect *clipRect;

        bool activate;
        uint64_t focus[(int)FocusType::Size];
        Position FocusPos;
    } self = { 0 };
};

}

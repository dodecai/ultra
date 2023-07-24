export module Ultra.Engine.UIRenderer;

import Ultra.Core;
import Ultra.Engine.Phoenix;
import Ultra.Logger;
import Ultra.Math;

export import Ultra.Engine.Font;

///
/// @brief UIRenderer: This is a simple ui renderer, nothing more, nothing less.
///
export namespace Ultra {

///
/// @brief Types
///

struct Color {
    float Red;
    float Green;
    float Blue;
    float Alpha;
};

struct Position {
    float X;
    float Y;
};

struct Size {
    float Width;
    float Height;
};

///
/// @brief Elements
///

class UIElement {
public:
    UIElement(Position position, Size size): mPosition(position), mSize(size) {
    }
    virtual ~UIElement() = default;

    Position GetPosition() const { return mPosition; }
    Size GetSize() const { return mSize; }

    virtual void Draw() const = 0;

public:
    Position mPosition {};
    Size mSize {};
};

class ClipRect {
    struct ClipRectData {
        bool Enabled;
        Ultra::Position Position;
        Ultra::Size Size;
    };

    struct ClipRectTransform {
        float tx, ty;
        float sx, sy;
    };

public:
    static void Push(const Position &position, const Size &size);
    static void PushCombined(const Position &position, const Size &size);
    static void PushDisabled();
    static void PushTransform(const Position &position, const Size &size);
    static void Pop();
    static void PopTransform();

private:
    static void Activate();
    inline static void TransformRectangle(float &x, float &y, float &sx, float &sy);
    static bool Validate();

private:
    inline static constexpr int mStackDepth = 128;
    inline static int mCurrentIndex = -1;
    inline static int mCurrentTransformIndex = -1;

    inline static array<ClipRectData, mStackDepth> mRectangle {};
    inline static array<ClipRectTransform, mStackDepth> mTransform {};
};

class Image: public UIElement {
public:
    Image(Position position, Size size, Tex2DData *image):
        UIElement(position, size), mImage(image) {
    }
    ~Image() = default;

    void Draw() const override {
        Tex2D::Draw(mImage, mPosition.X, mPosition.Y, mSize.Width, mSize.Height);
    }

private:
    Tex2DData *mImage;
};

class Panel: public UIElement {
public:
    Panel(Position position, Size size, Color color, float bevel, float innerAlpha):
        UIElement(position, size), mColor(color), mBevel(bevel), mInnerAlpha(innerAlpha) {
    }
    ~Panel() = default;

    void Draw() const override {
    }

public:
    Color mColor {};
    float mBevel {};
    float mInnerAlpha {};
};

class Rectangle: public UIElement {
public:
    Rectangle(Position position, Size size, Color color, bool outline):
        UIElement(position, size), mColor(color), mOutline(outline) {
    }
    ~Rectangle() = default;

    void Draw() const override {
        Draw::Color(mColor.Red, mColor.Green, mColor.Blue, mColor.Alpha);
        if (mOutline) {
            Draw::Border(1.0f, mPosition.X, mPosition.Y, mSize.Width, mSize.Height);
        } else {
            Draw::Rect(mPosition.X, mPosition.Y, mSize.Width, mSize.Height);
        }
    }

private:
    Color mColor {};
    bool mOutline {};
};

class Text: public UIElement {
public:
    Text(Position position, Size size, const string &text, Color color, FontData *font):
        UIElement(position, size), mText(text), mColor(color), mFont(font) {
    }
    ~Text() = default;

    void Draw() const override {
        Font::Draw(mFont, mText.c_str(), mPosition.X, mPosition.Y, mColor.Red, mColor.Green, mColor.Blue, mColor.Alpha);
    }

private:
    Color mColor;
    string mText;
    FontData *mFont;
};

///
/// @brief Layer
///

class UILayer: public std::enable_shared_from_this<UILayer> {
public:
    // Default
    UILayer(Position position, Size size, bool clip):
        mPosition(position), mSize(size), mClip(clip) {
        if (!mShader) mShader = PhxShader::Load("vertex/ui", "fragment/ui/panel");
    }
    ~UILayer() {
        PhxShader::Free(mShader);
    };

    // Methods
    void AddChild(Reference<UILayer> element) {
        element->mParent = shared_from_this();
        mChildren.push_back(std::move(element));
    }
    void AddElement(Scope<UIElement> element) {
        mElements.push_back(std::move(element));
    }
    void AddPanel(Scope<Panel> element) {
        mPanels.push_back(std::move(element));
    }
    void Draw() const {
        if (!mPanels.empty()) {
            const float pad = 64.0f;

            PhxShader::Start(mShader);
            PhxShader::SetFloat("padding", pad);

            for (auto &panel : mPanels) {
                float x = panel->mPosition.X - pad;
                float y = panel->mPosition.Y - pad;
                float sx = panel->mSize.Width + 2.0f * pad;
                float sy = panel->mSize.Height + 2.0f * pad;

                PhxShader::SetFloat("innerAlpha", panel->mInnerAlpha);
                PhxShader::SetFloat("bevel", panel->mBevel);
                PhxShader::SetFloat2("size", sx, sy);
                PhxShader::SetFloat4("color", panel->mColor.Red, panel->mColor.Green, panel->mColor.Blue, panel->mColor.Alpha);
                Draw::Rect(x, y, sx, sy);
            }

            PhxShader::Stop(mShader);
        }

        if (mElements.empty()) return;
        for (const auto &element : mElements) {
            element->Draw();
        }
    }

    // Accessors
    const vector<Reference<UILayer>> &GetChildren() {
        return mChildren;
    }
    const Reference<UILayer> &GetParent() const {
        return mParent;
    }
    const Position &GetPosition() const {
        return mPosition;
    }
    const Size &GetSize() const {
        return mSize;
    }

    // States
    bool Clipped() {
        return mClip;
    }

private:
    // Properties
    const bool mClip;
    Position mPosition {};
    Size mSize {};

    // UI Tree
    Reference<UILayer> mParent {};
    vector<Reference<UILayer>> mChildren {};

    // Panels and Elements to be drawn
    vector<Scope<Panel>> mPanels {};
    vector<Scope<UIElement>> mElements {};

    // Shaders
    inline static ShaderData *mShader = nullptr;
};

///
/// @brief Renderer
///

class UIRenderer {
public:
    // Default
    UIRenderer() = default;
    ~UIRenderer() = default;
    static UIRenderer &Instance() {
        static UIRenderer instance;
        return instance;
    }

    // Functions
    static void Begin() {
        static auto &initialize = UIRenderer::Instance();

        Instance().mLayers.clear();

        Vector2Di nativeSize {};
        Viewport::GetSize(&nativeSize);
        auto size = Size { (float)nativeSize.x, (float)nativeSize.y };
        BeginLayer({}, size, true);
    }
    static void BeginLayer(Position position, Size size, bool clip) {
        auto layer = CreateReference<UILayer>(position, size, clip);

        if (mCurrentLayer) {
            mCurrentLayer->AddChild(layer);
        }
        mCurrentLayer = layer;

        if (!mCurrentLayer->GetParent()) {
            Instance().mLayers.push_back(layer);
        }
    }
    static void EndLayer() {
        mCurrentLayer = mCurrentLayer->GetParent();
    }
    static void End() {
        EndLayer();
    }
    static void Draw() {
        RenderState::PushBlendMode(PhxBlendMode::Alpha);
        UIRenderer::Instance().Render();
        RenderState::PopBlendMode();
    }

    static void Panel(const Position &position, const Size &size, const Color &color, float bevel, float innerAlpha) {
        mCurrentLayer->AddPanel(CreateScope<Ultra::Panel>(position, size, color, bevel, innerAlpha));
    }
    static void Rect(const Position &position, const Size &size, const Color &color, bool outline) {
        mCurrentLayer->AddElement(CreateScope<Rectangle>(position, size, color, outline));
    }
    static void Image(const Position &position, const Size &size, Tex2DData *image) {
        mCurrentLayer->AddElement(CreateScope<Ultra::Image>(position, size, image));
    }
    static void Text(const Position &position, const string &text, const Color &color, FontData *font) {
        mCurrentLayer->AddElement(CreateScope<Ultra::Text>(position, Size {}, text.c_str(), color, font));
    }

private:
    // Methods
    void AddLayer(Reference<UILayer> element) {
        mLayers.push_back(std::move(element));
    }
    void Render() {
        function<void(const Reference<UILayer> &)> RenderLayer = [&](const Reference<UILayer> &layer) {
            if (layer->Clipped()) {
                ClipRect::PushCombined(layer->GetPosition(), layer->GetSize());
            }

            layer->Draw();

            auto children = layer->GetChildren();
            for (auto it = children.rbegin(); it != children.rend(); it++) {
                RenderLayer(*it);
            }

            if (layer->Clipped()) {
                ClipRect::Pop();
            }
        };

        for (const auto &layer : mLayers) {
            RenderLayer(layer);
        }
    }

private:
    // Properties
    inline static Reference<UILayer> mCurrentLayer = nullptr;
    vector<Reference<UILayer>> mLayers;
};

}

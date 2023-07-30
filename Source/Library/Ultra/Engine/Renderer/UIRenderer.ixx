
export module Ultra.Engine.UIRenderer;

import Ultra.Core;
import Ultra.Engine.Renderer.PipelineState;
import Ultra.Engine.Renderer.Shader;
import Ultra.Engine.Renderer.Texture;
import Ultra.Engine.Renderer.Viewport;
import Ultra.Logger;
import Ultra.Math;

#define TEST_NEW_RENDERER1

export import Ultra.Engine.Font;

///
/// @brief UIRenderer: This is a simple ui renderer, nothing more, nothing less.
///
export namespace Ultra {

///
/// @brief Types
///

struct Alignment {
    float X {};
    float Y {};
};

struct Color {
    float Red {};
    float Green {};
    float Blue {};
    float Alpha {};
};

struct Padding {
    float Left {};
    float Top {};
    float Bottom {};
    float Right {};

    explicit Padding(float x, float y):
        Left(x), Right(x),
        Top(y), Bottom(y) {
    }
};

struct Position {
    float X;
    float Y;
};

struct Size {
    float Width;
    float Height;
};

struct Stretch {
    float X {};
    float Y {};
};


///
/// @brief Elements
///

class UIDraw {
public:
    static void Load();
    static void StartFrame();
    static void FinishFrame();

    static void Flush();
    static void FlushAndReset();
    static void DrawPanel(glm::vec2 position, glm::vec2 size, glm::vec4 color, float innerAlpha, float bevel);
    static void DrawRectangle(glm::vec2 position, glm::vec2 size, glm::vec4 color = glm::vec4(1.0f));
    static void DrawText(glm::vec2 position, glm::vec2 size, glm::vec4 color, glm::vec2 uv1, glm::vec2 uv2);

    // Legacy
    static void DrawBorder(float s, float x, float y, float w, float h);
    static void DrawColor(float r, float g, float b, float a);
    static void DrawRect(float x1, float y1, float xs, float ys);

private:
    inline static glm::mat4 ProjectionMatrix {};
    inline static glm::mat4 ModelViewMatrix = glm::mat4(1.0f);
};

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
    static void PushCombined(const Position &position, const Size &size, Viewport *viewport);
    static void PushDisabled();
    static void PushTransform(const Position &position, const Size &size);
    static void Pop();
    static void PopTransform();

private:
    static void Activate();
    inline static void TransformRectangle(float &x, float &y, float &sx, float &sy);
    static bool Validate();

private:
    inline static Viewport *mViewport = nullptr;

    inline static constexpr int mStackDepth = 128;
    inline static int mCurrentIndex = -1;
    inline static int mCurrentTransformIndex = -1;

    inline static array<ClipRectData, mStackDepth> mRectangle {};
    inline static array<ClipRectTransform, mStackDepth> mTransform {};
};

class Image: public UIElement {
public:
    Image(Position position, Size size, Texture *image):
        UIElement(position, size), mImage(image) {
    }
    ~Image() = default;

    void Draw() const override {
        mImage->Draw(0, mPosition.X, mPosition.Y, mSize.Width, mSize.Height);
    }

private:
   Texture *mImage = nullptr;
};

class Panel: public UIElement {
public:
    Panel(Position position, Size size, Color color, float bevel, float innerAlpha):
        UIElement(position, size), mColor(color), mBevel(bevel), mInnerAlpha(innerAlpha) {
    }
    ~Panel() = default;

    void Draw() const override {
        const float pad = 64.0f;
        float x = mPosition.X - pad;
        float y = mPosition.Y - pad;
        float width = mSize.Width + 2.0f * pad;
        float height = mSize.Height + 2.0f * pad;

        UIDraw::DrawPanel({ x, y }, { width, height }, { mColor.Red, mColor.Green, mColor.Blue, mColor.Alpha }, mInnerAlpha, mBevel);
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
        UIDraw::DrawColor(mColor.Red, mColor.Green, mColor.Blue, mColor.Alpha);
        if (mOutline) {
            //UIDraw::DrawBorder2(1.0f, { mPosition.X, mPosition.Y }, { mSize.Width, mSize.Height }, { mColor.Red, mColor.Green, mColor.Blue, mColor.Alpha });
        } else {
            //UIDraw::DrawRectangle({ mPosition.X, mPosition.Y }, { mSize.Width, mSize.Height }, { mColor.Red, mColor.Green, mColor.Blue, mColor.Alpha });
            UIDraw::DrawRect(mPosition.X, mPosition.Y, mSize.Width, mSize.Height);
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

class UILayer {
public:
    // Default
    UILayer(Position position, Size size, bool clip):
        mPosition(position), mSize(size), mClip(clip) {
        // vertex/ui & fragment/ui/panel
        if (!mUIShader) mUIShader = Shader::Create("Assets/Shaders/UI.glsl");
    }
    ~UILayer() {
        //PhxShader::Free(mShader);
    };

    // Methods
    void AddChild(Reference<UILayer> element) {
        element->mParent = this;
        mChildren.push_back(std::move(element));
    }
    void AddElement(Scope<UIElement> element) {
        mElements.push_back(std::move(element));
    }
    void AddPanel(Scope<Panel> element) {
        mPanels.push_back(std::move(element));
    }
    void Draw() {
        if (!mPanels.empty()) {
        #ifdef TEST_NEW_RENDERER
            for (const auto &panel : mPanels) {
                panel->Draw();
            }
        #else

            const float pad = 64.0f;

            mUIShader->Bind();
            mUIShader->UpdateUniform("uPadding", pad);

            for (auto &panel : mPanels) {
                float x = panel->mPosition.X - pad;
                float y = panel->mPosition.Y - pad;
                float sx = panel->mSize.Width + 2.0f * pad;
                float sy = panel->mSize.Height + 2.0f * pad;

                mUIShader->UpdateUniform("uColor", Float4 { panel->mColor.Red, panel->mColor.Green, panel->mColor.Blue, panel->mColor.Alpha });
                mUIShader->UpdateUniform("uSize", Float2 { sx, sy });
                mUIShader->UpdateUniform("uInnerAlpha", panel->mInnerAlpha);
                mUIShader->UpdateUniform("uBevel", panel->mBevel);

                UIDraw::DrawRect(x, y, sx, sy);
            }

            mUIShader->Unbind();

        #endif
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
    UILayer *GetParent() const {
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
    UILayer *mParent {};
    vector<Reference<UILayer>> mChildren {};

    // Panels and Elements to be drawn
    vector<Scope<Panel>> mPanels {};
    vector<Scope<UIElement>> mElements {};

    // Shaders
    inline static Reference<Shader> mUIShader;
};

///
/// @brief Renderer
///

class UIRenderer {
public:
    // Default
    UIRenderer() {
        PipelineProperties properties {};
        properties.BlendMode = BlendMode::Alpha;
        properties.DepthTest = true;
        mPipelineState = PipelineState::Create(properties);

        UIDraw::Load();
    }
    ~UIRenderer() = default;
    static UIRenderer &Instance() {
        static UIRenderer instance;
        return instance;
    }

    // Functions
    static void Begin(const Scope<Viewport> &viewport) {
        static auto &initialize = UIRenderer::Instance();

        mViewport = viewport.get();
        auto properties = viewport->GetProperties();
        auto size = Size { properties.Width, properties.Height };

        BeginLayer({}, size, true);
    }
    static void BeginLayer(Position position, Size size, bool clip) {
        auto &&layer = CreateReference<UILayer>(position, size, clip);

        if (mCurrentLayer) {
            mCurrentLayer->AddChild(layer);
        }
        mCurrentLayer = layer.get();

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
        UIRenderer::Instance().mPipelineState->Bind();
        UIDraw::StartFrame();
        UIRenderer::Instance().Render();
        UIDraw::FinishFrame();
        UIRenderer::Instance().mPipelineState->Unbind();
    }

    static void Panel(const Position &position, const Size &size, const Color &color, float bevel, float innerAlpha) {
        mCurrentLayer->AddPanel(CreateScope<Ultra::Panel>(position, size, color, bevel, innerAlpha));
    }
    static void Rect(const Position &position, const Size &size, const Color &color, bool outline) {
        mCurrentLayer->AddElement(CreateScope<Rectangle>(position, size, color, outline));
    }
    static void Image(const Position &position, const Size &size, Texture *image) {
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
                ClipRect::PushCombined(layer->GetPosition(), layer->GetSize(), mViewport);
            }

            layer->Draw();

            const auto &children = layer->GetChildren();
            for (auto it = children.rbegin(); it != children.rend(); it++) {
                RenderLayer(*it);
            }

            if (layer->Clipped()) {
                ClipRect::Pop();
            }
        };

        for (auto &layer : mLayers) {
             RenderLayer(layer);
        }

        mLayers.clear();
    }

private:
    // Properties
    inline static Scope<PipelineState> mPipelineState {};
    inline static UILayer *mCurrentLayer {};
    vector<Reference<UILayer>> mLayers;
    inline static Viewport *mViewport = nullptr;
};

}

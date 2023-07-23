export module Ultra.Engine.UIRenderer;

import Ultra.Core;
import Ultra.Logger;
import Ultra.Math;
import Ultra.Engine.Phoenix;
export import Ultra.Engine.Font;

export namespace Ultra {

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

class UILayer {
public:
    UILayer(Position position, Size size, bool clip):
        mPosition(position), mSize(size), mClip(clip) {
        if (!mShader) mShader = PhxShader::Load("vertex/ui", "fragment/ui/panel");
    }
    ~UILayer() {
        mElements.clear();
    };
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

public:
    bool mClip;
    Position mPosition {};
    Size mSize {};
    vector<Scope<Panel>> mPanels;
    vector<Scope<UIElement>> mElements;

    static ShaderData *mShader;
};


class UIRenderer {
public:
    UIRenderer() = default;
    ~UIRenderer() = default;

    // Methods
    void AddLayer(Reference<UILayer> element) {
        mLayers.push_back(std::move(element));
    }
    void BeginFrame() {

    }
    void FinishFrame() {

    }
    void Render() {
        for (const auto &layer : mLayers) {
            if (layer->mClip) ClipRect::PushCombined(layer->mPosition.X, layer->mPosition.Y, layer->mSize.Width, layer->mSize.Height);
            layer->Draw();
            if (layer->mClip) ClipRect::Pop();
        }
    }
    Reference<UILayer> GetLastLayer() {
        if (!mLayers.empty()) return mLayers.back();
        else return nullptr;
    }

    ///
    /// @brief ToDo
    ///
    static UIRenderer &Instance() {
        static UIRenderer instance;
        return instance;
    }
    
    static void Begin();
    static void BeginLayer(float x, float y, float sx, float sy, bool clip);
    static void EndLayer();
    static void End();
    static void Draw();

    static void Image(Tex2DData *, float x, float y, float sx, float sy);
    static void Panel(float x, float y, float sx, float sy, float r, float g, float b, float a, float bevel, float innerAlpha);
    static void Rect(float x, float y, float sx, float sy, float r, float g, float b, float a, bool outline);
    static void Text(FontData *font, const char  *text, float x, float y, float r, float g, float b, float a);

public:
    vector<Reference<UILayer>> mLayers;
};

}

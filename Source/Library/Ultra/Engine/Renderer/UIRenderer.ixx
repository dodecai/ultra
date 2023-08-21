
export module Ultra.Engine.UIRenderer;

export import Ultra.Engine.Renderer.Data;

import Ultra.Core;
import Ultra.Engine.Renderer.CommandBuffer;
import Ultra.Engine.Renderer.PipelineState;
import Ultra.Engine.Renderer.Shader;
import Ultra.Engine.Renderer.Texture;
import Ultra.Engine.Renderer.Viewport;
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

struct Alignment {
    float X {};
    float Y {};
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
    Image(Position position, Size size, Reference<Texture> image, const Color &color = { 1.0f, 1.0f, 1.0f, 1.0f }):
        UIElement(position, size),
        mImage(image),
        mColor(color) {
    }
    ~Image() = default;

    void Draw() const override;

private:
   Color mColor;
   Reference<Texture> mImage;
};

class Panel: public UIElement {
public:
    Panel(Position position, Size size, Color color, float bevel, float innerAlpha):
        UIElement(position, size), mColor(color), mBevel(bevel), mInnerAlpha(innerAlpha) {
    }
    ~Panel() = default;

    void Draw() const override;

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

    void Draw() const override;

private:
    Color mColor {};
    bool mOutline {};
};

class Text: public UIElement {
public:
    Text(Position position, Size size, const string &text, Color color, Font *font):
        UIElement(position, size), mText(text), mColor(color), mFont(font) {
    }
    ~Text() = default;

    void Draw() const override;

private:
    Color mColor;
    string mText;
    Font *mFont;
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
            for (const auto &panel : mPanels) {
                panel->Draw();
            }
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

struct UIComponent {
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TextureCoordinate;
    float TextureIndex;
    float TilingFactor;
};

struct UIPanelComponent {
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 Size;
    float InnerAlpha;
    float Bevel;
    glm::vec2 TextureCoordinate;
};

class UIRenderer {
public:
    // Default
    UIRenderer() {
        // Panel
        {
            SRenderData.PanelShader = Shader::Create("Assets/Shaders/UI.glsl");
            PipelineProperties panelProperties;
            panelProperties.BlendMode = BlendMode::Alpha;
            panelProperties.DepthTest = true;
            panelProperties.Layout = {
                { ShaderDataType::Float3, "aPosition" },
                { ShaderDataType::Float4, "aColor" },
                { ShaderDataType::Float2, "aSize" },
                { ShaderDataType::Float, "aInnerAlpha" },
                { ShaderDataType::Float, "aBevel" },
                { ShaderDataType::Float2, "aTextureCoordinate" },
            };
            SRenderData.PanelPipeline = PipelineState::Create(panelProperties);

            SRenderData.PanelVertexBufferData.reserve(SRenderData.PanelMaxVertices);
            SRenderData.PanelVertexBuffer = Buffer::Create(BufferType::Vertex, nullptr, SRenderData.PanelMaxVertices * sizeof(UIPanelComponent));
            SRenderData.PanelVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
            SRenderData.PanelVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
            SRenderData.PanelVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
            SRenderData.PanelVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

            uint32_t offset = 0;
            uint32_t *indices = new uint32_t[SRenderData.PanelMaxIndices];
            for (uint32_t i = 0; i < SRenderData.PanelMaxIndices; i += 6) {
                indices[i + 0] = offset + 0;
                indices[i + 1] = offset + 1;
                indices[i + 2] = offset + 2;

                indices[i + 3] = offset + 2;
                indices[i + 4] = offset + 3;
                indices[i + 5] = offset + 0;
                offset += 4;
            }
            SRenderData.PanelIndexBuffer = Buffer::Create(BufferType::Index, indices, SRenderData.PanelMaxVertices * sizeof(uint32_t));
            delete[] indices;
        }

        // Component
        {
            SRenderData.ComponentShader = Shader::Create("Assets/Shaders/UIElement.glsl");
            PipelineProperties componentProperties;
            componentProperties.BlendMode = BlendMode::Alpha;
            componentProperties.DepthTest = true;
            componentProperties.Layout = {
                { ShaderDataType::Float3, "aPosition" },
                { ShaderDataType::Float4, "aColor" },
                { ShaderDataType::Float2, "aTextureCoordinate" },
                { ShaderDataType::Float, "aTextureIndex" },
                { ShaderDataType::Float, "aTilingFactor" },
            };
            SRenderData.ComponentPipeline = PipelineState::Create(componentProperties);

            SRenderData.ComponentVertexBufferData.reserve(SRenderData.ComponentMaxVertices);
            SRenderData.ComponentVertexBuffer = Buffer::Create(BufferType::Vertex, nullptr, SRenderData.ComponentMaxVertices * sizeof(UIComponent));
            SRenderData.ComponentVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
            SRenderData.ComponentVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
            SRenderData.ComponentVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
            SRenderData.ComponentVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

            uint32_t offset = 0;
            uint32_t *indices = new uint32_t[SRenderData.ComponentMaxIndices];
            for (uint32_t i = 0; i < SRenderData.ComponentMaxIndices; i += 6) {
                indices[i + 0] = offset + 0;
                indices[i + 1] = offset + 1;
                indices[i + 2] = offset + 2;

                indices[i + 3] = offset + 2;
                indices[i + 4] = offset + 3;
                indices[i + 5] = offset + 0;
                offset += 4;
            }
            SRenderData.ComponentIndexBuffer = Buffer::Create(BufferType::Index, indices, SRenderData.ComponentMaxVertices * sizeof(uint32_t));
            delete[] indices;

            // Texture
            uint32_t whiteTextureData = 0xffffffff;
            SRenderData.WhiteTexture = Texture::Create(TextureProperties(), &whiteTextureData, sizeof(uint32_t));

            int32_t samplers[SRenderData.MaxTextureSlots];
            for (uint32_t i = 0; i < SRenderData.MaxTextureSlots; i++) samplers[i] = i;
            SRenderData.ComponentShader->Bind();
            SRenderData.ComponentShader->UpdateUniformBuffer("uTextures", (void *)samplers, SRenderData.MaxTextureSlots);
            SRenderData.TextureSlots[0] = SRenderData.WhiteTexture;


            CheckerBoard = Texture::Create(TextureProperties(), "Assets/Textures/smiley.png");
        }

        SCommandBuffer = CommandBuffer::Create();

        SRenderData.TransformUniformBuffer = Buffer::Create(BufferType::Uniform, nullptr, sizeof(RenderData::TransformUniform));
        SRenderData.TransformUniformBuffer->Bind(0);
        SRenderData.PanelPropertiesUniformBuffer = Buffer::Create(BufferType::Uniform, nullptr, sizeof(RenderData::PanelPropertiesUniform));
        SRenderData.PanelPropertiesUniformBuffer->Bind(1);

        // Legacy
        PipelineProperties properties {};
        properties.BlendMode = BlendMode::Alpha;
        properties.DepthTest = true;
        mPipelineState = PipelineState::Create(properties);

        //mBackground = Texture::Create(TextureProperties(), "Assets/Textures/Wallpaper.png");;
    }
    ~UIRenderer() = default;
    static UIRenderer &Instance() {
        static UIRenderer instance;
        return instance;
    }

    void BeginScene() {
        auto windowWidth = 1280.0f;
        auto windowHeight = 1024.0f;
        //float fov = 45.0f;
        //float nearPlane = 0.1f;
        //float farPlane = 100.0f;

        SRenderData.Transform.ProjectionMatrix = glm::ortho(0.0f, windowWidth, windowHeight, 0.0f);
        // glm::perspective(glm::radians(fov), windowWidth / windowHeight, nearPlane, farPlane);
        SRenderData.Transform.ViewMatrix = glm::mat4(1.0f);

        SRenderData.TransformUniformBuffer->UpdateData(&SRenderData.Transform, sizeof(RenderData::TransformUniform));
        SRenderData.PanelPropertiesUniformBuffer->UpdateData(&SRenderData.PanelProperties, sizeof(RenderData::PanelPropertiesUniform));
    }

    void DrawPanel(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, float innerAlpha, float bevel);
    void DrawRectangle(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color = glm::vec4(1.0f));
    void DrawRectangle(const glm::vec3 &position, const glm::vec2 &size, const Reference<Texture> &texture, const glm::vec4 &color = glm::vec4(1.0f), float tiling = 1.0f);
    void EndScene();

private:
    void Flush();
    void Reset();

private:
    inline static Scope<CommandBuffer> SCommandBuffer {};
    struct RenderData {
        // Limits
        const size_t MaxComponents = 4096;
        const size_t MaxPanels = 4096;
        static constexpr uint32_t MaxTextureSlots = 32; // ToDo: RenderDevice::GetCapabilities().MaxTextureUnits

        // UI Panel
        Reference<PipelineState> PanelPipeline;
        Reference<Shader> PanelShader;
        Reference<Buffer> PanelVertexBuffer;
        Reference<Buffer> PanelIndexBuffer;

        const size_t PanelMaxIndices = MaxPanels * 6;
        const size_t PanelMaxVertices = MaxPanels * 4;
        vector<UIPanelComponent> PanelVertexBufferData;
        array<glm::vec4, 4> PanelVertexPositions;

        // UI Component
        Reference<PipelineState> ComponentPipeline;
        Reference<Shader> ComponentShader;
        Reference<Buffer> ComponentVertexBuffer;
        Reference<Buffer> ComponentIndexBuffer;

        const size_t ComponentMaxIndices = MaxComponents * 6;
        const size_t ComponentMaxVertices = MaxComponents * 4;
        vector<UIComponent> ComponentVertexBufferData;
        array<glm::vec4, 4> ComponentVertexPositions;

        // Textures
        uint32_t TextureSlotIndex = 1;
        array<Reference<Texture>, MaxTextureSlots> TextureSlots;
        Reference<Texture> WhiteTexture;

        // Transformation
        struct TransformUniform {
            glm::mat4 ProjectionMatrix {};
            glm::mat4 ViewMatrix {};
        } Transform;

        struct PanelPropertiesUniform {
            float Padding { 64.0f };
        } PanelProperties;


        Reference<Buffer> TransformUniformBuffer;
        Reference<Buffer> PanelPropertiesUniformBuffer;

    } SRenderData;


    /// Legacy
public:

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
        Instance().BeginScene();

        //Instance().DrawRectangle({ 0.0f, 0.0f, 0.3f }, { 1280.0f, 1204.0f }, mBackground);
        Instance().DrawRectangle({ 500.0f, 200.0f, 0.0f }, { 200.0f, 200.0f }, { 1.0f, 0.0f, 0.0f, 1.0f });
        Instance().DrawRectangle({ 650.0f, 300.0f, 0.2f }, { 200.0f, 200.0f }, { 1.0f, 0.0f, 1.0f, 0.8f });
        Instance().DrawRectangle({ 750.0f, 400.0f, 0.3f }, { 200.0f, 200.0f }, CheckerBoard);

        UIRenderer::Instance().Render();

        Instance().EndScene();
        Instance().mLayers.clear();
    }
    
    static void DrawBorder(float s, const Position &position, const Size &size, const Color &color) {
        glm::vec4 colorOld = { color.Red, color.Green, color.Blue, color.Alpha *1.0f };
        UIRenderer::Instance().DrawRectangle({ position.X, position.Y, 0 }, { size.Width, s });
        UIRenderer::Instance().DrawRectangle({ position.X, position.Y + size.Height - s, 0 }, { size.Width, s });
        UIRenderer::Instance().DrawRectangle({ position.X, position.Y + s, 0 }, { s, size.Height - 2 * s });
        UIRenderer::Instance().DrawRectangle({ position.X + size.Width - s , position.Y + s, 0 }, { s, size.Height - 2 * s });
    }

    static void Panel(const Position &position, const Size &size, const Color &color, float bevel, float innerAlpha) {
        mCurrentLayer->AddPanel(CreateScope<Ultra::Panel>(position, size, color, bevel, innerAlpha));
    }
    static void Rect(const Position &position, const Size &size, const Color &color, bool outline) {
        mCurrentLayer->AddElement(CreateScope<Rectangle>(position, size, color, outline));
    }
    static void Image(const Position &position, const Size &size, Reference<Texture>image) {
        mCurrentLayer->AddElement(CreateScope<Ultra::Image>(position, size, image));
    }
    static void Text(const Position &position, const string &text, const Color &color, Font *font) {
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

    inline static Reference<Texture> mBackground = nullptr;
    inline static Reference<Texture> CheckerBoard;
};

}

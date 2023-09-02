export module Ultra.Engine.UIRenderer;

export import Ultra.Engine.Renderer.Data;
import Ultra.Core;
import Ultra.Engine.Renderer.Buffer;
import Ultra.Engine.Renderer.CommandBuffer;
import Ultra.Engine.Renderer.PipelineState;
import Ultra.Engine.Renderer.Shader;
import Ultra.Engine.Renderer.Texture;
import Ultra.Engine.Renderer.Viewport;
import Ultra.Logger;
import Ultra.Math;

export import Ultra.Engine.Font;

// Helpers
namespace Ultra {

inline uint32_t DecodeUtf8(string_view::iterator &begin, string_view::iterator end) {
    unsigned char byte = *begin;
    uint32_t codepoint = 0;
    int additionalBytes = 0;

    // 1-Byte Character (ASCII)
    if (byte <= 0x7F) {
        codepoint = byte;
    // 2-Byte Character
    } else if ((byte & 0xE0) == 0xC0) {
        codepoint = byte & 0x1F;
        additionalBytes = 1;
    // 3-Byte Character
    } else if ((byte & 0xF0) == 0xE0) {
        codepoint = byte & 0x0F;
        additionalBytes = 2;
    // 4-Byte Character
    } else if ((byte & 0xF8) == 0xF0) {
        codepoint = byte & 0x07;
        additionalBytes = 3;
    // Invalid UTF-8 start byte
    } else {
        begin++;
        return 0xFFFD; // Unicode replacement character
    }

    begin++;

    while (additionalBytes > 0) {
        if (begin == end || (*begin & 0xC0) != 0x80) {
            // Premature end or invalid UTF-8 continuation byte
            return 0xFFFD; // Unicode replacement character
        }

        codepoint = (codepoint << 6) | (*begin & 0x3F);
        begin++;
        additionalBytes--;
    }

    return codepoint;
}

}

///
/// @brief UIRenderer: This is a simple ui renderer, nothing more, nothing less.
///
export namespace Ultra {

///
/// @brief Renderer
///

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

class UIRenderer: public SteadyObject {
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
    }
    ~UIRenderer() = default;

public:
    // Get the global instance to the logger
    static UIRenderer &Instance() {
        static UIRenderer instance;
        return instance;
    }

    static void Begin(const Scope<Viewport> &viewport) {
        static auto &initialize = UIRenderer::Instance();
        initialize.Test();

        mViewport = viewport.get();
        auto properties = viewport->GetProperties();
        auto size = Size { properties.Width, properties.Height };
    }
    static void End() {
        Instance().Reset();
    }
    static void Draw() {
        auto windowWidth = 1280.0f;
        auto windowHeight = 1024.0f;
        auto &renderer = Instance();
        //float fov = 45.0f;
        //float nearPlane = 0.1f;
        //float farPlane = 100.0f;
        // glm::perspective(glm::radians(fov), windowWidth / windowHeight, nearPlane, farPlane);

        renderer.SRenderData.Transform.ProjectionMatrix = glm::ortho(0.0f, windowWidth, windowHeight, 0.0f);
        renderer.SRenderData.Transform.ViewMatrix = glm::mat4(1.0f);

        renderer.SRenderData.TransformUniformBuffer->UpdateData(&renderer.SRenderData.Transform, sizeof(RenderData::TransformUniform));
        renderer.SRenderData.PanelPropertiesUniformBuffer->UpdateData(&renderer.SRenderData.PanelProperties, sizeof(RenderData::PanelPropertiesUniform));

        renderer.Reset();
    }
    static void Test() {
        //Instance().DrawRectangle({ 500.0f, 200.0f, 0.0f }, { 200.0f, 200.0f }, { 1.0f, 0.0f, 0.0f, 1.0f });
        //Instance().DrawRectangle({ 650.0f, 300.0f, 0.2f }, { 200.0f, 200.0f }, { 1.0f, 0.0f, 1.0f, 0.8f });
    }

    // Methods
    static void AddBorder(float s, const Position &position, const Size &size, const Color &color) {
        glm::vec4 colorOld = { color.Red, color.Green, color.Blue, color.Alpha * 1.0f };
        UIRenderer::Instance().DrawRectangle({ position.X, position.Y, 0 }, { size.Width, s }, colorOld);
        UIRenderer::Instance().DrawRectangle({ position.X, position.Y + size.Height - s, 0 }, { size.Width, s }, colorOld);
        UIRenderer::Instance().DrawRectangle({ position.X, position.Y + s, 0 }, { s, size.Height - 2 * s }, colorOld);
        UIRenderer::Instance().DrawRectangle({ position.X + size.Width - s , position.Y + s, 0 }, { s, size.Height - 2 * s }, colorOld);
    }
    static void AddPanel(const Position &position, const Size &size, const Color &color, float bevel, float innerAlpha) {
        const float pad = 64.0f;
        float x = position.X - pad;
        float y = position.Y - pad;
        float width = size.Width + 2.0f * pad;
        float height = size.Height + 2.0f * pad;

        UIRenderer::Instance().DrawPanel({ x, y, 0.0f }, { width, height }, { color.Red, color.Green, color.Blue, color.Alpha }, innerAlpha, bevel);
    }
    static void AddRectangle(const Position &position, const Size &size, const Color &color, bool outline) {
        UIRenderer::Instance().DrawRectangle({ position.X, position.Y, 0.0f }, { size.Width, size.Height }, { color.Red, color.Green, color.Blue, color.Alpha });
    }
    static void AddImage(const Position &position, const Size &size, const Reference<Texture> &image) {
        UIRenderer::Instance().DrawRectangle({ position.X, position.Y, 0.0f }, { size.Width, size.Height }, image, { 1.0f, 1.0f, 1.0f, 1.0f });
    }
    static void AddText(const Position &position, const string &text, const Color &color, Font *font) {
        auto x = std::floor(position.X);
        auto y = std::floor(position.Y);
        string_view view = text;
        auto begin = view.begin();
        auto end = view.end();
        uint32_t lastGlyph = 0;
        while (begin != end) {
            uint32_t codepoint = DecodeUtf8(begin, end);
            auto *glyph = font->GetGlyph(codepoint);
            if (!glyph) {
                lastGlyph = 0;
                continue;
            }
            if (lastGlyph) x += font->GetKerning(lastGlyph, glyph->UniqueID);
            lastGlyph = glyph->UniqueID;
            UIRenderer::Instance().DrawRectangle({ x + glyph->X, y + glyph->Y, 0.0f }, { glyph->Width, glyph->Height }, glyph->Texture, { color.Red, color.Green, color.Blue, color.Alpha });
            x += glyph->Advance;
        }
    }
    static void Clip(const Position &position, const Size &size) {
        ClipRect::PushCombined(position, size, Instance().mViewport);
    }
    static void Unclip() {
        ClipRect::Pop();
        Instance().Reset();
    }
    void Reset();

private: // Internal Methods
    void DrawPanel(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, float innerAlpha, float bevel);
    void DrawRectangle(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color = glm::vec4(1.0f));
    void DrawRectangle(const glm::vec3 &position, const glm::vec2 &size, const Reference<Texture> &texture, const glm::vec4 &color = glm::vec4(1.0f), float tiling = 1.0f);
    void Flush();

private:
    inline static Scope<CommandBuffer> SCommandBuffer {};
    inline static Scope<PipelineState> mPipelineState {};
    inline static Viewport *mViewport {};

    // Properties
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
};

}

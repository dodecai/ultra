module;

#define MSDF_ATLAS_PUBLIC
#include <msdfgen/msdfgen.h>
#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include <msdf-atlas-gen/FontGeometry.h>
#include <msdf-atlas-gen/GlyphGeometry.h>

export module Ultra.UI.Renderer;

export import Ultra.Renderer.Data;
export import Ultra.Renderer.Font;

import Ultra.Core;
import Ultra.Renderer.Buffer;
import Ultra.Renderer.CommandBuffer;
import Ultra.Renderer.PipelineState;
import Ultra.Renderer.Shader;
import Ultra.Renderer.Texture;
import Ultra.Renderer.Viewport;
import Ultra.Logger;
import Ultra.Math;


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

struct UITextComponent {
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TextureCoordinate;
};

struct TextParams {
    glm::vec4 Color { 1.0f };
    float Kerning = 0.0f;
    float LineSpacing = 0.0f;
};

class UIRenderer: public SteadyObject {
    // Default
    UIRenderer() {
        // Panel
        {
            SRenderData.PanelShader = Shader::Create("Assets/Shaders/UI/UI.glsl");
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
            SRenderData.ComponentShader = Shader::Create("Assets/Shaders/UI/UIElement.glsl");
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

            // Textures
            uint32_t whiteTextureData = 0xffffffff;
            SRenderData.WhiteTexture = Texture::Create(TextureProperties(), &whiteTextureData, sizeof(uint32_t));

            int32_t samplers[SRenderData.MaxTextureSlots];
            for (uint32_t i = 0; i < SRenderData.MaxTextureSlots; i++) samplers[i] = i;
            SRenderData.ComponentShader->Bind();
            SRenderData.ComponentShader->UpdateUniformBuffer("uTextures", (void *)samplers, SRenderData.MaxTextureSlots);
            SRenderData.TextureSlots[0] = SRenderData.WhiteTexture;
        }

        // Text
        {
            SRenderData.TextShader = Shader::Create("Assets/Shaders/UI/UIText2.glsl");
            PipelineProperties textProperties;
            textProperties.BlendMode = BlendMode::Alpha;
            textProperties.DepthTest = true;
            textProperties.Layout = {
                { ShaderDataType::Float3, "aPosition" },
                { ShaderDataType::Float4, "aColor" },
                { ShaderDataType::Float2, "aTextureCoordinate" },
                { ShaderDataType::Float, "aTextureIndex" },
                { ShaderDataType::Float, "aTilingFactor" },
            };
            SRenderData.TextPipeline = PipelineState::Create(textProperties);

            SRenderData.TextVertexBufferData.reserve(SRenderData.TextMaxVertices);
            SRenderData.TextVertexBuffer = Buffer::Create(BufferType::Vertex, nullptr, SRenderData.TextMaxVertices * sizeof(UIComponent));
            SRenderData.TextVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
            SRenderData.TextVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
            SRenderData.TextVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
            SRenderData.TextVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

            uint32_t offset = 0;
            uint32_t *indices = new uint32_t[SRenderData.TextMaxIndices];
            for (uint32_t i = 0; i < SRenderData.TextMaxIndices; i += 6) {
                indices[i + 0] = offset + 0;
                indices[i + 1] = offset + 1;
                indices[i + 2] = offset + 2;

                indices[i + 3] = offset + 2;
                indices[i + 4] = offset + 3;
                indices[i + 5] = offset + 0;
                offset += 4;
            }
            SRenderData.TextIndexBuffer = Buffer::Create(BufferType::Index, indices, SRenderData.TextMaxVertices * sizeof(uint32_t));
            delete[] indices;

            int32_t samplers[SRenderData.MaxTextureSlots];
            for (uint32_t i = 0; i < SRenderData.MaxTextureSlots; i++) samplers[i] = i;
            SRenderData.TextShader->Bind();
            SRenderData.TextShader->UpdateUniformBuffer("uFontAtlas", (void *)samplers, SRenderData.MaxTextureSlots);
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
    static void AddLine(const Position &start, const Position &end, const Color &color) {
        UIRenderer::Instance().DrawLine({ start.X, start.Y, 0.0f }, { end.X, end.Y, 0.0f }, { color.Red, color.Green, color.Blue, color.Alpha });
    }
    static void AddRectangle(const Position &position, const Size &size, const Color &color) {
        UIRenderer::Instance().DrawRectangle({ position.X, position.Y, 0.0f }, { size.Width, size.Height }, { color.Red, color.Green, color.Blue, color.Alpha });
    }
    static void AddImage(const Position &position, const Size &size, const Reference<Texture> &image) {
        UIRenderer::Instance().DrawRectangle({ position.X, position.Y, 0.0f }, { size.Width, size.Height }, image, { 1.0f, 1.0f, 1.0f, 1.0f });
    }
    static void AddText(const Position &position, const string &text, const Color &color, Font *font) {
    #define OLD_TEXT_RENDERING
    #ifdef OLD_TEXT_RENDERING
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
            //UIRenderer::Instance().DrawText({ x + glyph->X, y + glyph->Y, 0.0f }, { glyph->Width, glyph->Height }, glyph->Texture, {color.Red, color.Green, color.Blue, color.Alpha});
            x += glyph->Advance;
        }
    #else
        if (Instance().SRenderData.ComponentVertexBufferData.size() * 6 >= Instance().SRenderData.ComponentMaxIndices) Instance().Reset();

        TextParams textParams {};
        const auto &geometry = font->GetMSDFData()->FontGeometry;
        const auto &metrics = geometry.getMetrics();
        
        double x = position.X;
        double y = position.Y;
        double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);

        //const float spaceGlyphAdvance = geometry.getGlyph(' ')->getAdvance();
        float spaceGlyphAdvance = 2.0f;

        auto fontAtlas = font->GetTexture();

        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < Instance().SRenderData.TextTextureSlotIndex; i++) {
            if (*Instance().SRenderData.TextTextureSlots[i].get() == *fontAtlas.get()) {
                textureIndex = (float)i;
                break;
            }
        }
        if (textureIndex == 0.0f) {
            if (Instance().SRenderData.TextTextureSlotIndex >= Instance().SRenderData.MaxTextureSlots) Instance().Reset();

            textureIndex = (float)Instance().SRenderData.TextTextureSlotIndex;
            Instance().SRenderData.TextTextureSlots[Instance().SRenderData.TextTextureSlotIndex] = fontAtlas;
            Instance().SRenderData.TextTextureSlotIndex++;
        }

        for (size_t i = 0; i < text.size(); i++) {
            char character = text[i];
            if (character == '\r') continue;
            if (character == ' ') {
                float advance = spaceGlyphAdvance;
                if (i < text.size() - 1) {
                    char nextCharacter = text[i + 1];
                    double dAdvance;
                    geometry.getAdvance(dAdvance, character, nextCharacter);
                    advance = (float)dAdvance;
                }

                x += fsScale * advance + textParams.Kerning;
                continue;
            }
            if (character == '\n') {
                x = 0;
                y -= fsScale * metrics.lineHeight + textParams.LineSpacing;
                continue;
            }
            if (character == '\t') {
                x += 4.0f * (fsScale * spaceGlyphAdvance + textParams.Kerning);
                continue;
            }

            auto glyph = geometry.getGlyph(character);
            if (!glyph) glyph = geometry.getGlyph('?');
            if (!glyph) return;

            double al, ab, ar, at;
            glyph->getQuadAtlasBounds(al, ab, ar, at);
            glm::vec2 texCoordMin((float)al, (float)ab);
            glm::vec2 texCoordMax((float)ar, (float)at);

            double pl, pb, pr, pt;
            glyph->getQuadPlaneBounds(pl, pb, pr, pt);
            glm::vec2 quadMin((float)pl, (float)pb);
            glm::vec2 quadMax((float)pr, (float)pt);

            quadMin *= fsScale, quadMax *= fsScale;
            quadMin += glm::vec2(x, y);
            quadMax += glm::vec2(x, y);

            float texelWidth = 1.0f / fontAtlas->GetProperties().Width;
            float texelHeight = 1.0f / fontAtlas->GetProperties().Height;
            texCoordMin *= glm::vec2(texelWidth, texelHeight);
            texCoordMax *= glm::vec2(texelWidth, texelHeight);


            glm::vec3 size {texelWidth *24.0f, texelHeight*24.0f, 0.0f };
            glm::vec3 nativePosition = { x, y, 0.0f };
            glm::vec2 center = nativePosition + glm::vec3(size.x, size.y, 0.0f) * 0.5f;
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(center, 0.0f)) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });
            glm::vec4 nativeColor = { color.Red, color.Green, color.Blue, color.Alpha };

            Instance().SRenderData.TextVertexBufferData.emplace_back((transform * glm::vec4(quadMin, 0.0f, 1.0f)), nativeColor, texCoordMin);
            Instance().SRenderData.TextVertexBufferData.emplace_back((transform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f)), nativeColor, glm::vec2(texCoordMin.x, texCoordMax.y));
            Instance().SRenderData.TextVertexBufferData.emplace_back((transform * glm::vec4(quadMax, 0.0f, 1.0f)), nativeColor, texCoordMax);
            Instance().SRenderData.TextVertexBufferData.emplace_back((transform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f)), nativeColor, glm::vec2(texCoordMax.x, texCoordMin.y));
        }


        //UIRenderer::Instance().DrawText({ x + glyph->X, y + glyph->Y, 0.0f }, { glyph->Width, glyph->Height }, font->GetTexture(), { color.Red, color.Green, color.Blue, color.Alpha });
    #endif
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
    void DrawLine(const glm::vec3 &start, const glm::vec3 &end, const glm::vec4 &color);
    void DrawRectangle(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color = glm::vec4(1.0f));
    void DrawRectangle(const glm::vec3 &position, const glm::vec2 &size, const Reference<Texture> &texture, const glm::vec4 &color = glm::vec4(1.0f), float tiling = 1.0f);
    void DrawText(const glm::vec3 &position, const glm::vec2 &size, const Reference<Texture> &texture, const glm::vec4 &color = glm::vec4(1.0f), float tiling = 1.0f);
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
        const size_t MaxTexts = 4096;
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

        // Text
        Reference<PipelineState> TextPipeline;
        Reference<Shader> TextShader;
        Reference<Buffer> TextVertexBuffer;
        Reference<Buffer> TextIndexBuffer;

        const size_t TextMaxIndices = MaxTexts * 6;
        const size_t TextMaxVertices = MaxTexts * 4;
        vector<UITextComponent> TextVertexBufferData;
        array<glm::vec4, 4> TextVertexPositions;

        uint32_t TextTextureSlotIndex = 0;
        array<Reference<Texture>, MaxTextureSlots> TextTextureSlots;

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

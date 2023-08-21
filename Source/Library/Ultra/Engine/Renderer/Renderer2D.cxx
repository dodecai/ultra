module;

#include <glm/gtc/matrix_transform.hpp>

module Ultra.Engine.Renderer2D;

import Ultra.Engine.Renderer.Buffer;
import Ultra.Engine.Renderer.PipelineState;
import Ultra.Engine.Renderer.Shader;
import Ultra.Engine.Renderer.Texture;

namespace Ultra {

Scope<CommandBuffer> Renderer2D::sCommandBuffer = nullptr;

struct RendererData {
    // General
    bool DepthTest = true;
    Renderer2D::Statistics Stats;

    // Circles
    static constexpr size_t MaxCircles = 2000;
    static constexpr size_t MaxCircleVertices = MaxCircles * 4;
    static constexpr size_t MaxCircleIndices = MaxCircles * 6;

    vector<CircleComponent> CircleVertexBufferData;
    array<glm::vec4, 4> CircleVertexPositions;

    Reference<PipelineState> CirclePipeline;
    Reference<Shader> CircleShader;
    Reference<Buffer> CircleVertexBuffer;
    Reference<Buffer> CircleIndexBuffer;

    // Lines
    static constexpr size_t MaxLines = 12000;
    static constexpr size_t MaxLineVertices = MaxLines * 4;
    static constexpr size_t MaxLineIndices = MaxLines * 6;

    vector<LineComponent> LineVertexBufferData;
    array<glm::vec4, 4> LineVertexPositions;

    Reference<PipelineState> LinePipeline;
    Reference<Shader> LineShader;
    Reference<Buffer> LineVertexBuffer;
    Reference<Buffer> LineIndexBuffer;

    // Quads
    static constexpr uint32_t MaxQuads = 21000;
    static constexpr uint32_t MaxQuadVertices = MaxQuads * 4;
    static constexpr uint32_t MaxQuadIndices = MaxQuads * 6;

    vector<QuadComponent> QuadVertexBufferData;
    array<glm::vec4, 4> QuadVertexPositions;

    Reference<PipelineState> QuadPipeline;
    Reference<Buffer> QVertexBuffer;
    Reference<Buffer> QIndexBuffer;

    // Textures
    static constexpr uint32_t MaxTextureSlots = 16; // ToDo: RenderDevice::GetCapabilities().MaxTextureUnits

    uint32_t TextureSlotIndex = 1; // 0 = White
    array<Reference<Texture>, MaxTextureSlots> TextureSlots;

    Reference<Shader> TextureShader;
    Reference<Texture> WhiteTexture;

    // Uniforms
    struct CameraData {
        glm::mat4 ViewProjectionMatrix = {};
        glm::mat4 Projection = {};
        glm::mat4 View = {};
        float NearClip = {};
        float FarClip = {};
    };
    CameraData CameraBuffer;
    Reference<Buffer> CameraUniformBuffer;
};

static RendererData sData;

// Default
void Renderer2D::Load() {
    // Circles
    {
        sData.CircleShader = Shader::Create("Assets/Shaders/Circle.glsl");
        PipelineProperties pipelineProperties;
        pipelineProperties.Layout = {
            { ShaderDataType::Float3, "aWorldPosition" },
            { ShaderDataType::Float3, "aLocalPosition" },
            { ShaderDataType::Float4, "aColor"         },
            { ShaderDataType::Float,  "aThickness"     },
            { ShaderDataType::Float,  "aFade"          },
        };
        sData.CirclePipeline = PipelineState::Create(pipelineProperties);

        sData.CircleVertexBufferData.reserve(sData.MaxCircleVertices);
        sData.CircleVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        sData.CircleVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
        sData.CircleVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
        sData.CircleVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

        sData.CircleVertexBuffer = Buffer::Create(BufferType::Vertex, nullptr, sData.MaxCircleVertices * sizeof(CircleComponent));

        uint32_t offset = 0;
        uint32_t *circleIndicies = new uint32_t[sData.MaxCircleIndices];
        for (uint32_t i = 0; i < sData.MaxCircleIndices; i += 6) {
            circleIndicies[i + 0] = offset + 0;
            circleIndicies[i + 1] = offset + 1;
            circleIndicies[i + 2] = offset + 2;

            circleIndicies[i + 3] = offset + 2;
            circleIndicies[i + 4] = offset + 3;
            circleIndicies[i + 5] = offset + 0;
            offset += 4;
        }

        sData.CircleIndexBuffer = Buffer::Create(BufferType::Index, circleIndicies, sData.MaxCircleIndices * sizeof(uint32_t));
        delete[] circleIndicies;
    }

    // Lines
    {
        sData.LineShader = Shader::Create("Assets/Shaders/Line.glsl");

        PipelineProperties pipelineProperties;
        pipelineProperties.Layout = {
            { ShaderDataType::Float3, "aPosition" },
            { ShaderDataType::Float4, "aColor" }
        };
        sData.LinePipeline = PipelineState::Create(pipelineProperties);

        sData.LineVertexBufferData.reserve(sData.MaxLineVertices);
        sData.LineVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        sData.LineVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
        sData.LineVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
        sData.LineVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

        sData.LineVertexBuffer = Buffer::Create(BufferType::Vertex, nullptr, sData.MaxLineVertices * sizeof(LineComponent));

        uint32_t *lineIndices = new uint32_t[sData.MaxLineIndices];
        for (uint32_t i = 0; i < sData.MaxLineIndices; i++) {
            lineIndices[i] = i;
        }

        sData.LineIndexBuffer = Buffer::Create(BufferType::Index, lineIndices, sData.MaxLineIndices * sizeof(uint32_t));
        delete[] lineIndices;
    }

    // Quads
    {
        PipelineProperties pipelineProperties;
        pipelineProperties.Layout = {
            { ShaderDataType::Float3, "aPosition" },
            { ShaderDataType::Float4, "aColor" },
            { ShaderDataType::Float2, "aTexCoord" },
            { ShaderDataType::Float, "aTexIndex" },
            { ShaderDataType::Float, "aTilingFactor" },
        };
        sData.QuadPipeline = PipelineState::Create(pipelineProperties);

        sData.QuadVertexBufferData.reserve(sData.MaxQuadVertices);

        sData.QVertexBuffer = Buffer::Create(BufferType::Vertex, nullptr, sData.MaxQuadVertices * sizeof(QuadComponent));
        sData.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        sData.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
        sData.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
        sData.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

        uint32_t offset = 0;
        uint32_t *quadIndicies = new uint32_t[sData.MaxQuadIndices];
        for (uint32_t i = 0; i < sData.MaxQuadIndices; i += 6) {
            quadIndicies[i + 0] = offset + 0;
            quadIndicies[i + 1] = offset + 1;
            quadIndicies[i + 2] = offset + 2;

            quadIndicies[i + 3] = offset + 2;
            quadIndicies[i + 4] = offset + 3;
            quadIndicies[i + 5] = offset + 0;
            offset += 4;
        }

        sData.QIndexBuffer = Buffer::Create(BufferType::Index, quadIndicies, sData.MaxQuadIndices * sizeof(uint32_t));
        delete[] quadIndicies;
    }

    // Textures
    {
        uint32_t whiteTextureData = 0xffffffff;
        sData.WhiteTexture = Texture::Create(TextureProperties(), &whiteTextureData, sizeof(uint32_t));

        int32_t samplers[sData.MaxTextureSlots];
        for (uint32_t i = 0; i < sData.MaxTextureSlots; i++) samplers[i] = i;

        sData.TextureShader = Shader::Create("./Assets/Shaders/Texture.glsl");
        sData.TextureShader->Bind();
        sData.TextureShader->UpdateUniformBuffer("uTextures", (void *)samplers, sData.MaxTextureSlots);

        // Set all texture slots to 0
        sData.TextureSlots[0] = sData.WhiteTexture;

        sData.CameraUniformBuffer = Buffer::Create(BufferType::Uniform, nullptr, sizeof(RendererData::CameraData));
        sData.CameraUniformBuffer->Bind(0);
    }


    sCommandBuffer = CommandBuffer::Create();
}

void Renderer2D::Dispose() {
    sData.CircleVertexBufferData.clear();
    sData.CirclePipeline.reset();
    sData.CircleVertexBuffer.reset();
    sData.CircleIndexBuffer.reset();

    sData.LineVertexBufferData.clear();
    sData.LinePipeline.reset();
    sData.LineVertexBuffer.reset();
    sData.LineIndexBuffer.reset();
    sData.LineShader.reset();

    sData.QuadVertexBufferData.clear();
    sData.QuadPipeline.reset();
    sData.QVertexBuffer.reset();
    sData.QIndexBuffer.reset();

    sData.TextureShader.reset();
    sData.WhiteTexture.reset();
    for (auto &texture : sData.TextureSlots) {
        texture.reset();
    }
}


void Renderer2D::StartScene(const Camera &camera) {
    sData.DepthTest = true;

    sData.CameraBuffer.ViewProjectionMatrix = camera.GetProjection();
    sData.CameraUniformBuffer->UpdateData(&sData.CameraBuffer, sizeof(RendererData::CameraData));

    NextBatch();
}

void Renderer2D::StartScene(const DesignerCamera &camera) {
    sData.DepthTest = true;

    sData.CameraBuffer.ViewProjectionMatrix = camera.GetViewProjection();
    sData.CameraUniformBuffer->UpdateData(&sData.CameraBuffer, sizeof(RendererData::CameraData));

    NextBatch();
}

void Renderer2D::StartScene(const PerspectiveCamera &camera) {
    sData.DepthTest = true;

    sData.CameraBuffer.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    sData.CameraUniformBuffer->UpdateData(&sData.CameraBuffer, sizeof(RendererData::CameraData));

    NextBatch();
}

void Renderer2D::StartScene(const OrthographicCamera &camera) {
    sData.DepthTest = true;

    sData.CameraBuffer.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    sData.CameraUniformBuffer->UpdateData(&sData.CameraBuffer, sizeof(RendererData::CameraData));

    NextBatch();
}

void Renderer2D::FinishScene() {
    // Is the buffer empty?
    if (sData.CircleVertexBufferData.size() && sData.QuadVertexBufferData.size() && sData.LineVertexBufferData.size() == 0) return;
    Flush();
}

void Renderer2D::Flush() {
    if (sData.CircleVertexBufferData.size()) {
        auto dataSize = sizeof_vector(sData.CircleVertexBufferData);
        sData.CircleVertexBuffer->UpdateData(sData.CircleVertexBufferData.data(), dataSize);

        sData.CircleShader->Bind();
        //sData.CircleShader->UpdateUniform("uViewProjection", sData.ViewProjectionMatrix);

        sData.CircleVertexBuffer->Bind();
        sData.CirclePipeline->Bind();
        sData.QIndexBuffer->Bind();
        sCommandBuffer->DrawIndexed({ sData.QuadVertexBufferData.size() * 6 }, PrimitiveType::Circle, sData.DepthTest);
        sData.Stats.DrawCalls++;
    }

    if (sData.LineVertexBufferData.size()) {
        auto dataSize = sizeof_vector(sData.LineVertexBufferData);
        sData.LineVertexBuffer->UpdateData(sData.LineVertexBufferData.data(), dataSize);

        sData.LineVertexBuffer->Bind();
        sData.LineShader->Bind();
        sData.LinePipeline->Bind();
        sData.LineIndexBuffer->Bind();
        sCommandBuffer->DrawIndexed({ sData.LineVertexBufferData.size() }, PrimitiveType::Line, sData.DepthTest);
        sData.Stats.DrawCalls++;
    }

    if (sData.QuadVertexBufferData.size()) {
        auto dataSize = sizeof_vector(sData.QuadVertexBufferData);
        sData.QVertexBuffer->UpdateData(sData.QuadVertexBufferData.data(), dataSize);

        sData.TextureShader->Bind();
        //sData.TextureShader->UpdateUniform("uViewProjection", sData.ViewProjectionMatrix);

        for (uint32_t i = 0; i < sData.TextureSlotIndex; i++) {
            sData.TextureSlots[i]->Bind(i);
        }

        sData.QVertexBuffer->Bind();
        sData.QuadPipeline->Bind();
        sData.QIndexBuffer->Bind();
        sCommandBuffer->DrawIndexed({ sData.QuadVertexBufferData.size() * 6 }, PrimitiveType::Triangle, sData.DepthTest);
        sData.Stats.DrawCalls++;
    }
}

void Renderer2D::NextBatch() {
    sData.CircleVertexBufferData.clear();
    sData.LineVertexBufferData.clear();
    sData.QuadVertexBufferData.clear();

    sData.TextureSlotIndex = 1;
}

void Renderer2D::FlushAndReset(PrimitiveType primitive) {
    switch (primitive) {
        case PrimitiveType::Circle: {
            Flush();
            sData.CircleVertexBufferData.clear();
            break;
        }

        case PrimitiveType::Line: {
            Flush();
            sData.LineVertexBufferData.clear();
            break;
        }

        case PrimitiveType::Triangle: {
            Flush();
            sData.QuadVertexBufferData.clear();

            sData.TextureSlotIndex = 1;
            break;
        }

        default: {
            break;
        }
    }
}


// Primitives
void Renderer2D::DrawCircle(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, float thickness, float fade) {
    DrawCircle({ position.x, position.y, 0.0f }, size, color, thickness, fade);
}

void Renderer2D::DrawCircle(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, float thickness, float fade) {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
    DrawCircle(transform, color, thickness, fade);
}

void Renderer2D::DrawCircle(const glm::mat4 &transform, const glm::vec4 &color, float thickness, float fade) {
    if (sData.CircleVertexBufferData.size() * 6 >= RendererData::MaxCircleIndices) FlushAndReset();

    for (size_t i = 0; i < sData.CircleVertexPositions.size(); i++) {
        sData.CircleVertexBufferData.emplace_back((transform * sData.CircleVertexPositions[i]), (sData.CircleVertexPositions[i] * 2.0f), color, thickness, fade);
    }

    sData.Stats.CircleCount++;
}


void Renderer2D::DrawLine(const glm::vec2 &start, const glm::vec2 &end, const glm::vec4 &color) {
    DrawLine({ start.x, start.y, 0.0f }, { end.x, end.y, 0.0f }, color);
}

void Renderer2D::DrawLine(const glm::vec3 &start, const glm::vec3 &end, const glm::vec4 &color) {
    if (sData.LineVertexBufferData.size() >= RendererData::MaxLineIndices) FlushAndReset(PrimitiveType::Line);

    sData.LineVertexBufferData.emplace_back(start, color);
    sData.LineVertexBufferData.emplace_back(end, color);

    sData.Stats.LineCount++;
}


void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) {
    DrawQuad({ position.x, position.y, 0.0f }, size, color);
}

void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color) {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawQuad(transform, color);
}

void Renderer2D::DrawQuad(const glm::mat4 &transform, const glm::vec4 &color) {
    const float textureIndex = 0.0f; // White Texture
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    const float tilingFactor = 1.0f;

    if (sData.QuadVertexBufferData.size() * 6 >= RendererData::MaxQuadIndices) FlushAndReset();

    for (size_t i = 0; i < sData.QuadVertexPositions.size(); i++) {
        sData.QuadVertexBufferData.emplace_back((transform * sData.QuadVertexPositions[i]), color, textureCoords[i], textureIndex, tilingFactor);
    }

    sData.Stats.Triangles += 2;
    sData.Stats.QuadCount++;
}

void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Reference<Texture> &texture, const float tilingFactor, const glm::vec4 &color) {
    DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, color);
}

void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Reference<Texture> &texture, const float tilingFactor, const glm::vec4 &color) {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawQuad(transform, texture, tilingFactor, color);
}

void Renderer2D::DrawQuad(const glm::mat4 &transform, const Reference<Texture> &texture, const float tilingFactor, const glm::vec4 &color) {
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

    if (sData.QuadVertexBufferData.size() * 6 >= RendererData::MaxQuadIndices) FlushAndReset();

    float textureIndex = 0.0f;
    for (uint32_t i = 1; i < sData.TextureSlotIndex; i++) {
        if (*sData.TextureSlots[i].get() == *texture.get()) {
            textureIndex = (float)i;
            break;
        }
    }

    if (textureIndex == 0.0f) {
        if (sData.TextureSlotIndex >= RendererData::MaxTextureSlots) FlushAndReset();

        textureIndex = (float)sData.TextureSlotIndex;
        sData.TextureSlots[sData.TextureSlotIndex] = texture;
        sData.TextureSlotIndex++;
    }

    for (size_t i = 0; i < sData.QuadVertexPositions.size(); i++) {
        sData.QuadVertexBufferData.emplace_back((transform * sData.QuadVertexPositions[i]), color, textureCoords[i], textureIndex, tilingFactor);
    }

    sData.Stats.Triangles += 2;
    sData.Stats.QuadCount++;
}


void Renderer2D::DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, const float rotation, const glm::vec4 &color) {
    DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, const float rotation, const glm::vec4 &color) {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawRotatedQuad(transform, rotation, color);
}

void Renderer2D::DrawRotatedQuad(const glm::mat4 &transform, const float rotation, const glm::vec4 &color) {
    // ToDo: rotation missing
    if (rotation > 0) {}
    const float textureIndex = 0.0f; // White Texture
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    const float tilingFactor = 1.0f;

    if (sData.QuadVertexBufferData.size() * 6 >= RendererData::MaxQuadIndices) FlushAndReset();

    for (size_t i = 0; i < sData.QuadVertexPositions.size(); i++) {
        sData.QuadVertexBufferData.emplace_back((transform * sData.QuadVertexPositions[i]), color, textureCoords[i], textureIndex, tilingFactor);
    }

    sData.Stats.Triangles += 2;
    sData.Stats.QuadCount++;
}

void Renderer2D::DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, const float rotation, const Reference<Texture> &texture, const float tilingFactor, const glm::vec4 &color) {
    DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, const float rotation, const Reference<Texture> &texture, const float tilingFactor, const glm::vec4 &color) {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawRotatedQuad(transform, rotation, texture, tilingFactor, color);
}

void Renderer2D::DrawRotatedQuad(const glm::mat4 &transform, const float rotation, const Reference<Texture> &texture, const float tilingFactor, const glm::vec4 &color) {
    // ToDo: rotation missing
    if (rotation > 0) {}
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

    if (sData.QuadVertexBufferData.size() * 6 >= RendererData::MaxQuadIndices) FlushAndReset();

    float textureIndex = 0.0f;
    for (uint32_t i = 1; i < sData.TextureSlotIndex; i++) {
        if (*sData.TextureSlots[i].get() == *texture.get()) {
            textureIndex = (float)i;
            break;
        }
    }

    if (textureIndex == 0.0f) {
        if (sData.TextureSlotIndex >= RendererData::MaxTextureSlots) FlushAndReset();

        textureIndex = (float)sData.TextureSlotIndex;
        sData.TextureSlots[sData.TextureSlotIndex] = texture;
        sData.TextureSlotIndex++;
    }


    for (size_t i = 0; i < sData.QuadVertexPositions.size(); i++) {
        sData.QuadVertexBufferData.emplace_back((transform * sData.QuadVertexPositions[i]), color, textureCoords[i], textureIndex, tilingFactor);
    }

    sData.Stats.Triangles += 2;
    sData.Stats.QuadCount++;
}


void Renderer2D::DrawRect(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) {
    DrawRect({ position.x, position.y, 0.0f }, size, color);
}

void Renderer2D::DrawRect(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color) {
    glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
    glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
    glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
    glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

    DrawLine(p0, p1, color);
    DrawLine(p1, p2, color);
    DrawLine(p2, p3, color);
    DrawLine(p3, p0, color);
}

void Renderer2D::DrawRect(const glm::mat4 &transform, const glm::vec4 &color) {
    glm::vec3 lineVertices[4];
    for (size_t i = 0; i < 4; i++) lineVertices[i] = transform * sData.QuadVertexPositions[i];

    DrawLine(lineVertices[0], lineVertices[1], color);
    DrawLine(lineVertices[1], lineVertices[2], color);
    DrawLine(lineVertices[2], lineVertices[3], color);
    DrawLine(lineVertices[3], lineVertices[0], color);
}


// Statistics
void Renderer2D::ResetStatistics() {
    memset(&sData.Stats, 0, sizeof(Statistics));
}

Renderer2D::Statistics Renderer2D::GetStatistics() {
    return sData.Stats;
}

}

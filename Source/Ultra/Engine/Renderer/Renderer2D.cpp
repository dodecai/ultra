#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Ultra {

struct QuadVertex {
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TextureCoordinate;
    float TextureIndex;
    float TilingFactor;
};

struct RendererData {
    static const uint32_t MaxQuads = 5000;
    static const uint32_t MaxVertices = MaxQuads * 4;
    static const uint32_t MaxIndices = MaxQuads * 6;
    static const uint32_t MaxTextureSlots = 32; // ToDo: Render Capabilities

    Reference<VertexArray> QVertexArray;
    Reference<VertexBuffer> QVertexBuffer;
    Reference<Shader> TextureShader;
    Reference<Texture2D> WhiteTexture;

    uint32_t QIndexCount = 0;
    QuadVertex *QVertexBufferBase = nullptr;
    QuadVertex *QVertexBufferPtr = nullptr;

    std::array<Reference<Texture2D>, MaxTextureSlots> TextureSlots;
    uint32_t TextureSlotIndex = 1; // 0 = White

    glm::vec4 QVertexPositions[4];

    Renderer2D::Statistics Stats;
};

static RendererData sData;

void Renderer2D::Load() {
    sData.QVertexArray = VertexArray::Create();

    sData.QVertexBuffer = VertexBuffer::Create(sData.MaxVertices * sizeof(QuadVertex));
    sData.QVertexBuffer->SetLayout({
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float4, "a_Color" },
        { ShaderDataType::Float2, "a_TexCoordinates" },
        { ShaderDataType::Float, "a_TextureIndex" },
        { ShaderDataType::Float, "a_TilingFactor" },
    });
    sData.QVertexArray->AddVertexBuffer(sData.QVertexBuffer);

    sData.QVertexBufferBase = new QuadVertex[sData.MaxVertices];

    uint32_t *quadIndicies = new uint32_t[sData.MaxIndices];

    uint32_t offset = 0;
    for (uint32_t i = 0; i < sData.MaxIndices; i += 6) {
        quadIndicies[i + 0] = offset + 0;
        quadIndicies[i + 1] = offset + 1;
        quadIndicies[i + 2] = offset + 2;

        quadIndicies[i + 3] = offset + 2;
        quadIndicies[i + 4] = offset + 3;
        quadIndicies[i + 5] = offset + 0;      
        offset += 4;
    }

    Reference<IndexBuffer> QIndexBuffer = IndexBuffer::Create(quadIndicies, sData.MaxIndices);
    sData.QVertexArray->SetIndexBuffer(QIndexBuffer);
    delete[] quadIndicies;

    sData.WhiteTexture = Texture2D::Create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    sData.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

    int32_t samplers[sData.MaxTextureSlots];
    for (uint32_t i = 0; i < sData.MaxTextureSlots; i++) samplers[i] = i;

    sData.TextureShader = Shader::Create("./Assets/Shaders/Texture.glsl");
    sData.TextureShader->Bind();
    sData.TextureShader->SetIntArray("u_Textures", samplers, sData.MaxTextureSlots);

    // Set all texture slots to 0
    sData.TextureSlots[0] = sData.WhiteTexture;

    sData.QVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    sData.QVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
    sData.QVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
    sData.QVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
}

void Renderer2D::Unload() {
    delete[] sData.QVertexBufferBase;
}

void Renderer2D::BeginScene(const OrthographicCamera &camera) {
    sData.TextureShader->Bind();
    sData.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

    sData.QIndexCount = 0;
    sData.QVertexBufferPtr = sData.QVertexBufferBase;

    sData.TextureSlotIndex = 1;
}

void Renderer2D::Flush() {
    if (sData.QIndexCount == 0) return; // Empty Buffer

    for (uint32_t i = 0; i < sData.TextureSlotIndex; i++) {
        sData.TextureSlots[i]->Bind(i);
    }

    RenderCommand::DrawIndexed(sData.QVertexArray, sData.QIndexCount);
    sData.Stats.DrawCalls++;
}

void Renderer2D::EndScene() {
    uint32_t dataSize = (uint32_t)((uint8_t*)sData.QVertexBufferPtr - (uint8_t*)sData.QVertexBufferBase);
    sData.QVertexBuffer->SetData(sData.QVertexBufferBase, dataSize);
    
    Flush();
}

void Renderer2D::ResetStatistics() {
    memset(&sData.Stats, 0, sizeof(Statistics));
}

Renderer2D::Statistics Renderer2D::GetStatistics() {
    return sData.Stats;
}

void Renderer2D::FlushAndReset() {
    EndScene();

    sData.QIndexCount = 0;
    sData.QVertexBufferPtr = sData.QVertexBufferBase;

    sData.TextureSlotIndex = 1;
}



void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) {
    DrawQuad({ position.x, position.y, 0.0f }, size, color);
}

void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color) {

    constexpr size_t quadVertexCount = 4;
    const float textureIndex = 0.0f; // White Texture
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    const float tilingFactor = 1.0f;

    if (sData.QIndexCount >= RendererData::MaxIndices) FlushAndReset();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    for (size_t i = 0; i < quadVertexCount; i++) {
        sData.QVertexBufferPtr->Position = transform * sData.QVertexPositions[i];
        sData.QVertexBufferPtr->Color = color;
        sData.QVertexBufferPtr->TextureCoordinate = textureCoords[i];
        sData.QVertexBufferPtr->TextureIndex = textureIndex;
        sData.QVertexBufferPtr->TilingFactor = tilingFactor;
        sData.QVertexBufferPtr++;
    }

    sData.QIndexCount += 6;
    sData.Stats.QuadCount++;
}

void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Reference<Texture2D> &texture, const float tilingFactor, const glm::vec4 &color) {
    DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, color);
}

void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Reference<Texture2D> &texture, const float tilingFactor, const glm::vec4 &color) {
    constexpr size_t quadVertexCount = 4;
    constexpr glm::vec2 textureCoords[] = {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f }
    };

    if (sData.QIndexCount >= RendererData::MaxIndices) FlushAndReset();

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

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    for (size_t i = 0; i < quadVertexCount; i++) {
        sData.QVertexBufferPtr->Position = transform * sData.QVertexPositions[i];
        sData.QVertexBufferPtr->Color = color;
        sData.QVertexBufferPtr->TextureCoordinate = textureCoords[i];
        sData.QVertexBufferPtr->TextureIndex = textureIndex;
        sData.QVertexBufferPtr->TilingFactor = tilingFactor;
        sData.QVertexBufferPtr++;
    }

    sData.QIndexCount += 6;
    sData.Stats.QuadCount++;
}

void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Reference<SubTexture2D> &subTexture, const float tilingFactor, const glm::vec4 &color) {
    DrawQuad({ position.x, position.y, 0.0f }, size, subTexture, tilingFactor, color);
}

void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Reference<SubTexture2D> &subTexture, const float tilingFactor, const glm::vec4 &color) {
    constexpr size_t quadVertexCount = 4;
    const glm::vec2 *textureCoords = subTexture->GetTextCoordinates();
    const Reference<Texture2D> texture = subTexture->GetTexture();

    if (sData.QIndexCount >= RendererData::MaxIndices) FlushAndReset();

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

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    for (size_t i = 0; i < quadVertexCount; i++) {
        sData.QVertexBufferPtr->Position = transform * sData.QVertexPositions[i];
        sData.QVertexBufferPtr->Color = color;
        sData.QVertexBufferPtr->TextureCoordinate = textureCoords[i];
        sData.QVertexBufferPtr->TextureIndex = textureIndex;
        sData.QVertexBufferPtr->TilingFactor = tilingFactor;
        sData.QVertexBufferPtr++;
    }

    sData.QIndexCount += 6;
    sData.Stats.QuadCount++;
}


void Renderer2D::DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, const float rotation, const glm::vec4 &color) {
    DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, const float rotation, const glm::vec4 &color) {

    constexpr size_t quadVertexCount = 4;
    const float textureIndex = 0.0f; // White Texture
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    const float tilingFactor = 1.0f;

    if (sData.QIndexCount >= RendererData::MaxIndices)
        FlushAndReset();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
    
    for (size_t i = 0; i < quadVertexCount; i++) {
        sData.QVertexBufferPtr->Position = transform * sData.QVertexPositions[i];
        sData.QVertexBufferPtr->Color = color;
        sData.QVertexBufferPtr->TextureCoordinate = textureCoords[i];
        sData.QVertexBufferPtr->TextureIndex = textureIndex;
        sData.QVertexBufferPtr->TilingFactor = tilingFactor;
        sData.QVertexBufferPtr++;
    }

    sData.QIndexCount += 6;
    sData.Stats.QuadCount++;
}

void Renderer2D::DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, const float rotation, const Reference<Texture2D> &texture, const float tilingFactor, const glm::vec4 &color) {
    DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, const float rotation, const Reference<Texture2D> &texture, const float tilingFactor, const glm::vec4 &color) {
    constexpr size_t quadVertexCount = 4;
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

    if (sData.QIndexCount >= RendererData::MaxIndices)    FlushAndReset();

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

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    for (size_t i = 0; i < quadVertexCount; i++) {
        sData.QVertexBufferPtr->Position = transform * sData.QVertexPositions[i];
        sData.QVertexBufferPtr->Color = color;
        sData.QVertexBufferPtr->TextureCoordinate = textureCoords[i];
        sData.QVertexBufferPtr->TextureIndex = textureIndex;
        sData.QVertexBufferPtr->TilingFactor = tilingFactor;
        sData.QVertexBufferPtr++;
    }

    sData.QIndexCount += 6;
    sData.Stats.QuadCount++;
}

void Renderer2D::DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, const float rotation, const Reference<SubTexture2D> &subTexture, const float tilingFactor, const glm::vec4 &color) {
    DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, subTexture, tilingFactor, color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, const float rotation, const Reference<SubTexture2D> &subTexture, const float tilingFactor, const glm::vec4 &color) {
    constexpr size_t quadVertexCount = 4;
    const glm::vec2 *textureCoords = subTexture->GetTextCoordinates();
    const Reference<Texture2D> texture = subTexture->GetTexture();

    if (sData.QIndexCount >= RendererData::MaxIndices)    FlushAndReset();

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

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    for (size_t i = 0; i < quadVertexCount; i++) {
        sData.QVertexBufferPtr->Position = transform * sData.QVertexPositions[i];
        sData.QVertexBufferPtr->Color = color;
        sData.QVertexBufferPtr->TextureCoordinate = textureCoords[i];
        sData.QVertexBufferPtr->TextureIndex = textureIndex;
        sData.QVertexBufferPtr->TilingFactor = tilingFactor;
        sData.QVertexBufferPtr++;
    }

    sData.QIndexCount += 6;
    sData.Stats.QuadCount++;
}

}

module;

#include <glm/gtc/matrix_transform.hpp>

module Ultra.Engine.Renderer2D;

import Ultra.Engine.Renderer.Buffer;
import Ultra.Engine.Renderer.PipelineState;
import Ultra.Engine.Renderer.Shader;
import Ultra.Engine.Renderer.Texture;

namespace Ultra {

static Scope<CommandBuffer> sCommandBuffer = CommandBuffer::Create();

// Properties (hidden)
struct LineVertex {
    glm::vec3 Position;
    glm::vec4 Color;
};

struct QuadVertex {
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TextureCoordinate;
    float TextureIndex;
    float TilingFactor;
};

struct RendererData {
    // General
    glm::mat4 ViewProjectionMatrix;
    bool DepthTest = true;
    Renderer2D::Statistics Stats;

    // Lines
    static const uint32_t MaxLines = 12000;
    static const uint32_t MaxLineVertices = MaxLines * 4;
    static const uint32_t MaxLineIndices = MaxLines * 6;

    Reference<PipelineState> LinePipeline;
    Reference<Buffer> LineVertexBuffer;
    Reference<Buffer> LineIndexBuffer;
    Reference<Shader> LineShader;

    uint32_t LineIndexCount = 0;
    LineVertex *LineVertexBufferBase = nullptr;
    LineVertex *LineVertexBufferPtr = nullptr;

    // Quads
    static const uint32_t MaxQuads = 21000;
    static const uint32_t MaxVertices = MaxQuads * 4;
    static const uint32_t MaxIndices = MaxQuads * 6;
    static const uint32_t MaxTextureSlots = 160; // ToDo: RenderDevice::GetCapabilities().MaxTextureUnits

    Reference<PipelineState> QuadPipeline;
    Reference<Buffer> QVertexBuffer;
    Reference<Buffer> QIndexBuffer;

    uint32_t QIndexCount = 0;
    QuadVertex *QVertexBufferBase = nullptr;
    QuadVertex *QVertexBufferPtr = nullptr;

    // Textures
    Reference<Shader> TextureShader;
    Reference<Texture> WhiteTexture;
    std::array<Reference<Texture>, MaxTextureSlots> TextureSlots;
    uint32_t TextureSlotIndex = 1; // 0 = White

    glm::vec4 QVertexPositions[4];
};

static RendererData sData;


// Default
void Renderer2D::Load() {
    // Lines
    {
        sData.LineShader = Shader::Create("Assets/Shaders/Line.glsl");

        PipelineProperties pipelineProperties;
        pipelineProperties.Layout = {
            { ShaderDataType::Float3, "aPosition" },
            { ShaderDataType::Float4, "aColor" }
        };
        sData.LinePipeline = PipelineState::Create(pipelineProperties);

        sData.LineVertexBuffer = Buffer::Create(BufferType::Vertex, nullptr, sData.MaxLineVertices * sizeof(LineVertex));
        sData.LineVertexBufferBase = new LineVertex[sData.MaxLineVertices];

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

        sData.QVertexBuffer = Buffer::Create(BufferType::Vertex, nullptr, sData.MaxVertices * sizeof(QuadVertex));
        sData.QVertexBufferBase = new QuadVertex[sData.MaxVertices];

        uint32_t offset = 0;
        uint32_t *quadIndicies = new uint32_t[sData.MaxIndices];
        for (uint32_t i = 0; i < sData.MaxIndices; i += 6) {
            quadIndicies[i + 0] = offset + 0;
            quadIndicies[i + 1] = offset + 1;
            quadIndicies[i + 2] = offset + 2;

            quadIndicies[i + 3] = offset + 2;
            quadIndicies[i + 4] = offset + 3;
            quadIndicies[i + 5] = offset + 0;
            offset += 4;
        }

        sData.QIndexBuffer = Buffer::Create(BufferType::Index, quadIndicies, sData.MaxIndices * sizeof(uint32_t));
        delete[] quadIndicies;
    }

    // Miscelaneous
    uint32_t whiteTextureData = 0xffffffff;
    //sData.WhiteTexture = Texture::Create(TextureProperties(), &whiteTextureData, sizeof(uint32_t)); // ToDo: Doesn't work anymore cause data is not set
    sData.WhiteTexture = Texture::Create(TextureProperties(), "./Assets/Textures/CheckerBoard.png");

    int32_t samplers[sData.MaxTextureSlots];
    for (uint32_t i = 0; i < sData.MaxTextureSlots; i++) samplers[i] = i;

    sData.TextureShader = Shader::Create("./Assets/Shaders/Texture.glsl");
    sData.TextureShader->Bind();
    sData.TextureShader->SetUniformBuffer("uTextures", (void *)samplers, sData.MaxTextureSlots);

    // Set all texture slots to 0
    sData.TextureSlots[0] = sData.WhiteTexture;

    sData.QVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    sData.QVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
    sData.QVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
    sData.QVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
}

void Renderer2D::Unload() {
    sData.LinePipeline.reset();
    sData.LineVertexBuffer.reset();
    sData.LineIndexBuffer.reset();
    sData.LineShader.reset();

    sData.QuadPipeline.reset();
    sData.QVertexBuffer.reset();
    sData.QIndexBuffer.reset();

    sData.TextureShader.reset();
    sData.WhiteTexture.reset();
    for (auto &texture : sData.TextureSlots) {
        texture.reset();
    }

    delete[] sData.LineVertexBufferBase;
    delete[] sData.QVertexBufferBase;
    sData.LineVertexBufferBase = nullptr;
    sData.QVertexBufferBase = nullptr;
}

void Renderer2D::StartScene(const Camera &camera) {
    sData.DepthTest = true;
    sData.ViewProjectionMatrix = camera.GetProjection();

    sData.LineIndexCount = 0;
    sData.LineVertexBufferPtr = sData.LineVertexBufferBase;

    sData.QIndexCount = 0;
    sData.QVertexBufferPtr = sData.QVertexBufferBase;

    sData.TextureSlotIndex = 1;
}

void Renderer2D::StartScene(const Camera &camera, const glm::mat4 &transform) {
    sData.DepthTest = true;
    sData.ViewProjectionMatrix = camera.GetProjection() * glm::inverse(transform);

    sData.LineIndexCount = 0;
    sData.LineVertexBufferPtr = sData.LineVertexBufferBase;

    sData.QIndexCount = 0;
    sData.QVertexBufferPtr = sData.QVertexBufferBase;

    sData.TextureSlotIndex = 1;
}

void Renderer2D::StartScene(const DesignerCamera &camera) {
    sData.DepthTest = true;
    //sData.ViewProjectionMatrix = camera.GetViewProjection();

    sData.LineIndexCount = 0;
    sData.LineVertexBufferPtr = sData.LineVertexBufferBase;

    sData.QIndexCount = 0;
    sData.QVertexBufferPtr = sData.QVertexBufferBase;

    sData.TextureSlotIndex = 1;
}

void Renderer2D::StartScene(const PerspectiveCamera &camera) {
    sData.DepthTest = true;
    //sData.ViewProjectionMatrix = camera.GetViewProjectionMatrix();

    sData.LineIndexCount = 0;
    sData.LineVertexBufferPtr = sData.LineVertexBufferBase;

    sData.QIndexCount = 0;
    sData.QVertexBufferPtr = sData.QVertexBufferBase;

    sData.TextureSlotIndex = 1;
}

void Renderer2D::FinishScene() {
    // Is the buffer empty?
    if (sData.QIndexCount == 0 && sData.LineIndexCount == 0) return;
    Flush();
}

void Renderer2D::Flush() {
    uint32_t dataSizeL = (uint32_t)((uint8_t *)sData.LineVertexBufferPtr - (uint8_t *)sData.LineVertexBufferBase);
    if (dataSizeL) {
        sData.LineVertexBuffer->UpdateData(sData.LineVertexBufferBase, dataSizeL);

        sData.LineShader->Bind();
        sData.LineShader->SetUniform("uViewProjection", sData.ViewProjectionMatrix);

        sData.LineVertexBuffer->Bind();
        sData.LinePipeline->Bind();
        sData.LineIndexBuffer->Bind();
        //RenderCommand::SetLineThickness(3.0f);
        sCommandBuffer->DrawIndexed({ sData.LineIndexCount }, PrimitiveType::Line, sData.DepthTest);
        sData.Stats.DrawCalls++;
    }

    uint32_t dataSize = (uint32_t)((uint8_t *)sData.QVertexBufferPtr - (uint8_t *)sData.QVertexBufferBase);
    if (dataSize) {
        sData.QVertexBuffer->UpdateData(sData.QVertexBufferBase, dataSize);

        sData.TextureShader->Bind();
        sData.TextureShader->SetUniform("uViewProjection", sData.ViewProjectionMatrix);

        for (uint32_t i = 0; i < sData.TextureSlotIndex; i++) {
            sData.TextureSlots[i]->Bind(i);
        }

        sData.QVertexBuffer->Bind();
        sData.QuadPipeline->Bind();
        sData.QIndexBuffer->Bind();
        sCommandBuffer->DrawIndexed({ sData.QIndexCount }, PrimitiveType::Triangle, sData.DepthTest);
        sData.Stats.DrawCalls++;
    }
}

void Renderer2D::FlushAndReset(PrimitiveType primitive) {
    switch (primitive) {
        case PrimitiveType::Line: {
            Flush();
            sData.LineIndexCount = 0;
            sData.LineVertexBufferPtr = sData.LineVertexBufferBase;
            break;
        }

        case PrimitiveType::Triangle: {
            Flush();
            sData.QIndexCount = 0;
            sData.QVertexBufferPtr = sData.QVertexBufferBase;

            sData.TextureSlotIndex = 1;
            break;
        }

        default: {
            break;
        }
    }
}


// Primitives
void Renderer2D::DrawLine(const glm::vec3 &start, const glm::vec3 &end, const glm::vec4 &color) {
    if (sData.LineIndexCount >= RendererData::MaxLineIndices) FlushAndReset(PrimitiveType::Line);

    sData.LineVertexBufferPtr->Position = start;
    sData.LineVertexBufferPtr->Color = color;
    sData.LineVertexBufferPtr++;

    sData.LineVertexBufferPtr->Position = end;
    sData.LineVertexBufferPtr->Color = color;
    sData.LineVertexBufferPtr++;

    sData.LineIndexCount += 2;

    sData.Stats.LineCount++;
}

//void Renderer2D::DrawQuad(const glm::mat4 &transform, const glm::vec4 &color, string id) {
//    constexpr size_t quadVertexCount = 4;
//    const float textureIndex = 0.0f; // White Texture
//    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
//    const float tilingFactor = 1.0f;
//
//    if (sData.QIndexCount >= RendererData::MaxIndices) FlushAndReset();
//
//    for (size_t i = 0; i < quadVertexCount; i++) {
//        sData.QVertexBufferPtr->Position = transform * sData.QVertexPositions[i];
//        sData.QVertexBufferPtr->Color = color;
//        sData.QVertexBufferPtr->TextureCoordinate = textureCoords[i];
//        sData.QVertexBufferPtr->TextureIndex = textureIndex;
//        sData.QVertexBufferPtr->TilingFactor = tilingFactor;
//        sData.QVertexBufferPtr++;
//    }
//
//    sData.QIndexCount += 6;
//    sData.Stats.Triangles += 2;
//    sData.Stats.QuadCount++;
//}

void Renderer2D::DrawQuad(const glm::mat4 &transform, const glm::vec4 &color) {
    constexpr size_t quadVertexCount = 4;
    const float textureIndex = 0.0f; // White Texture
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    const float tilingFactor = 1.0f;

    if (sData.QIndexCount >= RendererData::MaxIndices) FlushAndReset();

    for (size_t i = 0; i < quadVertexCount; i++) {
        sData.QVertexBufferPtr->Position = transform * sData.QVertexPositions[i];
        sData.QVertexBufferPtr->Color = color;
        sData.QVertexBufferPtr->TextureCoordinate = textureCoords[i];
        sData.QVertexBufferPtr->TextureIndex = textureIndex;
        sData.QVertexBufferPtr->TilingFactor = tilingFactor;
        sData.QVertexBufferPtr++;
    }

    sData.QIndexCount += 6;
    sData.Stats.Triangles += 2;
    sData.Stats.QuadCount++;
}

void Renderer2D::DrawQuad(const glm::mat4 &transform, const Reference<Texture> &texture, const float tilingFactor, const glm::vec4 &color) {
    constexpr size_t quadVertexCount = 4;
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

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

    for (size_t i = 0; i < quadVertexCount; i++) {
        sData.QVertexBufferPtr->Position = transform * sData.QVertexPositions[i];
        sData.QVertexBufferPtr->Color = color;
        sData.QVertexBufferPtr->TextureCoordinate = textureCoords[i];
        sData.QVertexBufferPtr->TextureIndex = textureIndex;
        sData.QVertexBufferPtr->TilingFactor = tilingFactor;
        sData.QVertexBufferPtr++;
    }

    sData.QIndexCount += 6;
    sData.Stats.Triangles += 2;
    sData.Stats.QuadCount++;
}

//void Renderer2D::DrawQuad(const glm::mat4 &transform, const Reference<SubTexture> &subTexture, const float tilingFactor, const glm::vec4 &color) {
//    constexpr size_t quadVertexCount = 4;
//    const glm::vec2 *textureCoords = subTexture->GetTextCoordinates();
//    const Reference<Texture> texture = subTexture->GetTexture();
//
//    if (sData.QIndexCount >= RendererData::MaxIndices) FlushAndReset();
//
//    float textureIndex = 0.0f;
//    for (uint32_t i = 1; i < sData.TextureSlotIndex; i++) {
//        if (*sData.TextureSlots[i].get() == *texture.get()) {
//            textureIndex = (float)i;
//            break;
//        }
//    }
//
//    if (textureIndex == 0.0f) {
//        if (sData.TextureSlotIndex >= RendererData::MaxTextureSlots) FlushAndReset();
//
//        textureIndex = (float)sData.TextureSlotIndex;
//        sData.TextureSlots[sData.TextureSlotIndex] = texture;
//        sData.TextureSlotIndex++;
//    }
//
//    for (size_t i = 0; i < quadVertexCount; i++) {
//        sData.QVertexBufferPtr->Position = transform * sData.QVertexPositions[i];
//        sData.QVertexBufferPtr->Color = color;
//        sData.QVertexBufferPtr->TextureCoordinate = textureCoords[i];
//        sData.QVertexBufferPtr->TextureIndex = textureIndex;
//        sData.QVertexBufferPtr->TilingFactor = tilingFactor;
//        sData.QVertexBufferPtr++;
//    }
//
//    sData.QIndexCount += 6;
//    sData.Stats.Triangles += 2;
//    sData.Stats.QuadCount++;
//}

void Renderer2D::DrawRotatedQuad(const glm::mat4 &transform, const float rotation, const glm::vec4 &color) {
    constexpr size_t quadVertexCount = 4;
    const float textureIndex = 0.0f; // White Texture
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    const float tilingFactor = 1.0f;

    if (sData.QIndexCount >= RendererData::MaxIndices) FlushAndReset();

    for (size_t i = 0; i < quadVertexCount; i++) {
        sData.QVertexBufferPtr->Position = transform * sData.QVertexPositions[i];
        sData.QVertexBufferPtr->Color = color;
        sData.QVertexBufferPtr->TextureCoordinate = textureCoords[i];
        sData.QVertexBufferPtr->TextureIndex = textureIndex;
        sData.QVertexBufferPtr->TilingFactor = tilingFactor;
        sData.QVertexBufferPtr++;
    }

    sData.QIndexCount += 6;
    sData.Stats.Triangles += 2;
    sData.Stats.QuadCount++;
}

void Renderer2D::DrawRotatedQuad(const glm::mat4 &transform, const float rotation, const Reference<Texture> &texture, const float tilingFactor, const glm::vec4 &color) {
    constexpr size_t quadVertexCount = 4;
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

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

    for (size_t i = 0; i < quadVertexCount; i++) {
        sData.QVertexBufferPtr->Position = transform * sData.QVertexPositions[i];
        sData.QVertexBufferPtr->Color = color;
        sData.QVertexBufferPtr->TextureCoordinate = textureCoords[i];
        sData.QVertexBufferPtr->TextureIndex = textureIndex;
        sData.QVertexBufferPtr->TilingFactor = tilingFactor;
        sData.QVertexBufferPtr++;
    }

    sData.QIndexCount += 6;
    sData.Stats.Triangles += 2;
    sData.Stats.QuadCount++;
}

//void Renderer2D::DrawRotatedQuad(const glm::mat4 &transform, const float rotation, const Reference<SubTexture> &subTexture, const float tilingFactor, const glm::vec4 &color) {
//    constexpr size_t quadVertexCount = 4;
//    const glm::vec2 *textureCoords = subTexture->GetTextCoordinates();
//    const Reference<Texture> texture = subTexture->GetTexture();
//
//    if (sData.QIndexCount >= RendererData::MaxIndices) FlushAndReset();
//
//    float textureIndex = 0.0f;
//    for (uint32_t i = 1; i < sData.TextureSlotIndex; i++) {
//        if (*sData.TextureSlots[i].get() == *texture.get()) {
//            textureIndex = (float)i;
//            break;
//        }
//    }
//
//    if (textureIndex == 0.0f) {
//        if (sData.TextureSlotIndex >= RendererData::MaxTextureSlots) FlushAndReset();
//
//        textureIndex = (float)sData.TextureSlotIndex;
//        sData.TextureSlots[sData.TextureSlotIndex] = texture;
//        sData.TextureSlotIndex++;
//    }
//
//    for (size_t i = 0; i < quadVertexCount; i++) {
//        sData.QVertexBufferPtr->Position = transform * sData.QVertexPositions[i];
//        sData.QVertexBufferPtr->Color = color;
//        sData.QVertexBufferPtr->TextureCoordinate = textureCoords[i];
//        sData.QVertexBufferPtr->TextureIndex = textureIndex;
//        sData.QVertexBufferPtr->TilingFactor = tilingFactor;
//        sData.QVertexBufferPtr++;
//    }
//
//    sData.QIndexCount += 6;
//    sData.Stats.Triangles += 2;
//    sData.Stats.QuadCount++;
//}


// Primitives (including transform calculation)
void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) {
    DrawQuad({ position.x, position.y, 0.0f }, size, color);
}

void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color) {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawQuad(transform, color);
}

void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Reference<Texture> &texture, const float tilingFactor, const glm::vec4 &color) {
    DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, color);
}

void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Reference<Texture> &texture, const float tilingFactor, const glm::vec4 &color) {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawQuad(transform, texture, tilingFactor, color);
}

//void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Reference<SubTexture> &subTexture, const float tilingFactor, const glm::vec4 &color) {
//    DrawQuad({ position.x, position.y, 0.0f }, size, subTexture, tilingFactor, color);
//}
//
//void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Reference<SubTexture> &subTexture, const float tilingFactor, const glm::vec4 &color) {
//    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
//
//    DrawQuad(transform, subTexture, tilingFactor, color);
//}

void Renderer2D::DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, const float rotation, const glm::vec4 &color) {
    DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, const float rotation, const glm::vec4 &color) {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawRotatedQuad(transform, rotation, color);
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

//void Renderer2D::DrawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, const float rotation, const Reference<SubTexture> &subTexture, const float tilingFactor, const glm::vec4 &color) {
//    DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, subTexture, tilingFactor, color);
//}
//
//void Renderer2D::DrawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, const float rotation, const Reference<SubTexture> &subTexture, const float tilingFactor, const glm::vec4 &color) {
//    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
//        * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
//        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
//
//    DrawRotatedQuad(transform, rotation, subTexture, tilingFactor, color);
//}


// Statistics
void Renderer2D::ResetStatistics() {
    memset(&sData.Stats, 0, sizeof(Statistics));
}

Renderer2D::Statistics Renderer2D::GetStatistics() {
    return sData.Stats;
}

}
